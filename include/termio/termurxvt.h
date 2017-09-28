/*
 * termurxvt.h
 *
 *  Created on: Aug 31, 2016
 *      Author: axel
 */

#ifndef LIB_TERMINAL_TERMURXVT_H_
#define LIB_TERMINAL_TERMURXVT_H_

int check_running_urxvt(const void *inst);
int format_cmd_urxvt(char **cmd, const void *term, const void *args, const void *inst);

#endif /* LIB_TERMINAL_TERMURXVT_H_ */
