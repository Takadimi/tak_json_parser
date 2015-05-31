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
	PARSING_OBJECT,
	PARSING_KEY_NAME,
	PARSING_VALUE
} PARSING_STATE;

typedef struct
{
	// PARSING_STATE state;
	int json_str_index;
	char curr_key_name[1024];
	char curr_str_value[1024];
	int curr_int_value;
} Parser;

void print_parser(Parser* parser)
{
	printf("Parser:\n");
	// printf("state: %d\n", parser->state);
	printf("json_str_index: %d\n", parser->json_str_index);
	printf("curr_key_name: %s\n", parser->curr_key_name);
	printf("curr_str_value: %s\n", parser->curr_str_value);
	printf("curr_int_value: %d\n", parser->curr_int_value);
}

void parse_JSON_key_name(const char* json_str, const long str_length, Parser* parser)
{
	// parser->state = PARSING_KEY_NAME;
	printf("KEY NAME: ");
	parser->json_str_index++;

	while (parser->json_str_index < str_length && json_str[parser->json_str_index] != '\"')
	{
		printf("%c", json_str[parser->json_str_index]);
		parser->json_str_index++;
	}

	// parser->state = PARSING_OBJECT;
}

void parse_JSON_value(const char* json_str, const long str_length, Parser* parser)
{
	// parser->state = PARSING_VALUE;
	printf("VALUE: ");
	parser->json_str_index++;

	while (parser->json_str_index < str_length && json_str[parser->json_str_index] != ',')
	{
		printf("%c", json_str[parser->json_str_index]);
		parser->json_str_index++;
	}

	// parser->state = PARSING_OBJECT;
}

void parse_JSON_object(const char* json_str, const long str_length, Parser* parser)
{
	// parser->state = PARSING_OBJECT;

	while (parser->json_str_index < str_length && json_str[parser->json_str_index] != '}')
	{
		char c = json_str[parser->json_str_index];
		switch (c)
		{
			case '\"':
				// if (parser->state == PARSING_OBJECT)
				// {
				parse_JSON_key_name(json_str, str_length, parser);
				printf("\n");
				// }
				break;	

			case ':':
				parse_JSON_value(json_str, str_length, parser);
				printf("\n");
				break;

			default:
				break;
		}

		parser->json_str_index++;
	}
}

void parse_JSON(const char* json_str, const long str_length, Parser* parser)
{
	while (parser->json_str_index < str_length)
	{
		switch (json_str[parser->json_str_index])	
		{
			case '{': 
				parse_JSON_object(json_str, str_length, parser);	
				break;
			default:
				break;
		}

		parser->json_str_index++;
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

	parse_JSON(buffer, length, &parser);

	delete_parser(&parser);

	return 0;
}
