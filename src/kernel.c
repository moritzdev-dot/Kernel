#include "kernel.h"
#include <stdint.h>
#include <stddef.h>


char WHITE = 15;

uint16_t row = 0;
uint16_t col = 0;

uint16_t* video_mem;

uint16_t make_char(char c, char colour) {
   return (uint16_t)(colour << 8) + c;
}

int calculate_idx(int x, int y) {
  return y * VGA_WIDTH + x;
}

void init_terminal() {
  video_mem = (uint16_t*)(0xB8000);
  row = 0;
  col = 0;
  for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    video_mem[i] = make_char(' ', 0);
  }
}

size_t strlen(const char* str) {
  size_t i = 0;
  for(;str[i] != '\0'; i++){
  }
  return i;
}

void terminal_put_char(int x, int y, char c, char colour) {
  video_mem[calculate_idx(x, y)] = make_char(c, colour);
}

void terminal_write_char(char c, char colour) {
  if(c == '\n') {
    row += 1;
    col = 0;
    return;
  }

  terminal_put_char(col, row, c, colour);
  row += (col + 1) / VGA_WIDTH;
  col = (col + 1) % VGA_WIDTH;
}

void print(char* str) {
  size_t len = strlen(str);
  for(int i = 0; i < len; i++) {
    terminal_write_char(str[i], WHITE);
  }
}

void kernel_main() {
  init_terminal();
}

