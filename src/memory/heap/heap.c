#include "heap.h"
#include "../../config.h"
#include "../../status.h"
#include "../memory.h"
#include "../../kernel.h"


static int heap_validate_table(void* start, void* end, struct heap_table* table) {
  size_t d = (size_t)(end-start);
  if(table->total * HEAP_BLOCK_SIZE!= d) {
    return -INVALID_ARG;
  }
  return OK;
}

static int heap_validate_alignment(void* ptr) {
  return ((unsigned int)ptr % HEAP_BLOCK_SIZE)==0;
}

int heap_create(struct heap* heap, void* start, void* end, struct heap_table* table) {
  int res = 0;
  if(!heap_validate_alignment(start) || !heap_validate_alignment(end)) {
    print("INVALID ARG");
    res = -INVALID_ARG;
    goto out;
  }
  memset(heap, 0, sizeof(struct heap));
  res = heap_validate_table(start, end, table);
  if(res < 0) {
    goto out;
  }
  size_t table_size = sizeof(byte) * table->total;
  memset(table->entries, HEAP_BLOCK_FREE, table_size);
  heap->start_addr = start;
  heap->table = table;

out:
  return res;
}

int find_free_idx(struct heap* heap, uint32_t total_blocks) {
  for(int i = 0; i < heap->table->total - total_blocks; i++) {
    int free = 1;
    int j = 0;
    for(; j < total_blocks; j++) {
      if((heap->table->entries[i+j] & HEAP_BLOCK_TAKEN) == 1) {
        free = 0;
        break;
      }
    }
    if(free) {
      return i;
    }
    i+=j;
  }
  return -1;
}
void set_blocks(struct heap* heap, uint32_t total_blocks, int idx) {
  heap->table->entries[idx] = HEAP_BLOCK_TAKEN | HEAP_BLOCK_IS_FIRST;
  for(int i = 1; i < total_blocks-1; i++) {
    heap->table->entries[idx+i] = HEAP_BLOCK_TAKEN | HEAP_BLOCK_HAS_NEXT;
  }
  if(total_blocks > 1) {
    heap->table->entries[idx + total_blocks-1] = HEAP_BLOCK_TAKEN;
  }
}

void* heap_malloc(struct heap* heap, size_t size) {
  if(size % HEAP_BLOCK_SIZE != 0) {
    int num_blocks = size / HEAP_BLOCK_SIZE + 1;
    size = num_blocks * HEAP_BLOCK_SIZE;
  }
  uint32_t total_blocks = size / HEAP_BLOCK_SIZE;
  int idx = find_free_idx(heap, total_blocks);
  if(idx < 0) {
    print("INVALID\n");
    return 0;
  }
  set_blocks(heap, total_blocks, idx);
  void* addr = heap->start_addr + (idx * HEAP_BLOCK_SIZE);
  
  return addr;
}

void heap_free(struct heap* heap, void* ptr) {
  int idx = (ptr - heap->start_addr)/HEAP_BLOCK_SIZE;
  if(idx > heap->table->total) {
    return;
  }
  heap->table->entries[idx] = HEAP_BLOCK_FREE;
  idx++;
  while((heap->table->entries[idx] & (HEAP_BLOCK_IS_FIRST | HEAP_BLOCK_FREE)) == 0) {
    heap->table->entries[idx] = 0;
    idx++;
  }
  ptr = (void*)0;
}
