#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// void *memset(void *s, int c, size_t n);
// void *memcpy(void *dest, const void *src, size_t n);
// int memcmp(const void *s1, const void *s2, size_t n);
// size_t strlen(const char *s);
// char *strcpy(char *dest, const char *src);
// char *strcat(char *dest, const char *src);
// int strcmp(const char *s1, const char *s2);

void u_numberToStr(unsigned long long num, int base, bool is_negative, char *buffer);
void intToStr(int N, char *buffer);
void addrToStr(uint64_t num, char *buf) ;
int strlen(const char *str) ;
void strcpy(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, int n);


#endif 