#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct
{
	// PARSING_STATE state;
	char* json_str;
	long str_length;
	char curr_key_name[1024];
	char curr_str_value[1024];
	int curr_int_value;
	float curr_float_value;
} Parser;

void print_parser(Parser* parser)
{
	printf("Parser:\n");
	// printf("state: %d\n", parser->state);
	printf("curr_key_name: %s\n", parser->curr_key_name);
	printf("curr_str_value: %s\n", parser->curr_str_value);
	printf("curr_int_value: %d\n", parser->curr_int_value);
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

void parse_JSON_array(Parser* parser)
{
	int array_value_type = peek_array_value_type(parser);

	while (*(parser->json_str) && *(parser->json_str) != ']')
	{
		printf("%c", *(parser->json_str));

		parser->json_str++;
	}
}

void parse_JSON_key_name(Parser* parser)
{
	// parser->state = PARSING_KEY_NAME;
	printf("KEY NAME: ");
	parser->json_str++;

	while (*(parser->json_str) && *(parser->json_str) != '\"')
	{
		printf("%c", *(parser->json_str));
		parser->json_str++;
	}

	// parser->state = PARSING_OBJECT;
}

void parse_JSON_value(Parser* parser)
{
	// parser->state = PARSING_VALUE;
	printf("VALUE: ");
	parser->json_str++;

	while (*(parser->json_str) != ',' && *(parser->json_str) != '}')
	{
		if (*(parser->json_str) == '[')
		{
			parse_JSON_array(parser);
		}

		printf("%c", *(parser->json_str));
		parser->json_str++;
	}

	// parser->state = PARSING_OBJECT;
}

void parse_JSON(Parser* parser)
{
	// parser->state = PARSING_OBJECT;

	while (*(parser->json_str) && *(parser->json_str) != '}')
	{
		char c = *(parser->json_str);
		switch (c)
		{
			case '\"':
				// if (parser->state == PARSING_OBJECT)
				// {
				parse_JSON_key_name(parser);
				printf("\n");
				// }
				break;	

			case ':':
				parse_JSON_value(parser);
				printf("\n");
				break;

			default:
				break;
		}

		parser->json_str++;	
	}
}

void delete_parser(Parser* parser)
{
	//TODO(ethan): Free all dynamically allocated memory for the parser here	
}

int main()
{
	const char* file_path = "example.json";

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

	parse_JSON(&parser);

	return 0;
}
