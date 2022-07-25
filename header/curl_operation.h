#ifndef __CURL_OPERATION_H
#define __CURL_OPERATION_H

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl.h"


/*configs*/
#define JSON_DATA_MAX_BUFFER_SIZE 10000 * 1024
#define MAX_URL_CHARS  2048
#define URL "https://cd-static.bamgrid.com/dp-117731241344/home.json"


/*structure to format the data*/
typedef struct resultstructure {
	char *data;
	int pos;
}RESULT;

/*function signatures*/
size_t write_callback_for_curl(void *pptr, size_t psize, size_t pnmemb, void *pdata);
void curl_cleanup(CURL *pcurl,struct curl_slist  *pheaders, char *pdata, size_t pdata_fetched);
char *request_data_from_url(const char *url);
#endif /*__CURL_OPERATION_H*/
