#include "idt.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../kernel.h"
#include "../io/io.h"

struct idt_desc idt_descriptors[TOTAL_INTERUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_int();

void no_int_handler() {
  outb(0x20, 0x20);
}

void int21_handler() {
  print("keyboard pressed");
  outb(0x20, 0x20);
}

void idt_set(int i, void* addr) {
  struct idt_desc* desc = &idt_descriptors[i];
  desc->offset_1 = (uint32_t)addr & 0xFFFF;
  desc->selector = KERNEL_CODE_SEL;
  desc->zero = 0;
  desc->type_attr = 0xEE;

  desc->offset_2 = (uint32_t)addr >> 16;

}

void idt_init() {
  memset(idt_descriptors, 0, TOTAL_INTERUPTS);
  idtr_descriptor.limit = sizeof(idt_descriptors) -1;
  idtr_descriptor.base = (uint32_t)idt_descriptors;
  for(int i = 0; i < TOTAL_INTERUPTS; i++) {
    idt_set(i, no_int);
  }
  idt_set(0x21, int21h);
  idt_load(&idtr_descriptor);
}
