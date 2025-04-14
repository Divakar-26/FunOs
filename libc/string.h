#ifndef STRINGS_H
#define STRINGS_H

#include"../cpu/types.h"
#include <stddef.h> // For NULL

void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
int strcmp(char s1[], const char s2[]);
char *my_strchr(const char *str, char c);
void u32_to_str(u32 value, char* str);
void remove_leading_spaces(char *str);
int string_to_int(char * str);


#endif