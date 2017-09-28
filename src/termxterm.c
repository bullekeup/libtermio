/*
 * termxterm.c
 *
 *  Xterm specific helper functions
 */

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

#include <termio/terminal.h>
#include <termio/ansicodes.h>
#include <termio/termutils.h>
#include <termio/termxterm.h>

int check_running_xterm(const void *inst) {
	char _read[5];

	if(raw_mode((struct term_instance *)inst))
		return -1;

	read(((struct term_instance *)inst)->slave, _read, 5);
	while(strcmp(_read, ANSI_REP_STATUS_OK)) {
		write(((struct term_instance *)inst)->slave, ANSI_REQ_STATUS, sizeof(ANSI_REQ_STATUS));
		read(((struct term_instance *)inst)->slave, _read, 5);
	}

	write(((struct term_instance *)inst)->slave, ANSI_SEQ_SCREEN_CLEAR, sizeof(ANSI_SEQ_SCREEN_CLEAR));

	reset_mode((struct term_instance *)inst);
	return 0;
}

int configure_xterm(const void *inst) {
	struct termios termattr;

	tcgetattr(((struct term_instance *)inst)->slave, &termattr);
	termattr.c_iflag = (termattr.c_iflag & ~(IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK | ISTRIP | INLCR | IGNCR | IXOFF | IUCLC | IXANY | IMAXBEL)) | ICRNL | IXON;
	termattr.c_oflag = (termattr.c_oflag & ~( OLCUC | OCRNL | ONOCR | ONLRET | OFILL | OFDEL)) | OPOST | ONLCR | NL0 | CR0 | TAB0 | BS0 | VT0 | FF0;
	termattr.c_cflag = (termattr.c_cflag & ~(PARENB | PARODD | HUPCL | CSTOPB | CLOCAL | CRTSCTS)) | CS8 | CREAD;
	termattr.c_lflag = (termattr.c_lflag | (ICANON | ECHOE |ISIG | IEXTEN | ECHO | ECHOK | ECHOCTL | ECHOKE)) & ~(ECHONL | NOFLSH | XCASE | TOSTOP | ECHOPRT | FLUSHO);
	termattr.c_cc[VERASE] = '\x08';
	tcsetattr(((struct term_instance *)inst)->slave, TCSADRAIN, &termattr);

	return 0;
}

int format_cmd_xterm(char **cmd, const void *term, const void *args, const void *inst) {
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
	cmd[1] = (char *)malloc(sizeof(char) * (strlen(((struct term_desc *)term)->pty_fd) + strlen(((struct term_args *)args)->val_fd) + strlen(_tty) + 2));
	strcpy(cmd[1], ((struct term_desc *)term)->pty_fd);
	strcat(cmd[1], _tty);
	strcat(cmd[1], "/");
	strcat(cmd[1], ((struct term_args *)args)->val_fd);
	cmd[2] = ((struct term_desc *)term)->arg_title;
	cmd[3] = ((struct term_args *)args)->val_title;
	_check = 3;
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
