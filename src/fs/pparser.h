#ifndef PATHPARSER_H
#define PATHPARSER_H

struct path_root {
    int drive_no;
    struct path_part* first;
};

struct path_part {
    const char* part;
    struct path_part* next;
};

void free_path(struct path_root* root);

struct path_root* parse_path(const char* path, const char* cur_dir_path);

#endif
