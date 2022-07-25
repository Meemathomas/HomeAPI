1. Getting Started
        This document lists the system prerequisites, configurations, working directory structure, compilation and installation and running.
2. Working Directory Structure:
	HOMEAPI - This is the working directory. It contains the following sub directories:
	•	source - contains all .c files
	•	header - contains all .h files
	•	lib    - contains all .so files
	•	log    - contains the log LOG.txt
3. Compiling and running: 
	Prerequisites for build:	
	•	Operating System = Ubuntu - 18.04
	•	C Standard = c11
	•	C Compiler = gcc - 7.5.0
	Third Party Libraries:	
	•	Jansson - 2.11-1
	•	curl - 7.58.0-2ubuntu3.19
	Compiling
		Run the following commands on the CLI in the HOMEAPI directory
		make clean
		make build
	Running  the binary:

		1.	 Set the environment variables and configurations as mentioned below:	
			•	ENVIRONMENT VARIABLES TO SET IN ~/.bashrc
		2. 	Save the following entries in the .bashrc file. Ensure the path to the home directory is set to the working directory path e.g : export 				DC_HOME=/home/user001/samplecodejson/HOMEAPI
		
			#for HomeAPI
			export DC_HOME=<Enter your full path to working directory>/HOMEAPI
			export LD_LIBRARY_PATH=${DC_HOME}/lib
			export LOG_FILE_PATH=${DC_HOME}/log/LOG.txt
		3.	Set environment variables by running command:
			source  ~/.bashrc
		4.	Configuration settings:
			•	logger.h 	
				 Set log level to 0 or 1 for info or debug logs
				 #define LOG_LVL 1 /*0=INFO, 1=DEBUG*/
			•	curl_operation.h
 	              		Set the url for the JSON page as below
 	            		 #define URL "https://cd-static.bamgrid.com/dp-117731241344/home.json"
		5.	Unzip and copy the HOMEAPI directory to home directory path set in environment variable DC_HOME.
		6.	Run the binary "build" in the HOMEAPI directory using the following command
				 ./build
4.  Use Cases Implemented:
	1	To fetch JSON data from a given URL page, parse and collect Titles of streaming content and expose for the front end
	2.	To facilitate a search through the Titles, to display all available content matches when a partial search string is inputted 
5. Execution Steps

	1.	Run the binary in the HOMEAPI directory using the command: 
		./build
	2.	The Logger will start generating the logs in LOG.txt while the data from the URL is fetched and parsed. When the parsing is complete, the 			programming content will be displayed on CLI.
	3.	The system will prompt to enter a search title from the list displayed. Enter full/partial search string to get the listing of all programs 			corresponding to the search.



