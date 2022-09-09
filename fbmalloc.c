#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define META_SIZE sizeof(struct block_meta)

// head of the linked list
void *global_base = NULL;

struct block_meta {
  // for debbuging only
  int magic;

  // whether current block is free or not
  uint8_t is_free;

  // amount of bytes to be allocated
  uint16_t size;

  // pointer to the next block of memory
  struct block_meta *next;
};

struct block_meta *find_free_block(struct block_meta **last, uint16_t size) {
  struct block_meta *current = global_base;

  // checking if there's a free block available
  while (current && !(current->is_free && current->size >= size)) {
    *last = current;
    current = current->next;
  }

  return current;
}

struct block_meta *request_space(struct block_meta *last, uint16_t size) {
  struct block_meta *block;

  // returns a pointer to the current top of the heap
  block = sbrk(0);

  // allocate given bytes + our metadata
  void *request = sbrk(size + META_SIZE);

  if (request == (void*) -1)
    return NULL;
  

  if (last)
    // add the new block to the end of the linked list
    last->next = block;

  block->size = size;
  block->next = NULL;
  block->is_free = 0;
  block->magic = 0x12345678;

  return block;
}

void *fbmalloc(uint16_t bytes)
{
    uint16_t total_size;
    struct block_meta *block;

    if (!bytes) {
        return NULL;
    }

    // Check the head of linked list
    if (!global_base) {
      block = request_space(NULL, bytes);

      if (!block) {
        return NULL;
      }

      global_base = block;
    } else {
      struct block_meta *lastNode = global_base;
      block = find_free_block(&lastNode, bytes);

      if (!block) {
        block = request_space(lastNode, bytes);

        if (!block)
            return NULL;
      }

      block->is_free = 0;
      block->magic = 0x77777777;
    }

    return (block+1);
}
