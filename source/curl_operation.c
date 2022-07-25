/*******************************************************************************************************
File			: curl_operatio.c
Description		: File to perform operations to fetch data from given URL
********************************************************************************************************/


#include "curl_operation.h"

/**********************************************************************************
Function    : write_callback_for_curl
Parameters  : void *pptr = pointer to start of data, size_t psize = size of data, \
			  size_t pnmemb = number of memory bytes \
			  void *pdata = data
Description : Callback function to handle error messages returned during curl_easy_setopt()
***********************************************************************************/
size_t write_callback_for_curl(void *pptr, size_t psize, size_t pnmemb, void *pdata) {    

	/*cast data to type resultstructure*/
	RESULT *result = (RESULT *)pdata;

	/*check insufficient buffer size*/
	if (result->pos + psize * pnmemb >= JSON_DATA_MAX_BUFFER_SIZE - 1) {
		fprintf(stderr, "increase JSON_DATA_MAX_BUFFER_SIZE, current size is insufficient\n");
		return 0;
	}

	/*copy to fetched data to structure*/
	memcpy(result->data + result->pos, pptr, psize * pnmemb);
	result->pos += psize * pnmemb;

	return psize * pnmemb;

}

/**********************************************************************************
Function    : curl_cleanup
Parameters  : CURL *pcurl = curl pointer ,struct curl_slist  *pheaders = curl header,\
			  char *pdata = data
Return value: 
Description : Clean up function to free allocated buffers
***********************************************************************************/
void curl_cleanup(CURL *pcurl,struct curl_slist  *pheaders, char *pdata, size_t pdata_fetched)
{
	if(pcurl != NULL) {
		curl_easy_cleanup(pcurl);
	}
	if (pheaders != NULL) {
			curl_slist_free_all(pheaders);
	}
	/*Memory has been allocated and data was not fetched fetched*/
	if ((pdata != NULL) && (pdata_fetched == 0)) {
		free(pdata);
	}

	curl_global_cleanup();

}

/**********************************************************************************
Function    : request_data_from_url
Parameters  : const char *url = JSON - url
Return value : Fetched data or NULL in case of failure
Description : Function to fetch data from url 
***********************************************************************************/
char *request_data_from_url(const char *url) {

	CURL *curl = NULL;
	/*Curl function return type*/
	CURLcode status;
	struct curl_slist *headers = NULL;
	char *data = NULL;
	long code;
	/*Flag to indicate data has been fetched. 0 - data could not be fetched 1 - data has been successfully fetched*/
	size_t data_fetched_flag = 0; 

	/*set curl environment*/
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	/*In case of failure cleanup previous allocations and return NULL*/
	if (!curl) {

		curl_cleanup(curl, headers, data, data_fetched_flag);
		return NULL;
	}

	/*Allocate memory for fetched data*/
	data = malloc(JSON_DATA_MAX_BUFFER_SIZE);

	/*In case of failure cleanup previous allocations and return NULL*/
	if (!data) {

		curl_cleanup(curl, headers, data, data_fetched_flag);
		return NULL;
	}

	RESULT result;
	result.data = data;
	result.pos = 0;

	/*set options for the curl type*/
	curl_easy_setopt(curl, CURLOPT_URL, url);
	
	headers = curl_slist_append(headers, "header");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_for_curl);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

	/*Make blocking request to url*/
	status = curl_easy_perform(curl);

	/*In case of failure cleanup previous allocations and return NULL*/
	if (status != 0) {
		
		fprintf(stderr, "Failed to request data from %s:\n", url);
		fprintf(stderr, "Error code:%s\n", curl_easy_strerror(status));

		/*Call cleanup and return*/
		curl_cleanup(curl, headers, data, data_fetched_flag);
		return NULL;

	}

	/*get data*/
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

	/*In case of failure cleanup previous allocations and return NULL*/
	if (code != 200) {
		fprintf(stderr, "Server replied with error code: %ld\n", code);

		curl_cleanup(curl, headers, data, data_fetched_flag);
		return NULL;
			
	}

	/*Set flag before  calling cleanup so data fetched is not cleaned up!*/
	data_fetched_flag = 1;
	
	/* Terminate data with null char*/
	data[result.pos] = '\0';

	/*Cleanup before exiting function*/
	curl_cleanup(curl, headers, data, data_fetched_flag);

	return data;
	
}





