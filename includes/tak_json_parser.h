#pragma once

#define MAX_ITEM_COUNT 1024
#define MAX_OBJECT_COUNT 1024

typedef enum
{
    ARR_NO_TYPE,
    ARR_STRING,
    ARR_INTEGER
} JSON_ARRAY_TYPE;

typedef struct Parser
{
    char* json_str;
    long str_length;
} Parser;

typedef struct JSON_Item
{
    char* name;
    char* string_value;
    int int_value;
    float float_value;
    JSON_Item* next;
    JSON_Item* prev;
} JSON_Item;

typedef struct JSON_Obj
{
    char* name;
    JSON_Item* items[MAX_ITEM_COUNT];
    int item_count;
    JSON_Obj* objects[MAX_OBJECT_COUNT];
    int object_count;
} JSON_Obj;

void skip(Parser* parser);
JSON_Item* init_json_item();
JSON_Obj* init_json_obj();
JSON_ARRAY_TYPE peek_array_value_type(Parser* parser);
int peek_str_length(Parser* parser);
void parse_name(Parser* parser, JSON_Item* item);
void parse_value(Parser* parser, JSON_Item* item);
void parse_object(Parser* parser, JSON_Obj* obj);
JSON_Obj* parse_JSON(char* json_file_str, const long file_length);
void delete_parser(Parser* parser);
