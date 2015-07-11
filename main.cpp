#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

	printf("Items after parse:\n");
	printf("==================\n\n");
	for (int i = 0; i < base_obj->item_count; i++)
	{
		printf("Item %d name: %s\n", (i + 1), base_obj->items[i]->name);
	}

	return 0;
}
