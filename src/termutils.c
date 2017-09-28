/*
 * termutils.c
 *
 * Common terminal utilities
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <termio/terminal.h>

int raw_mode(struct term_instance *inst) {
	struct termios termattrs;

	if (inst == NULL) {
		errno = -EINVAL;
		return -1;
	}
	if (inst->pid == 0) {
		errno = -EINVAL;
		return -1;
	}
	if (inst->slave == 0){
		errno = -EINVAL;
		return -1;
	}

	tcgetattr(inst->slave, &termattrs);
	if (inst->attrs == NULL)
		inst->attrs = (struct termios *)malloc(sizeof(struct termios));
	memcpy((void *)inst->attrs, (const void *)&termattrs, sizeof(struct termios));

	termattrs.c_iflag = termattrs.c_iflag & ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	termattrs.c_oflag = termattrs.c_oflag & ~OPOST;
	termattrs.c_cflag = (termattrs.c_cflag & ~(CSIZE | PARENB)) | CS8;
	termattrs.c_lflag = termattrs.c_lflag & ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

	if (tcsetattr(inst->slave, TCSANOW, &termattrs) != 0){
		tcsetattr(inst->slave, TCSANOW, inst->attrs);
		errno = -EINVAL;
		return -1;
	}

	return 0;
}

int reset_mode(struct term_instance *inst) {
	struct termios oldattrs;

	if(inst->attrs == NULL){
		errno = -EINVAL;
		return -1;
	}
	if (inst->pid == 0) {
		errno = -EINVAL;
		return -1;
	}
	if (inst->slave == 0){
		errno = -EINVAL;
		return -1;
	}

	tcgetattr(inst->slave, &oldattrs);
	if (tcsetattr(inst->slave, TCSANOW, inst->attrs)){
		tcsetattr(inst->slave, TCSANOW, &oldattrs);
		errno = -EINVAL;
		return -1;
	}
	return 0;
}

