/*
 * terminal.h
 *
 * Define functions to use a log console
 */

#ifndef UTILS_TERMINAL_H_
#define UTILS_TERMINAL_H_

#include <sys/types.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>

struct term_desc {
	char *cmd;
	uint32_t embeddable;
	char *arg_class;
	char *arg_title;
	char *arg_icon;
	char *arg_into_win;
	char *pty_fd;
	int (*check_running)(const void *);
	int (*configure)(const void *);
	int (*format_cmd)(char **, const void *, const void *, const void *);
};

struct term_args {
	char *val_window;
	char *val_class;
	char *val_title;
	char *val_icon;
	char *val_fd;
};

struct term_instance {
	struct term_desc *term;
	struct term_args *args;
	int master;
	int slave;
	pid_t pid;
	struct termios *attrs;
};

/* Spawn the term and fill term struct. If NULL allocate new one. If termcmd is NULL, open default terminal */
struct term_desc *term_desc(char *termcmd);
int spawn_term(struct term_instance *inst);
int kill_term(struct term_instance *inst);

#endif /* UTILS_TERMINAL_H_ */
