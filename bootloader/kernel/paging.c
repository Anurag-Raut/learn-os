#include "memory.h"
#include "memutils.h"
#include "screen.h"
#include <stdint.h>

uint32_t *page_directory;
uint32_t *page_table;
void paging_init() {

  page_directory = (uint32_t *)pm_allocate();

  page_table = (uint32_t *)pm_allocate();
  memset(page_directory, 0, 4096);
  memset(page_table, 0, 4096);
  /*
   *mapping multiple kernel pages using identiy paging to accomodatetet 10Mb
   * kernel
   */
  for (int p = 0; p < 3; p++) {

    for (uint32_t i = 0; i < 1024; i++) {
      page_table[i] = (((p * 1024) + i) * 0x1000) |
                      0x3; // bit 0 =present =1, bit 1- write = 1
    }

    page_directory[p] = ((uint32_t)page_table) | 0x3;
    page_table = (uint32_t *)pm_allocate();
  }
}

void paging_enable() {
  asm volatile("mov %0, %%cr3" : : "r"(page_directory) : "memory");

  uint32_t cr0;

  asm volatile("mov %%cr0, %0" : "=r"(cr0));

  cr0 |= 0x80000000;

  asm volatile("mov %0, %%cr0" : : "r"(cr0));
  print_string("Enabled paging...\n");
}

void map_page(uint32_t vaddr, uint32_t paddr) {
  uint32_t pd_ind = vaddr >> 22;           // high 10
  uint32_t pt_ind = (vaddr >> 12) & 0x3FF; // mid  10
  uint32_t offset = vaddr & 0xFFF;
  if (!(page_directory[pd_ind] & 1)) {
    uint32_t *new_pt = (uint32_t *)pm_allocate();
    memset(new_pt, 0, PAGE_SIZE);
    page_directory[pd_ind] = (uint32_t)new_pt | 0x03;
  }

  uint32_t pt =
      page_directory[pd_ind] & 0xFFFFF000; // removing the last 3 flag bytes
  uint32_t *pt_pointer = (uint32_t *)pt;
  pt_pointer[pt_ind] = (paddr & 0xFFFFF000) | 0x03;
}

uint32_t *unmap_page(uint32_t vaddr) {
  uint32_t pd_ind = vaddr >> 22;
  uint32_t pt_ind = vaddr >> 12 & 0x3FF;
  uint32_t offset = (vaddr & 0xFFF);
  if (!(page_directory[pd_ind] & 1)) {
    return NULL;
  }
  uint32_t pt = page_directory[pd_ind] & 0xFFFFF000;
  uint32_t *pt_pointer = (uint32_t *)pt;
  uint32_t paddr = pt_pointer[pt_ind];
  pt_pointer[pt_ind] = (0x0000);

  /*
      IMP : invalidate TLB, TLB is like a cache which stores the mapping of
     v->p, even if we update the page tbale in the memory, CPU still has access
     to the mapping in the buffer.
  */
  asm volatile("invlpg (%0)" ::"r"(vaddr) : "memory");

  return (uint32_t *)paddr;
}
