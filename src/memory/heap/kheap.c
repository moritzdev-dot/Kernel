#include "kheap.h"
#include "heap.h"
#include "../../config.h"
#include "../../kernel.h"
#include "../memory.h"

struct heap kernel_heap;
struct heap_table table;


void* kmalloc(size_t size) {
  return heap_malloc(&kernel_heap, size);
}
void kfree(void* ptr) {
  heap_free(&kernel_heap, ptr);
}

void kheap_init() {
  int num_entrys = HEAP_SIZE / HEAP_BLOCK_SIZE;
  table.entries = (byte*)HEAP_TABLE_ADDR;
  table.total = num_entrys;

  void* end = (void*)(HEAP_ADDR + HEAP_SIZE);
  int res = heap_create(&kernel_heap, (void*)(HEAP_ADDR), end, &table);
  if(res < 0) {
    print("COULDNT CREATE HEAP\n");
  }

}

void* kzalloc(size_t size) {
  void* ptr = kmalloc(size);
  if(!ptr) {
    return 0;
  }
  memset(ptr, 0, size);
  return ptr;
}
