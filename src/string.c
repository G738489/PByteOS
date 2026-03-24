#include "string.h"

int starts_with(const char* buffer, const char* prefix) {
    while(*prefix) { 
        if(*prefix != *buffer) return 0; 
        prefix++;
        buffer++;
    }
    return 1;
}



int str_compare(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s1[i] == s2[i]) i++;
    return (s1[i] == s2[i]);
}