#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tak_json_parser.h"
#include "tak_json.h"

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

void skip(Parser* parser)
{
	while ((unsigned char) *(parser->json_str) <= 32)
	{
		parser->json_str++;
	}
}

JSON_Item* init_json_item()
{
	JSON_Item* new_item = (JSON_Item*) malloc(sizeof(JSON_Item));
	if (new_item == NULL)
	{
		printf("Error: Can't allocate new memory for JSON Item\n");
	}

	return new_item;
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

void parse_name(Parser* parser, JSON_Item* item)
{
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
	parser->json_str++;
}

void parse_value(Parser* parser, JSON_Item* item)
{
	assert(*(parser->json_str) == ':');

	parser->json_str++; // Point to first character after :
	skip(parser);

	while (*(parser->json_str) != ',')
	{
		parser->json_str++;
	}

	parser->json_str++;
}

void parse_item(Parser* parser, JSON_Obj* item)
{
	// parse_name(parser, item);

	// parser->json_str++;
	// skip(parser);
	// if (*(parser->json_str) != ':')
	// {
	// 	printf("Improperly separated name and value of item.\n");
	// }

	// parse_value(parser, item);
}

void parse_object(Parser* parser, JSON_Obj* obj)
{
	// JSON_Obj* curr_obj = obj->child = init_json_obj();
	// JSON_Item* curr_item = init_json_item();
	// skip(parser);

	//parse_item(parser, curr_obj);	
}

JSON_Obj* parse_JSON(char* json_file_str, const long file_length)
{
	Parser parser = {};
	parser.json_str = json_file_str;
	parser.str_length = file_length;

	skip(&parser);

	if (*(parser.json_str) != '{')
	{
		printf("Error: Invalid json. File must begin with \'{\'\n");
	}

	parser.json_str++;
	skip(&parser);	

	JSON_Obj* base = init_json_obj();

	// TODO: test code for parsing objects/items
	//       should be factored out into functions
	for (;*(parser.json_str) && *(parser.json_str) != '}'; parser.json_str++)
	{
		skip(&parser);
		if (*(parser.json_str) != '\"')
		{
			break;
		}

		// Parse item
		base->items = init_json_item(); 
		parse_name(&parser, base->items);
		printf("Item name: %s\n", base->items->name);
		parse_value(&parser, base->items);
		base->items++;
	}

	base->items--; // TODO: Get rid of this, and find way to reset items pointer

	return base;
}

void delete_parser(Parser* parser)
{
	//TODO(ethan): Free all dynamically allocated memory for the parser here	
}

int main()
{
	const char* file_path = "../example.json";

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

	JSON_Obj* base_obj = parse_JSON(buffer, length);
	printf("Name of first item: %s\n", base_obj->items->name);
	
	// Just here to keep compiler from erroring on unused variables
	printf("*****************************************\n");
	printf("base_obj memory location: %p\n", base_obj);

	return 0;
}
