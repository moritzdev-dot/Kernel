#include "paging.h"
#include "../heap/kheap.h"
#include "../../status.h"

extern void paging_load_directory(uint32_t* directory);

static uint32_t* current_dir = 0;

struct paging_4gb_chunk* new_4gb(uint8_t flags) {
  uint32_t* directory = (uint32_t*) kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
  int offset = 0;
  for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) {
    uint32_t* page_table_entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    for(int j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; j++) {
      page_table_entry[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
    }
    offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
    directory[i] = (uint32_t)page_table_entry | flags | PAGING_IS_WRITABLE;
  }

  struct paging_4gb_chunk* chunk = kzalloc(sizeof(struct paging_4gb_chunk));
  chunk->directory_entry = directory;
  return chunk;
}

void paging_switch(uint32_t* directory) {
  paging_load_directory(directory);
  current_dir = directory;
}


uint32_t* paging_4gb_chunk_to_dir(struct paging_4gb_chunk* chunk) {
  return chunk->directory_entry;
}

int paging_is_aligned(void* addr) {
  return ((uint32_t) addr % PAGING_PAGE_SIZE == 0);
}

int paging_get_idxs(void* virt_addr, uint32_t* dir_idx, uint32_t* table_idx) {
  if(!paging_is_aligned(virt_addr)) {
    return - INVALID_ARG;
  }

  *dir_idx = ((uint32_t) virt_addr / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
  *table_idx = ((uint32_t) virt_addr % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE)/ PAGING_PAGE_SIZE);
  return 0;
}

int paging_set(uint32_t* directory, void* virt_addr, uint32_t val) {
  if(!paging_is_aligned(virt_addr)) {
    return - INVALID_ARG;
  }
  uint32_t dir_idx = 0;
  uint32_t table_idx = 0;

  int err = paging_get_idxs(virt_addr, &dir_idx, &table_idx);
  if(err < 0) {
    return err;
  }
  uint32_t entry = directory[dir_idx];
  uint32_t* table = (uint32_t*)(entry & 0xfffff000);
  table[table_idx] = val;

  return 0;
}
