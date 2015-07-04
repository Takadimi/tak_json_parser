#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tak_json_parser.h"

#define FILE_READ_ERR -1

long get_file_length(const char* file_path)
{
	FILE* fp = fopen(file_path, "rb");

	if (fp == NULL)
	{
		printf("File @ %s could not open correctly\n", file_path);
		return FILE_READ_ERR;
	}

	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fclose(fp);

	return length;
}

int read_json_file_into_string(const char* file_path, const long length, char* result)
{
	FILE* fp = fopen(file_path, "rb");

	if (fp == NULL)
	{
		printf("File @ %s could not open correctly\n", file_path);
		return FILE_READ_ERR;
	}

	fread(result, 1, length, fp);
	fclose(fp);

	result[length] = '\0';

	return 0;
}

typedef enum
{
	PRIMITIVE,
	ARRAY,
	STRING
} JSON_TYPE;

typedef enum
{
	ARR_INTEGER,
	ARR_FLOAT,
	ARR_BOOLEAN,
	ARR_STRING,
	ARR_OBJECT,
	ARR_NO_TYPE
} JSON_ARRAY_TYPE;

typedef enum
{
	PARSING_OBJECT,
	PARSING_KEY_NAME,
	PARSING_VALUE
} PARSING_STATE;

typedef struct Parser
{
	// PARSING_STATE state;
	char* json_str;
	long str_length;
	char curr_key_name[1024];
	char curr_str_value[1024];
	int curr_int_value;
	float curr_float_value;
} Parser;

typedef struct JSON_Obj
{
	JSON_Obj* child;
	JSON_Obj* next;
	char* name;
	char* string_value;
	int int_value;
	float float_value;
} JSON_Obj;

void print_parser(Parser* parser)
{
	printf("Parser:\n");
	// printf("state: %d\n", parser->state);
	printf("curr_key_name: %s\n", parser->curr_key_name);
	printf("curr_str_value: %s\n", parser->curr_str_value);
	printf("curr_int_value: %d\n", parser->curr_int_value);
}

void skip(Parser* parser)
{
	while ((unsigned char) *(parser->json_str) <= 32)
	{
		parser->json_str++;
	}
}

JSON_Obj* init_json_obj()
{
	JSON_Obj* new_obj = (JSON_Obj*) malloc(sizeof(JSON_Obj));
	if (new_obj == NULL)
	{
		printf("Error: Can't allocate new memory for JSON Object\n");
	}

	return new_obj;
}

JSON_ARRAY_TYPE peek_array_value_type(Parser* parser)
{
	JSON_ARRAY_TYPE array_value_type = ARR_NO_TYPE;
	char* peek_str = parser->json_str + 1;

	while (*peek_str && *peek_str != ',')
	{
		switch (*peek_str)
		{
			case '\"':
				array_value_type = ARR_STRING;
				break;
			case '1': case '2': case '3': case '4': case '5': 
			case '6': case '7': case '8': case '9': case '0':
				array_value_type = ARR_INTEGER;
				break;
		}

		peek_str++;	
	}

	return array_value_type;
}

int peek_str_length(Parser* parser)
{
	int peek_counter = 0;
	int str_length = 0;

	while (*(parser->json_str) != '\"')			
	{
		str_length++;
		parser->json_str++;
	}

	peek_counter = str_length;

	while (peek_counter > 0)
	{
		parser->json_str--;
		peek_counter--;
	}

	return str_length;
}

void parse_name(Parser* parser, JSON_Obj* item)
{
	skip(parser);
	
	if (*(parser->json_str) != '\"')
	{
		printf("Did not skip to first item name quotation!\n");
	}

	parser->json_str++; // Point to first character in name string

	int name_length = peek_str_length(parser);
	item->name = (char*) malloc((name_length + 1) * sizeof(char));

	int i = 0;
	while (*(parser->json_str) != '\"')
	{
		item->name[i] = *(parser->json_str);
		parser->json_str++;
		i++;
	}
	item->name[i] = '\0';
}

void parse_value(Parser* parser, JSON_Obj* item)
{

}

void parse_item(Parser* parser, JSON_Obj* item)
{
	parse_name(parser, item);

	parser->json_str++;
	skip(parser);
	if (*(parser->json_str) != ':')
	{
		printf("Improperly separated name and value of item.\n");
	}

	parse_value(parser, item);
}

void parse_object(Parser* parser, JSON_Obj* obj)
{
	JSON_Obj* curr_obj = obj->child = init_json_obj();

	skip(parser);

	parse_item(parser, curr_obj);	

	// while (*(parser->json_str) && *(parser->json_str) != '}')
	// {
	// 	char c = *(parser->json_str);
	// 	printf("%c", c);

	// 	if (c != '\"')
	// 	{
	// 		// printf("Malformed JSON\n");
	// 	}

	// 	parser->json_str++;	
	// }
}

JSON_Obj* start_Parse(Parser* parser)
{
	JSON_Obj* base;
	skip(parser);

	if (*(parser->json_str) != '{')
	{
		printf("Error in beginning of parsing\n");
	}

	parser->json_str++;

	base = init_json_obj();
	parse_object(parser, base);

	return base;
}

void delete_parser(Parser* parser)
{
	//TODO(ethan): Free all dynamically allocated memory for the parser here	
}

int main()
{
	const char* file_path = "/Users/ethanwoodward/Projects/tak_json_parser/example.json";

	long length = get_file_length(file_path);
	if (length == -1)
	{
		exit(1);
	}

	char* buffer = (char*) malloc(length);

	int ret_val = read_json_file_into_string(file_path, length, buffer);
	if (ret_val == -1)
	{
		exit(1);
	}

	Parser parser = {};

	parser.json_str = (char*) malloc(length);
	strcpy(parser.json_str, buffer);
	parser.str_length = length;

	JSON_Obj* base_obj = start_Parse(&parser);
	
	// Just here to keep compiler from erroring on unused variables
	printf("*****************************************\n");
	printf("base_obj memory location: %p\n", base_obj);

	return 0;
}
