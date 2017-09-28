/*
 * debug.h
 *
 * Defines debug macros
 * Skipped when building in release mode
 */

#ifndef AUTH_SRC_DEBUG_H_
#define AUTH_SRC_DEBUG_H_

/* Debug defines and code. Limit to the necessary.
 * Colors:
 * DEBUG	-	CYAN
 * INFO 	-	GREEN
 * WARNING	-	YELLOW
 * ERROR	-	MAGENTA
 * CRITICAL	-	RED*/
#if DEBUG
  #include <stdio.h>
  #if TERMLOG
    #pragma message "Debug log console handling to be implemented"
  #endif
  //#else
    #define print_debug(fmt, args...) printf( "\e[36m[DEBUG] %s:%d:%s(): " fmt "\e[39m\n", __FILE__, \
											 __LINE__, __func__, ##args)
    #define print_info(fmt, args...) printf( "\e[32m[INFO] %s:%d:%s(): " fmt "\e[39m\n", __FILE__, \
											 __LINE__, __func__, ##args)
    #define print_warning(fmt, args...) printf( "\e[33m[WARNING] %s:%d:%s(): " fmt "\e[39m\n", __FILE__, \
											 __LINE__, __func__, ##args)
    #define print_error(fmt, args...) printf( "\e[35m[ERROR] %s:%d:%s(): " fmt "\e[39m\n", __FILE__, \
											 __LINE__, __func__, ##args)
    #define print_critical(fmt, args...) printf( "\e[31m[CRITICAL] %s:%d:%s(): " fmt "\e[39m\n", __FILE__, \
											 __LINE__, __func__, ##args)
  //#endif
#else
	#define print_debug(fmt, ...)
    #define print_info(fmt, ...)
    #define print_warning(fmt, ...)
    #define print_error(fmt, ...)
    #define print_critical(fmt, ...)
#endif

#endif /* AUTH_SRC_DEBUG_H_ */
