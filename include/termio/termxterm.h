/*
 * termxterm.h
 *
 *  Created on: Aug 31, 2016
 *      Author: axel
 */

#ifndef LIB_TERMINAL_TERMXTERM_H_
#define LIB_TERMINAL_TERMXTERM_H_

int check_running_xterm(const void *inst);
int configure_xterm(const void *inst);
int format_cmd_xterm(char **cmd, const void *term, const void *args, const void *inst);

#endif /* LIB_TERMINAL_TERMXTERM_H_ */
