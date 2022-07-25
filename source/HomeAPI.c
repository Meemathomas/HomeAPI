/*******************************************************************************************************
File			: HomeAPI.c
Description		: File to expose the data in JSON file for consumption by FRONT END 
********************************************************************************************************/

#include"HomeAPI.h"
#include"logger.h"
#include"curl_operation.h"


/*Variable Declarations*/

/*Global pointer to map*/
MAP *global_map_ptr = NULL;


/*found container flag*/
int global_container_found_flag = 0; 
/*keep track of the current index*/
int global_tree_level_array_index = -1;
/*index for subtree members arrya*/
//int global_subtree_members_count_array_index = 0;
/*store the current key*/
//char global_current_key[MAX_KEY_CHARS];


/*Array Declarations*/

/*Array to keep track of the key names of the subtree*/
char *global_subtree_keys_array[MAX_SUBTREES_LIMIT];
/*Array to keep track of subtrees members count*/
//size_t global_subtree_members_count_array[MAX_SUBTREES_LIMIT];


/*******************************************************************************
Function    : global_cleanup_function()
Parameters  : 
Description : Function called before exiting to clean up allocated memory blocks
*********************************************************************************/
void global_cleanup_function() {

	char buffer[MAX_CHARS];


	sprintf(buffer, "\n%s:%s:%d: Cleaning up before exit", __FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);

	/*Iterate through keys array and free the pointers*/
	for(int local_ctr = 0 ; local_ctr < MAX_SUBTREES_LIMIT ; local_ctr++) {
	
			if(global_subtree_keys_array[local_ctr] != NULL) {
			free(global_subtree_keys_array[local_ctr]);
		}
	}

	/*Iterate through map and free the allocated memory*/
	MAP *temp = NULL;
	while(global_map_ptr->link !=NULL) {
	
		temp = global_map_ptr->link;
		free(global_map_ptr);
		global_map_ptr = temp;
	}

	free(global_map_ptr);

}



/*******************************************************************************************
Function    : global_update_map
Parameters  : const char *pValue = contains the search content, \
			  MAP *pmap_ptr = points to the first record in the map 
Description : Function to update the link list which simulates \
			  map of key,value pairs in the json
********************************************************************************************/
void global_update_map(const char *pValue, MAP *pmap_ptr) {

	char buffer[MAX_CHARS];

	sprintf(buffer, "\n%s:%s:%d: Entered global_update_map", __FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);
		
	/*if the map has not been created, create first record*/
	if(pmap_ptr == NULL) {
	
		sprintf(buffer, "\n%s:%s:%d: pmap_ptr = NULL ", __FILE__, __FUNCTION__, __LINE__);	
		global_write_log(buffer, 1);
		memset(buffer,'\0',MAX_CHARS);
		
		pmap_ptr = (MAP *) malloc (sizeof(MAP));

		/*In case of allocation failure cleanup and exit*/
		if(pmap_ptr == NULL) {
			//fprintf (stderr, "Error while copying keyname to key array");
			sprintf(buffer, "\n%s:%s:%d: Error while storing key value pair to map. Calling cleanup and exiting!", __FILE__, __FUNCTION__, __LINE__);
			global_write_log(buffer, 1);
			memset(buffer,'\0',MAX_CHARS);
			
			/*call cleanup*/
			global_cleanup_function();
			exit(-1);
		}

		// store the head of this map into a global variable
		global_map_ptr = pmap_ptr;

		/*get the key from the global_tree_level_array_index and store in key field of record*/
		strcpy(pmap_ptr->key, global_subtree_keys_array[global_tree_level_array_index]);

		/*get the value from the json string and store in value field of record*/
		strcpy(pmap_ptr->value, pValue);

			
		pmap_ptr->link = NULL;

		sprintf(buffer, "\n%s:%s:%d: End of if loop", __FILE__, __FUNCTION__, __LINE__);	
		global_write_log(buffer, 0);
		memset(buffer,'\0',MAX_CHARS);

			
	}
	else {
	
		sprintf(buffer, "\n%s:%s:%d: Entered else loop", __FILE__, __FUNCTION__, __LINE__);	
		global_write_log(buffer, 1);
		memset(buffer,'\0',MAX_CHARS);

		/* Traverse through the list till we reach the last node*/
		while(pmap_ptr->link != NULL) {
		
			pmap_ptr = pmap_ptr->link; 
		
		}
						
		/*Found last record Create new record and link it to the map*/
		pmap_ptr->link = (MAP *) malloc (sizeof(MAP));

		/*In case of allocation failure cleanup and exit*/
		if(pmap_ptr == NULL) {
		
			sprintf(buffer, "\n%s:%s:%d: Error while storing key value pair to map. Calling cleanup and exiting!", __FILE__, __FUNCTION__, __LINE__);	
			global_write_log(buffer, 1);
			memset(buffer,'\0',MAX_CHARS);
		
			/*call cleanup*/
			global_cleanup_function();
			exit(-1);
		}

		/*traverse to new record to store the key,value*/
		pmap_ptr = pmap_ptr->link; 

		/*get the key from the global_tree_level_array_index and store in key field of record*/
		strcpy(pmap_ptr->key, global_subtree_keys_array[global_tree_level_array_index]);

		/*get the value from the json string and store in value field of record*/
		strcpy(pmap_ptr->value, pValue);

		/*set this as new end of map*/
		pmap_ptr->link = NULL;

	}

	sprintf(buffer, "\n%s:%s:%d: Exiting global_update_map", __FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);

}



/**********************************************************************************
Function    : global_print_map
Parameters  : MAP *pmap_ptr = points to the first record in the map
Description : Function to print the data in the map
***********************************************************************************/
void global_print_map(MAP *pmap_ptr ) {

	char buffer[MAX_CHARS];

	sprintf(buffer, "\n%s:%s:%d: Entered function printing map", __FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);

	if(pmap_ptr == NULL) {
		
		sprintf(buffer, "\n%s:%s:%d: Head pointer to map is NULL", __FILE__, __FUNCTION__, __LINE__);	
		global_write_log(buffer, 1);
		memset(buffer,'\0',MAX_CHARS);	

		/*call cleanup function*/
		global_cleanup_function();

		exit(-1);
	}
	
	do {

		/* Disabled due to size of map .If enabled, increase buffer size > 9000 according to warning
		sprintf(buffer, "\n%s:%s:%d: key = %s, Value = %s", __FILE__, __FUNCTION__, __LINE__,pmap_ptr->key,pmap_ptr->value);	
		global_write_log(buffer, 1);
		memset(buffer,'\0',MAX_CHARS);	*/

		fprintf (stdout, "\n%s : %s",pmap_ptr->key,pmap_ptr->value);
			
		pmap_ptr = pmap_ptr->link; 
				
	}while(pmap_ptr->link != NULL);
	
	sprintf(buffer, "\n%s:%s:%d: Exiting function printing map",__FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);

}


/*************************************************************************************
Function    : parse_json
Parameters  : json_t *pRoot = root node, const char *pSearchString = string to search for
Description : Function to parse through JSON structure
**************************************************************************************/
void parse_json(json_t *pRoot, const char *pSearchString) {

	char buffer[MAX_CHARS];

	sprintf(buffer, "\n%s:%s:%d: Entered function to parse through JSON OBJECT tree",__FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);

	parse_json_path(pRoot, pSearchString);

	sprintf(buffer, "\n%s:%s:%d: Exiting function to parse through JSON OBJECT tree",__FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);
}


/*************************************************************************************
Function    : parse_json
Parameters  : json_t *pRoot = root node, const char *pSearchString = string to search for
Description : Function to parse through each JSON type in the JSON structure
**************************************************************************************/
void parse_json_path(json_t *pElement, const char *pSearchString) {

	char buffer[MAX_CHARS];

	sprintf(buffer, "\n%s:%s:%d: Entered function to parse through each JSON type in the JSON tree",__FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);


	switch (json_typeof(pElement)) {
		case JSON_OBJECT:
			parse_json_object(pElement, pSearchString);
			break;
		case JSON_ARRAY:
			parse_json_array(pElement, pSearchString);
			break;
		case JSON_STRING:
			//parse_json_string(pElement, pSearchString);
			break;
		case JSON_INTEGER:
			parse_json_integer(pElement);
			break;
		case JSON_REAL:
			parse_json_real(pElement);
			break;
		case JSON_TRUE:
			parse_json_true(pElement);
			break;
		case JSON_FALSE:
			parse_json_false(pElement);
			break;
		case JSON_NULL:
			parse_json_null(pElement);
			break;
		default:
			fprintf(stderr, "unrecognized JSON type %d\n", json_typeof(pElement));
	}

	sprintf(buffer, "\n%s:%s:%d: Exiting function to parse through each JSON type in the JSON tree",__FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);
}


/********************************************************************************************
Function    : json_plural
Parameters  : size_t pcount
Description : Function used for logging to type the 's' when there are mre than 1\
			  array elements or object elements
********************************************************************************************/
const char *json_plural(size_t pcount){
	return pcount == 1 ? "" : "s";
}


/***********************************************************************************
Function    : parse_json_object
Parameters  : json_t *pElement = root node, const char *pSearchString
Description : Function to process a JSON object of elements
***********************************************************************************/
void parse_json_object(json_t *pElement, const char *pSearchString) {


	size_t size;
	const char *key;
	json_t *value;
	char formatted_string[MAX_CHARS];
	char buffer[MAX_CHARS];

	sprintf(buffer, "\n%s:%s:%d:Entered function to process JSON object elements", __FILE__, __FUNCTION__, __LINE__);
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);
	
	/*get the number of elements in the JSON object*/
	size = json_object_size(pElement);

	sprintf(buffer, "\n%s:%s:%d:JSON Object of %lld pair%s:", __FILE__, __FUNCTION__, __LINE__, (long long)size, json_plural(size));
	global_write_log(buffer, 1);
	memset(buffer,'\0',MAX_CHARS);

	json_object_foreach(pElement, key, value) {
	
		sprintf(buffer, "\n%s:%s:%d: JSON Key:%s", __FILE__, __FUNCTION__, __LINE__, key);
		global_write_log(buffer, 1);
		memset(buffer,'\0',MAX_CHARS);

		/*if containers subtree found, set global flag*/
		//if(strcmp(key,"default") == 0) {
		if(strcmp(key,"full") == 0) {
		
			sprintf(buffer, "\n%s:%s:%d: Container subtree found", __FILE__, __FUNCTION__, __LINE__);
			global_write_log(buffer, 1);
			memset(buffer,'\0',MAX_CHARS);
			

			/*set flag to indicate container found*/
			global_container_found_flag = 1;
			
		}
		else if(global_container_found_flag == 1) { /*key = default was found previously check if the 1st key in next obj is content*/

			if((strcmp(key,"content")) == 0) {

				sprintf(buffer, "\n%s:%s:%d: Content key found", __FILE__, __FUNCTION__, __LINE__);
				global_write_log(buffer,1);
				memset(buffer,'\0',MAX_CHARS);
			
				/*increment index to make a new entry of subtree key*/
				global_tree_level_array_index = global_tree_level_array_index + 1;

				/*allocate memory to store the key*/
				global_subtree_keys_array[global_tree_level_array_index] = (char*) malloc(MAX_KEY_CHARS);
			
				/*In case of allocation failure cleanup and exit*/
				if(global_subtree_keys_array[global_tree_level_array_index] == NULL) {
				
					sprintf(buffer, "\n%s:%s:%d: Error while copying key name to key array Calling cleanup and exiting!", __FILE__, __FUNCTION__, __LINE__);
					global_write_log(buffer,1);
					memset(buffer,'\0',MAX_CHARS);
			
					/*call cleanup*/
					global_cleanup_function();
					exit(-1);
				}

				sprintf(buffer, "\n%s:%s:%d: Allocated space for key in keys array", __FILE__, __FUNCTION__, __LINE__);
				global_write_log(buffer, 1);
				memset(buffer,'\0',MAX_CHARS);
			
				/*initialize to \0*/
				memset(global_subtree_keys_array[global_tree_level_array_index], '\0', MAX_KEY_CHARS);

				sprintf(buffer, "\n%s:%s:%d: Copied key in keys array", __FILE__, __FUNCTION__, __LINE__);	
				global_write_log(buffer, 1);
				memset(buffer,'\0',MAX_CHARS);
			
				/*Store the key into the global array*/
				strcpy(global_subtree_keys_array[global_tree_level_array_index],key); 


				sprintf(buffer, "\n%s:%s:%d: Calling parse_json_string", __FILE__, __FUNCTION__, __LINE__);
				global_write_log(buffer, 1);
				memset(buffer,'\0',MAX_CHARS);
			
				/* Print the json_string*/
				parse_json_string(value, pSearchString);

				sprintf(buffer, "\n%s:%s:%d: finished parse_json_string for the content to map", __FILE__, __FUNCTION__, __LINE__);
				global_write_log(buffer, 1);
				memset(buffer,'\0',MAX_CHARS);

				/*reset flag */
				global_container_found_flag = 0;

				/*finished storing to map, continue to next foreach element */
				sprintf(buffer, "\n%s:%s:%d: Continuting with other members of default obj", __FILE__, __FUNCTION__, __LINE__);
				global_write_log(buffer, 1);
				memset(buffer,'\0',MAX_CHARS);

				continue;
				
			}
		}

		/* Iterate throug the next element in the object*/
		parse_json_path(value, pSearchString);
	}

	sprintf(buffer, "\n%s:%s:%d:Exiting function to process JSON object elements", __FILE__, __FUNCTION__, __LINE__);
	global_write_log(buffer, 1);
	memset(buffer,'\0',MAX_CHARS);
}


/********************************************************************************
Function    : parse_json_array
Parameters  : json_t *pElement = root node, const char *pSearchString
Description : Function to process the JSON array elements
**********************************************************************************/
void parse_json_array(json_t *pElement, const char *pSearchString) {
	
	size_t i;
	char buffer[MAX_CHARS];
	size_t size = json_array_size(pElement);
	
	sprintf(buffer, "\n%s:%s:%d:JSON Array of %lld element%s:", __FILE__, __FUNCTION__, __LINE__, (long long)size, json_plural(size));
	global_write_log(buffer, 1);
	memset(buffer,'\0',MAX_CHARS);	

	for (i = 0; i < size; i++) {

		/*Iterate through each array element */
		parse_json_path(json_array_get(pElement, i), pSearchString);
	}
	
}


/*********************************************************************************
Function    : parse_json_string
Parameters  : json_t *pElement = root node, const char* pSearchString
Description : Function to process a JSON string element
**********************************************************************************/
void parse_json_string(json_t *pElement, const char* pSearchString) {

	char buffer[MAX_CHARS];
	
	sprintf(buffer, "\n%s:%s:%d: inside  printing json string value : %s", __FILE__, __FUNCTION__, __LINE__,json_string_value(pElement));
	global_write_log(buffer, 1);
	memset(buffer,'\0',MAX_CHARS);	
	

	/*if global_container_found_flag is set to 1 then store the key value pair in map*/
	if(global_container_found_flag == 1) {
	
		sprintf(buffer, "\n%s:%s:%d: Calling global_update_map with string, %s", __FILE__, __FUNCTION__, __LINE__,json_string_value(pElement));
		global_write_log(buffer, 1);
		memset(buffer,'\0',MAX_CHARS);
		
		global_update_map(json_string_value(pElement), global_map_ptr);
	}
	
}

void parse_json_integer(json_t *pElement) {
	
	/*char formatted_value[10];
	
	printf("JSON Integer: \"%" JSON_INTEGER_FORMAT "\"\n", json_integer_value(pElement));
	/*if global_container_found_flag is set to 1 then store the key value pair in map
	if(global_container_found_flag == 1)
	{
		sprintf(formatted_value, "\"%" JSON_INTEGER_FORMAT "\"", json_integer_value(pElement)); 

		global_update_map(formatted_value);

		global_update_keys_array();
	}*/
}

void parse_json_real(json_t *pElement) {

	/*char formatted_value[10];

	printf("JSON Real: %f\n", json_real_value(pElement));
	/*if global_container_found_flag is set to 1 then store the key value pair in map
	if(global_container_found_flag == 1)
	{
		sprintf(formatted_value, "%f", json_real_value(pElement)); 

		global_update_map(formatted_value);

		global_update_keys_array();
	}
	*/
}

void parse_json_true(json_t *pElement) {

/*	char formatted_value[10];	
	
	(void)pElement;
	
	printf("JSON True\n");
	/*if global_container_found_flag is set to 1 then store the key value pair in map
	if(global_container_found_flag == 1)
	{
		sprintf(formatted_value, "%s", "True")); 

		global_update_map(formatted_value);

		global_update_keys_array();
	}
	*/
}

void parse_json_false(json_t *pElement) {

	/*char formatted_value[10];

	(void)pElement;
	
	printf("JSON False\n");
	/*if global_container_found_flag is set to 1 then store the key value pair in map
	if(global_container_found_flag == 1)
	{
		sprintf(formatted_value, "%s", "False")); 

		global_update_map(formatted_value);

		global_update_keys_array();
	}
	*/
}

void parse_json_null(json_t *pElement) {

/*	char formatted_value[10];

	(void)pElement;
	
	printf("JSON Null\n");
	/*if global_container_found_flag is set to 1 then store the key value pair in map
	if(global_container_found_flag == 1)
	{
		sprintf(formatted_value, "%s", "Null")); 

		global_update_map(formatted_value);

		global_update_keys_array();
	}
	*/
}


/**************************************************
Function    : load_json
Parameters  : const char *text
Description : Function to parse a text to a valid JSON
***************************************************/
json_t *load_json(const char *pText) {
	
	json_t *root;
	json_error_t error;

	root = json_loads(pText, 0, &error);

	if (root) {

		return root;

	}else {

		fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
		return (json_t *)0;
	}
}


/***********************************************************************************
Function    : convert_string_to_upper_case
Parameters  : const char *pSourceString, char *pDestinationString
Description : Function to convert a given source string to all uppercase characters
************************************************************************************/

void convert_string_to_upper_case(const char *pSourceString, char *pDestinationString) {

		if(pSourceString != NULL) {

			/*Iterate through the source string convert each character in the string to upper case*/
			size_t len = strlen(pSourceString);
			
			for(int ctr = 0; ctr < len; ctr++) {
				
				*pDestinationString = toupper(*pSourceString++);

				pDestinationString++;
			}
			
		}else {

			/*set the pDestinationString to NULL as conversion was not done*/
			pDestinationString = NULL;
		}

		
    }


/**********************************************************************
Function    : search_by_title
Parameters  : const char *pSearchString = search string, MAP *pmap_ptr = pointer to the first record of map
Description : Function to extract all titles matching a  pattern from a valid JSON
***********************************************************************/
void search_by_title(const char *pSearchString, MAP *pmap_ptr){

	char buffer[MAX_CHARS];
	char *upper_case_pSearchString = NULL;
	char *upper_case_value = NULL;
	

	//fprintf(stdout,"strlen of   %s = %ld",pSearchString,strlen(pSearchString));

	sprintf(buffer, "\n%s:%s:%d: Entered search by title function", __FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);	

	/*NULL check*/
	if ((pSearchString == NULL) || (pmap_ptr == NULL)) {
		
		sprintf(buffer, "\n%s:%s:%d: NULL pointer encountered", __FILE__, __FUNCTION__, __LINE__);	
		global_write_log(buffer, 1);
		memset(buffer,'\0',MAX_CHARS);	

		fprintf (stdout, "\nNo match found!");

		return;

	}

	/*Allocate space*/
	upper_case_pSearchString = (char *) malloc (strlen(pSearchString) + 1);

	/*Set allocated block to all null characters*/
	memset(upper_case_pSearchString,'\0',strlen(pSearchString) + 1);	

	/*Convert the search string to upper case for comparison*/
	convert_string_to_upper_case(pSearchString, upper_case_pSearchString);

	/*Search through the map for all partial matches to the search string*/
	while(pmap_ptr != NULL) {

		upper_case_value = (char *) malloc (strlen(pmap_ptr->value) + 1);

		memset(upper_case_value,'\0',strlen(pmap_ptr->value) + 1);	

		convert_string_to_upper_case(pmap_ptr->value, upper_case_value);

		sprintf(buffer, "\n%s:%s:%d:map value str:%s search string:%s ", __FILE__, __FUNCTION__, __LINE__,upper_case_value, upper_case_pSearchString);	
		global_write_log(buffer, 0);
		memset(buffer,'\0',MAX_CHARS);	
		
		fflush(stdout);

		if(strstr(upper_case_value, upper_case_pSearchString) != NULL) {
				
			/*title match found*/
			fprintf (stdout, "\n%s",pmap_ptr->value);
		}
			
		/*Move to the next map record*/
		pmap_ptr = pmap_ptr->link; 

		/*free the allocated memory block and set to reser to NULL*/
		free(upper_case_value);

		upper_case_value = NULL;
		
	}
	sprintf(buffer, "\n%s:%s:%d: Exiting search by title function", __FILE__, __FUNCTION__, __LINE__);	
	global_write_log(buffer, 0);
	memset(buffer,'\0',MAX_CHARS);	

	free(upper_case_pSearchString);

}
	

int main(int argc, char *argv[]) {
	
	char search_title[MAX_CHARS];
	char *buffer = NULL;
	FILE *fd;
	size_t length;
	char log_buffer[MAX_CHARS];

	if (argc != 1) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		exit(-1);
	}


/********************************************************CURL*****************************/

	char *text;
	
	//get the URL from config
	char url[MAX_URL_CHARS] = URL;
		
	text = request_data_from_url(url);
	if (!text) {
			
		fprintf(stdout,"Error reading from URL. Exiting..!");
		exit(-1);
	}
	fflush(stdout);
		
/********************************************CURL****************************************/
	
	fprintf(stdout,"Building Title list...");
	fflush(stdout);
	
	sprintf(log_buffer, "\n%s:%s:%d: ==========================Starting LOG=============================", __FILE__, __FUNCTION__, __LINE__);
	global_write_log(log_buffer, 0);
	memset(log_buffer,'\0',MAX_CHARS);

	json_t *root = load_json(text);

	char search_string[10] = "dummy";
			
	if (root) {

		/* print the JSON structure*/ 
		parse_json(root, search_string);

		/*Expose content*/
		global_print_map(global_map_ptr);

		memset(search_title,'\0',MAX_CHARS);
		
		while(1){					

			fprintf(stdout,"\n\nSearch Title :");
					
			fgets(search_title, MAX_CHARS, stdin); //check failure scenatrio

			/*No valid search string entered, exit program */
			if (strlen(search_title) <= 1) { 
						
				fprintf(stdout,"\n No valid search string entered, Exiting...\n");
				break;
			}

			/*Replace \n with \0 in last char of input*/
			search_title[strlen(search_title)-1] = '\0';

			sprintf(log_buffer, "\n%s:%s:%d:Searching for entered title: %s", __FILE__, __FUNCTION__, __LINE__,search_title);
			global_write_log(log_buffer, 0);
			memset(log_buffer,'\0',MAX_CHARS);

			/*Call function to search through map and list out the content matching entered pattern*/
			search_by_title(search_title, global_map_ptr);
			memset(search_title,'\0',MAX_CHARS);

		}

		/*dereference the JSON*/
		json_decref(root);
	}

	//At end of use free the buffer
	free(buffer);

	sprintf(log_buffer, "\n%s:%s:%d: Calling cleanup function", __FILE__, __FUNCTION__, __LINE__);
	global_write_log(log_buffer, 0);
	memset(log_buffer,'\0',MAX_CHARS);

	/*call the global clean up function */
	global_cleanup_function();
	
	return 0;
}
