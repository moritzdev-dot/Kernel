#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>

#define PAGING_CHACHE_DISABLES  0b00010000
#define PAGING_WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100
#define PAGING_IS_WRITABLE      0b00000010
#define PAGING_IS_PRESENT       0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096


struct paging_4gb_chunk* new_4gb(uint8_t flags);

struct paging_4gb_chunk {
    uint32_t* directory_entry;
};

uint32_t* paging_4gb_chunk_to_dir(struct paging_4gb_chunk* chunk);

void paging_switch(uint32_t* directory);

extern void enable_paging();

int paging_set(uint32_t* directory, void* virt_addr, uint32_t val);

#endif
