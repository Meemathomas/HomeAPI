#compiler
CC = gcc

#Compiler flags -g = add debug info to exe, -Wall = turn on all warnings
CFLAGS = -g -Wall

#jansson library
JANSSON_LIB = -ljansson

#curl library
CURL_LIB = -lcurl

#include
INCLUDE = -I${DC_HOME}/header 

#executable
build: HomeAPI.o  logger.o  curl_operation.o
	        $(CC) $(CFLAGS) HomeAPI.o logger.o curl_operation.o -o $@ $(JANSSON_LIB) $(CURL_LIB)   #$@ == build

HomeAPI.o: source/HomeAPI.c  header/HomeAPI.h 
	        $(CC) -c $(INCLUDE) $<  #$<== first argument source/HomeAPI.c
logger.o: source/logger.c header/logger.h
		$(CC) -c $(INCLUDE) $<
curl_operation.o: source/curl_operation.c header/curl_operation.h
		$(CC) -c $(INCLUDE) $<	

clean:
	        rm -f *.o
