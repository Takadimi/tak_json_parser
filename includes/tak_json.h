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
} JSON_Item;

typedef struct JSON_Obj
{
    char* name;
    JSON_Item* items[MAX_ITEM_COUNT];
    int item_count;
    JSON_Obj* objects[MAX_OBJECT_COUNT];
    int object_count;
} JSON_Obj;
