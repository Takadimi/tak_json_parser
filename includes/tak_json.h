#pragma once

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
    JSON_Item* items;
    char* name;
} JSON_Obj;
