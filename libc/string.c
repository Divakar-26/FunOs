#include "string.h"

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

/* K&R */
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* K&R */
int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

/* K&R 
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int strcmp(char s1[], const char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void u32_to_str(u32 value, char* str) {
    char buffer[11];  // Max for 32-bit unsigned int = 4294967295 (10 digits) + null
    int i = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    // Reverse the string into `str`
    for (int j = 0; j < i; j++) {
        str[j] = buffer[i - j - 1];
    }

    str[i] = '\0';
}



void remove_leading_spaces(char *str) {
    // Move the pointer to the first non-space character
    while (*str == ' ') {
        str++;
    }

    // Shift the characters to the left, starting from the first non-space character
    char *start = str;
    while (*start != '\0') {
        *str++ = *start++;
    }

    // Null-terminate the string
    *str = '\0';
}



char *my_strchr(const char *str, char c) {
    // Iterate through each character in the string
    while (*str != '\0') {
        if (*str == c) {
            return (char *)str;  // Return a pointer to the first occurrence of c
        }
        str++;  // Move to the next character
    }
    
    // If character was not found, return NULL
    return NULL;
}
