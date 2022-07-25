#ifndef __LOGGER_H
#define __LOGGER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*LOG LEVEL Config */
#define LOG_LVL 1 /*0=INFO, 1=DEBUG*/

/*Function signature*/
void global_write_log(char *plog_print, int plog_level);


#endif /* __LOGGER_H */





















