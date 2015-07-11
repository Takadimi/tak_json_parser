@echo off

cl -FC -Zi -Fetak_json /I..\includes ..\src\tak_json_parser.cpp ..\main.cpp
