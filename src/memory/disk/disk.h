#ifndef DISK_H
#define DISK_H


typedef unsigned int disk_type;

#define DISK_TYPE_REAL 0

struct disk {
    disk_type type;
    int sector_size;
};

struct disk* get_disk(int idx);

int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);

void disk_search_and_init();

#endif

