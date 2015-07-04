#!/bin/bash

g++ -Wall -Werror -pedantic -I../includes/ ../src/tak_json.cpp ../src/tak_json_parser.cpp -o ../bin/tak_json
