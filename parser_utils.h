// 
// Various functions for parsing strings
// I figure these will probably be used more than once so they get their own source file
//
// 2023, Jonathan Tainer
//

#ifndef PARSER_UTILS_H
#define PARSET_UTILS_H

int StringIsEmpty(const char* str);

int StringIsWhitespace(const char* str);

int StringStartsWith(const char* str, char flag);

#endif
