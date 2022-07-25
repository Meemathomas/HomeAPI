#ifndef __HOME_API_H
#define __HOME_API_H

#include "jansson.h"
#include "curl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*Configs*/
#define MAX_KEY_CHARS 4096
#define MAX_CHARS 4096
#define MAX_SUBTREES_LIMIT 50

//Config 
#define LOG_LVL 1 /*0=INFO, 1=DEBUG*/

typedef struct map{
	char key[MAX_CHARS];
	char value[MAX_CHARS];
	struct map *link;
}MAP;


/*Function signatures*/
void convert_string_to_upper_case(const char *pSourceString, char *pDestinationString);
void search_by_title(const char *pSearchString, MAP *pmap_ptr);
void global_update_map(const char *pValue, MAP *pmap_ptr);
void global_print_map(MAP *pmap_ptr);
void global_update_keys_array();
void parse_json(json_t *pRoot, const char *pSearchString);
void parse_json_path(json_t *pElement, const char *pSearchString);
const char *json_plural(size_t count);
void parse_json_object(json_t *pElement, const char *pSearchString);
void parse_json_array(json_t *pElement, const char *pSearchString);
void parse_json_string(json_t *pElement, const char *pSearchString);
void parse_json_integer(json_t *pElement);
void parse_json_real(json_t *pElement);
void parse_json_true(json_t *pElement);
void parse_json_false(json_t *pElement);
void parse_json_null(json_t *pElement);

#endif/*__HOME_API_H*/

