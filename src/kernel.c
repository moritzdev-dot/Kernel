#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "memory/disk/disk.h"
#include "string/string.h"
#include "fs/pparser.h"


char WHITE = 15;

uint16_t row = 0;
uint16_t col = 0;

uint16_t* video_mem;

static struct paging_4gb_chunk* kernel_chunk;

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
  kheap_init();

  
  disk_search_and_init();

  idt_init();

  kernel_chunk = new_4gb(
            PAGING_IS_WRITABLE 
          | PAGING_IS_PRESENT 
          | PAGING_ACCESS_FROM_ALL
  );
  paging_switch(paging_4gb_chunk_to_dir(kernel_chunk));

  enable_paging();

  enable_int();
}

