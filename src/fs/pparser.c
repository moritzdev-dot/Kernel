#include "pparser.h"
#include "../kernel.h"
#include "../string/string.h"
#include "../memory/heap/kheap.h"
#include "../status.h"
#include "../config.h"

static int path_valid(const char* filename) {
  int len = strnlen(filename, MAX_PATH_LEN);
  return (len >= 3 && is_digit(filename[0]) && filename[1] == ':' && filename[2] == '/');
}


static int get_drive_by_path(const char** path) {
  if(!path_valid(*path)) {
    return -BAD_PATH_ERROR;
  }
  int idx = char_to_int(*path[0]);
  *path += 3;
  return idx;
}

static struct path_root* create_root(int drive_number) {
  struct path_root* root = kzalloc(sizeof(struct path_root));
  root->drive_no = drive_number;
  root->first = 0;
  return root;
}

static const char* get_path_part(const char** path) {
  char* path_part = kzalloc(MAX_PATH_LEN);
  int i = 0;
  while(**path != '/' && **path != 0) {
    path_part[i] = **path;
    *path += 1;
    i++;
  }

  if(**path == '/') {
    *path += 1;
  }

  if(i == 0) {
    kfree(path_part);
    path_part = 0;
  }

  return path_part;
}

struct path_part* parse_path_part(struct path_part* last, const char** path) {
  const char* str = get_path_part(path);
  if(!str) {
    return 0;
  }
  struct path_part* part = kzalloc(sizeof(struct path_part));
  part->part = str;
  part->next = 0;
  if(last) {
    last->next = part;
  }
  return part;
}


void free_path(struct path_root* root) {
  struct path_part* p= root->first;
  while(p) {
    struct path_part* next = p->next;
    kfree((void*) p->part);
    kfree(p);
    p = next;
  }
  kfree(root);
}

struct path_root* parse_path(const char* path, const char* cur_dir_path) {
  int res = 0;
  if(strlen(path) > MAX_PATH_LEN) {
    return 0;
  }

  const char* tmp = path;
  struct path_root* root;

  res = get_drive_by_path(&tmp);
  if(res < 0) {
    return 0;
  }
  root = create_root(res);
  if(!root) {
    return 0;
  }

  struct path_part* first = parse_path_part(NULL, &tmp);
  if(!first) {
    kfree(first);
    return 0;
  }
  root->first = first;
  struct path_part* part = parse_path_part(first, &tmp);
  while(part) {
    part = parse_path_part(first, &tmp);
  }

  return root;
}
