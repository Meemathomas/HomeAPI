/*******************************************************************************************************
File			: logger.c
Description		: File to generate logs
********************************************************************************************************/


#include"logger.h"

/********************************************************************************************
Function    : global_write_log
Parameters  : char *plog_print = contains log line, int plog_level = determines \
			  log level of logline
Description : Function to print logs into LOG.txt based on LOG_LVL set in config \
              section in .h file \
			  #define LOG_LVL 1 0=INFO, 1=DEBUG
********************************************************************************************/
void global_write_log(char *plog_print, int plog_level) {

	/*flag to print log*/	
	int print_log_flag = 0;

	/*Print INFO log*/
	if(LOG_LVL == 0){
	
		 if(plog_level == 0){

			/*enable info log prints*/
			 print_log_flag = 1; 
		
		}
	}

	/*Print INFO and DEBUG log*/
	else if(LOG_LVL == 1) {

		if((plog_level == 0) || (plog_level == 1)) {
			
			 /*enable info and debug log prints*/
			 print_log_flag = 1;			 

		}		
	}
	/*If flag is enabled, print logs*/
	if(print_log_flag == 1) {
	
		FILE *fd;
	
			char *logfilepath = getenv ("LOG_FILE_PATH");

			fd = fopen(logfilepath, "a");

			//fd = fopen ("/home/user001/samplecodejson/source/LOG.txt","a");
			if(fd == NULL) {
				 
				fprintf (stderr, "Log file couldn't be opened!");
			
				/*call the global clean up function */
				//global_cleanup_function();

				exit (-1);
			}
			
		/* write log print to file*/
		fwrite(plog_print, 1, strlen(plog_print), fd );	

		/*close file descriptor*/
		fclose(fd);
	}

}
