/*
 * terminal.c
 *
 * Implementation of functions declared in terminal.h
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pty.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <termios.h>

#include <termio/debug.h>
#include <termio/terminal.h>
#include <termio/ansicodes.h>
#include <termio/termurxvt.h>
#include <termio/termxterm.h>

#define FD_MAX_CHAR_SIZE 5

static int openpty_wrap(struct term_instance *inst) {
	if(openpty(&(inst->master), &(inst->slave), NULL, NULL, NULL)) {
		inst->master = 0;
		inst->slave = 0;
		print_error("Failed to get new pty descriptors");
		return -1;
	}
	return 0;
}

static const struct term_desc urxvtDesc = {
	.cmd = "urxvt",
	.embeddable = 1,
	.arg_class = NULL,
	.arg_title = "-T",
	.arg_icon = "-icon",
	.arg_into_win = "-embed",
	.pty_fd = "-pty-fd",
	.check_running = check_running_urxvt,
	.configure = NULL,
	.format_cmd = format_cmd_urxvt,
};

static const struct term_desc xtermDesc = {
	.cmd = "xterm",
	.embeddable = 1,
	.arg_class = "-class",
	.arg_title = "-T",
	.arg_icon = "-n",
	.arg_into_win = "-into",
	.pty_fd = "-S",
	.check_running = check_running_xterm,
	.configure = configure_xterm,
	.format_cmd = format_cmd_xterm,
};

static const char *defaultTermTitle = "M2SL log terminal";

static struct term_desc *supportedTerms[2] = {
	(struct term_desc *)&urxvtDesc,
	(struct term_desc *)&xtermDesc
};

struct term_desc *term_desc(char *termcmd) {
	int i;
	struct term_desc *_term, *_ptr;

	_term = NULL;

	for(i = 0; i < sizeof(supportedTerms); i++){
		if (strcmp(supportedTerms[i]->cmd, (const char *)termcmd) != 0) {
			continue;
		}
		else {
			print_debug("Found a valid term_desc structure for %s", termcmd);
			_term = (struct term_desc *)malloc(sizeof(struct term_desc));
			_ptr = memcpy(_term, supportedTerms[i], sizeof(struct term_desc));
			if (_ptr != _term) {
				errno = -EFAULT;
				return NULL;
			}
			break;
		}
	}

	if (_term == NULL) {
		errno = -EINVAL;
		return NULL;
	}
	return _term;
}

/*
 * As you can (and probably should) pass pre-filled structures
 * we don't clean up on error. So you MUST check the result of
 * function call and perform memory cleanup if necessary.
 */
int spawn_term(struct term_instance *inst) {
	void *_ptr;
	int _check;
	char *cmd[12];

	/* Check inst parameter */
	if (inst == NULL) {
		goto bad_param;
	}
	else if (inst->pid != 0) {
		print_debug("Instance PID already set, won't spawn another terminal");
		goto bad_param;
	}

	/* Check term parameter. It must not be NULL since terminal
	 * descriptions are generated before and must be assigned from existing
	 * ones. User must retrieve one from term_desc function before call. */
	if (inst->term == NULL) {
		print_error("struct inst term member is NULL");
		goto bad_param;
	}

	/* Check inst parameter. Same as term struct member
	 * must be retrieved with term_args function before call. */
	if (inst->args == NULL) {
		print_error("No args found in inst");
		goto bad_param;
	}
	else if(inst->args->val_title == NULL) {
		inst->args->val_title = (char *)malloc(sizeof(char) * (strlen(defaultTermTitle) + 1));
		_ptr = memcpy((void *)inst->args->val_title, (const void *)defaultTermTitle, (strlen(defaultTermTitle) + 1));
		if (_ptr != inst->args->val_title)
			return -1;
		print_debug("No term_args structured passed");
	}

	/* Get new pty descriptors */
	if ((inst->master != 0) && (inst->slave != 0)) {
		print_warning("Instance already have master and slave fds, should not be the case");
	}
	else if (inst->master != 0) {
		print_debug("Instance has master fd but not slave...");
		goto bad_param;
	}
	else if (inst->slave != 0) {
		print_debug("Instance has slave fd but not master...");
		goto bad_param;
	}
	else {
		print_debug("Open new pty fd pair");
		if (openpty_wrap(inst) != 0)
			return -1;
	}

	if (inst->args->val_fd == NULL) {
		print_debug("Converting master fd to char for terminal argument");
		inst->args->val_fd = (char *)malloc(sizeof(char) * FD_MAX_CHAR_SIZE);
		if (snprintf(inst->args->val_fd, FD_MAX_CHAR_SIZE, "%d", inst->master) <= 0) {
			perror(NULL);
			goto bad_param;
		}
	}
	else {
		_check = (int)strtol(inst->args->val_fd, NULL, 10);
		if (inst->master != _check) {
			print_error("Inconsistent master FD between term_instance and term_args");
			goto bad_param;
		}
	}

	/* Fork then exec the term in the new process */
	inst->pid = fork();
	if (inst->pid == 0) {
		close(inst->slave);
		print_debug("Creating arg list for execlp");
		if (inst->term->format_cmd != NULL)
			inst->term->format_cmd(cmd, inst->term, inst->args, inst);

		print_debug("Replace current process with %s", inst->term->cmd);
		execvp(inst->term->cmd, cmd);
		perror(NULL);
		errno = -ENOEXEC;
		return -1;
	}
	else if (inst->pid < 0) {
		close(inst->master);
		close(inst->slave);
		inst->pid = 0;
		errno = -EBADF;
		return -1;
	}

	close(inst->master);
	if (inst->term->check_running != NULL)
		inst->term->check_running(inst);
	if (inst->term->configure != NULL)
		inst->term->configure(inst);

	return 0;

bad_param:
	errno = -EINVAL;
	return -1;
}

int kill_term(struct term_instance *inst) {
	if (inst == NULL)
		goto bad_param;

	if (inst->pid == 0)
		goto bad_param;

	if (kill(inst->pid, SIGKILL) != 0)
		goto bad_param;
	close(inst->slave);
	if(inst->attrs != NULL)
		free(inst->attrs);
	return 0;

bad_param:
	errno = -EINVAL;
	return -1;
}
