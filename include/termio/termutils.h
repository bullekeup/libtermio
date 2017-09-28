/*
 * termutils.h
 *
 * Common terminal utilities
 */

#ifndef LIB_TERMINAL_TERMUTILS_H_
#define LIB_TERMINAL_TERMUTILS_H_

#include <termio/terminal.h>

int raw_mode(struct term_instance *inst);
int reset_mode(struct term_instance *inst);

#endif /* LIB_TERMINAL_TERMUTILS_H_ */
