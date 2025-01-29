#include "string.h"
int strlen(const char* str) {
  int i = 0;
  for(;str[i] != '\0'; i++){
  }
  return i;
}

int strnlen(const char* str, int max) {
  int i = 0;
  for(;str[i] != '\0' && i < max; i++){
  }
  return i;
}

int is_digit(char ch) {
  return '0' <= ch && ch <= '9';
}

int char_to_int(char ch) {
  return ch - '0';
}
