/*
 * termurxvt.c
 *
 *  URxvt specific helper functions
 */

#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <termio/terminal.h>
#include <termio/ansicodes.h>
#include <termio/termutils.h>
#include <termio/termurxvt.h>

int check_running_urxvt(const void *inst) {
	char _read[5];

	sleep(1);
	if(raw_mode((struct term_instance *)inst))
		return -1;

	/* DOESNT WORK ON RPIs, just bypass and hope sleep(1) will be sufficient...
	do {
		write(((struct term_instance *)inst)->slave, ANSI_REQ_STATUS, sizeof(ANSI_REQ_STATUS));
		read(((struct term_instance *)inst)->slave, _read, 5);
	} while(strcmp(_read, ANSI_REP_STATUS_OK));
	*/

	reset_mode((struct term_instance *)inst);
	return 0;
}

int format_cmd_urxvt(char **cmd, const void *term, const void *args, const void *inst) {
	int _check;
	char *_tty;

	if (cmd == NULL || term == NULL || args == NULL) {
		errno = -EINVAL;
		return -1;
	}

	_tty = ttyname(((struct term_instance *)inst)->master);
	if (_tty == NULL) {
		errno = -EINVAL;
		return -1;
	}

	cmd[0] = ((struct term_desc *)term)->cmd;
	cmd[1] = ((struct term_desc *)term)->pty_fd;
	cmd[2] = ((struct term_args *)args)->val_fd;
	cmd[3] = ((struct term_desc *)term)->arg_title;
	cmd[4] = ((struct term_args *)args)->val_title;
	_check = 4;
	if (((struct term_args *)args)->val_class != NULL){
		cmd[++_check] = ((struct term_desc *)term)->arg_class;
		cmd[++_check] = ((struct term_args *)args)->val_class;
	}
	if (((struct term_args *)args)->val_icon != NULL){
		cmd[++_check] = ((struct term_desc *)term)->arg_icon;
		cmd[++_check] = ((struct term_args *)args)->val_icon;
	}
	if (((struct term_args *)args)->val_window != NULL){
		cmd[++_check] = ((struct term_desc *)term)->arg_into_win;
		cmd[++_check] = ((struct term_args *)args)->val_window;
	}
	cmd[++_check] = NULL;

	return 0;
}
