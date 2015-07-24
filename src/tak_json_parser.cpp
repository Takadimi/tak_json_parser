#include "tak_json_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// TODO: Write basic Linear Memory Allocator?
/* It should just be a matter of allocating a block
*  of memory up front and reallocating that block
*  whenever we run into a point where we need more
*  memory. Hopefully, that's all there is to it... :/
*/

void skip(Parser* parser)
{
	while ((unsigned char) *(parser->json_str) <= 32)
	{
		parser->json_str++;
	}
}

JSON_Item* init_json_item()
{
	JSON_Item* new_item = (JSON_Item*) calloc(1, sizeof(JSON_Item));
	if (new_item == NULL)
	{
		printf("Error: Can't allocate new memory for JSON Item\n");
	}

	return new_item;
}

JSON_Obj* init_json_obj()
{
	JSON_Obj* new_obj = (JSON_Obj*) calloc(1, sizeof(JSON_Obj));
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
	int str_length = 0;

	while (*(parser->json_str) != '\"')			
	{
		str_length++;
		parser->json_str++;
	}

	parser->json_str -= str_length;

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

	if (*(parser.json_str) == '}')
	{
		printf("Base JSON Object is empty!\n");
		return NULL;
	}
	
	JSON_Obj* base = init_json_obj();

	// TODO: test code for parsing objects/items
	//       should be factored out into functions
	for ( ;*(parser.json_str) && *(parser.json_str) != '}'; parser.json_str++)
	{
		skip(&parser);
		if (*(parser.json_str) != '\"')
		{
			break;
		}

		// Parse item
		base->items[base->item_count] = init_json_item();
		parse_name(&parser, base->items[base->item_count]);
		parse_value(&parser, base->items[base->item_count]);
		base->item_count++;
		// TODO: Change to using linked list for items, instead of fixed array
	}

	return base;
}

void delete_parser(Parser* parser)
{
	//TODO(ethan): Free all dynamically allocated memory for the parser here	
}
