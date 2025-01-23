#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define META_SIZE sizeof(struct block_meta)

// head of the linked list
void *global_base = NULL;

typedef struct block_meta MemoryBlock;

struct block_meta {
  int magic;
  uint8_t is_free;
  uint16_t size;
  MemoryBlock *next;
};

MemoryBlock *find_free_block(struct block_meta **last, uint16_t size) {
  MemoryBlock *current = global_base;

  // checking if there's a free block available
  while (current && !(current->is_free && current->size >= size)) {
    *last = current;
    current = current->next;
  }

  return current;
}

MemoryBlock *request_space(struct block_meta *last, uint16_t size) {
  MemoryBlock *block;

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

MemoryBlock *get_block_ptr(void *ptr) {
  return (MemoryBlock *) ptr - 1;
}

void fb_free(void *ptr) {
  if (!ptr) {
    return;
  }

  // Get the block from given pointer
  MemoryBlock *block_ptr = get_block_ptr(ptr);

  if (!block_ptr) {
    return;
  }

  // Set block to free
  block_ptr->is_free = 1;
  block_ptr->magic = 0x55555555;
}

void *fb_alloc(uint16_t bytes)
{
    uint16_t total_size;
    MemoryBlock *block;

    if (!bytes) {
        return NULL;
    }

    // Check whether linked list is empty or not
    if (!global_base) {
      block = request_space(NULL, bytes);

      if (!block) {
        return NULL;
      }

      global_base = block;
    } else {
      // Get last value from linked list
      MemoryBlock *lastNode = global_base;

      // Check whether there's a free block to be allocated
      block = find_free_block(&lastNode, bytes);

      // If dont, requests some memory
      if (!block) {
        block = request_space(lastNode, bytes);

        if (!block)
            return NULL;
      }

      // If yes, set the block to the "not free" condition
      block->is_free = 0;
      block->magic = 0x77777777;
    }

    return (block+1);
}

void *fb_realloc(void *ptr, uint16_t bytes) {
  MemoryBlock *block;

  // If ptr is null, just allocate memory and return
  if (!ptr) {
    return fb_alloc(bytes);
  }

  // Get the block pointed by ptr
  MemoryBlock *block_ptr = get_block_ptr(ptr);

  // If the amount of bytes that I want to allocate
  // is less than or equal than the space already allocated
  // I simply return the ptr
  if (bytes <= block_ptr->size)
    return ptr;

  // If not, I'll allocate the given bytes
  block = fb_alloc(bytes);

  // Copy all the content from the old location to the new one
  memcpy(block, ptr, block_ptr->size);

  // Free the old memory
  fb_free(ptr);

  return block;
}

void *fb_calloc(uint16_t nelem, uint16_t elsize) {
  if (!nelem || !elsize)
    return NULL;

  MemoryBlock *block;
  uint16_t size = nelem * elsize;

  // Allocate some memory
  block = fb_alloc(size);

  if (!block)
    return NULL;

  // Initialize all bytes with zero
  memset(block, 0, size);

  return block;
}

