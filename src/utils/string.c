#include "string.h"
#include "math.h"

/*
    Warning:
    I made the following function using AI :(
*/

// void numberToStr(int num, int base, int is_signed, char *buffer) {
//     if (base < 2 || base > 36) {
//         buffer[0] = '\0';
//         return;
//     }

//     int i = 0;
//     int isNegative = 0;
//     unsigned int n;

//     // For 32-bit OS, int is 32 bits. Handle INT_MIN carefully.
//     if (is_signed && num < 0) {
//         isNegative = 1;
//         n = (unsigned int)(-(num + 1)) + 1; // Avoids overflow for INT_MIN
//     } else {
//         n = (unsigned int)num;
//     }

//     if (n == 0) {
//         buffer[i++] = '0';
//     } else {
//         while (n > 0 && i < 32 - 1) { // 32 is a safe buffer size for 32-bit ints
//             int digit = n % base;
//             buffer[i++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
//             n /= base;
//         }
//     }

//     if (isNegative && i < 32 - 1) {
//         buffer[i++] = '-';
//     }

//     buffer[i] = '\0';

//     // Reverse the string
//     for (int j = 0, k = i - 1; j < k; j++, k--) {
//         char temp = buffer[j];
//         buffer[j] = buffer[k];
//         buffer[k] = temp;
//     }
// }

void u_numberToStr(unsigned long long num, int base, bool is_negative, char *buffer){
    if (base < 2 || base > 36) {
        buffer[0] = '\0';
        return;
    }

    int i = 0;
    unsigned long long n = num;

    if (n == 0) {
        buffer[i++] = '0';
    } else {
        while (n > 0 && i < 32 - 1) { // 32 is a safe buffer size for 64-bit ints
            int digit = umod64(n, base);
            buffer[i++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
            n = udiv64(n, base);
        }
    }

    if (is_negative && i < 32 - 1) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Reverse the string
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }
}


void intToStr(int N, char *buff){
    u_numberToStr(N, 10, 1, buff);
}



void addrToStr(uint64_t num, char *buf) {
    for (int i = 15; i >= 0; i--) {
        buf[i] = "0123456789ABCDEF"[num & 0xF];
        num >>= 4;
    }
    buf[16] = '\0';
}



int strlen(const char *str) {
    int len = 0;
    while (*str++) len++;
    return len;
}


void strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}


/*
if,
    str1 == str2, return 0
    str1 < str2, return negative value
    str1 > str2, return positive value
*/
int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}   


/*
if,
    str1 == str2, return 0
    str1 < str2, return negative value
    str1 > str2, return positive value
*/
int strncmp(const char *str1, const char *str2, int n) {
    while (n && *str1 && (*str1 == *str2)) {
        str1++;
        str2++;
        n--;
    }
    if (n == 0) return 0;
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}