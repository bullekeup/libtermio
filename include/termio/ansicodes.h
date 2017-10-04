/*
 * ansicodes.h
 *
 *  Created on: Aug 31, 2016
 *      Author: axel
 */

#ifndef LIB_TERMINAL_ANSICODES_H_
#define LIB_TERMINAL_ANSICODES_H_

#define ANSI_ESC "\e"
#define ANSI_CSI "\e["

#define ANSI_REQ_STATUS ANSI_ESC "[5n"

#define ANSI_REP_STATUS_OK ANSI_ESC "[0n"
#define ANSI_REP_STATUS_nOK ANSI_ESC "[3n"

#define ANSI_SEQ_SCREEN_CLEAR ANSI_ESC "[2J"

#endif /* LIB_TERMINAL_ANSICODES_H_ */
