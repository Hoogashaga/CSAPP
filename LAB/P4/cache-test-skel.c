/*
CSE 351 Lab 4, Part 1 - Mystery Caches
SHENG-HAO CHEN
shchen77
*/

#include <stdlib.h>
#include <stdio.h>

#include "support/mystery-cache.h"

/*
 * NOTES: 
 * 1. When using access_cache() you do not need to provide a "real" memory
 * addresses. You can use any convenient integer value as a memory address,
 * you should not be able to cause a segmentation fault by providing a memory
 * address out of your programs address space as the argument to access_cache.
 * 
 * 2. Do NOT change the provided main function, especially the print statement.
 * If you do so, the autograder may fail to grade your code even if it produces
 * the correct result.
 */

/*
   Returns the size (in B) of each block in the cache.
*/
int get_block_size(void) {
  /* YOUR CODE GOES HERE */
  flush_cache();
  int count;
  access_cache(0);
  while(access_cache(count)) {
    count++;
  }
  return count;
 	
}

/*
   Returns the size (in B) of the cache.
*/
int get_cache_size(int block_size) {
  /* YOUR CODE GOES HERE */
  int count = 0;
  access_cache(0);
  while(access_cache(0)) {
    count += block_size;
    flush_cache();
    for (int i=0; i < count; i+=block_size) {
      access_cache(i);
    }
    if (!access_cache(0)) break;
  }
  return count - block_size;	
}

/*
   Returns the associativity of the cache.
*/
int get_cache_assoc(int cache_size) {
  /* YOUR CODE GOES HERE */

  flush_cache();
  int associcativity = 1;
  int size;
  int address;
  while(associcativity < cache_size + 1) {
    size = cache_size / associcativity;
    for (address = 0; address < cache_size + 1; address += size) {
      access_cache(address);
    }
    if (!access_cache(0)) break;
    associcativity *=2;
  }
  return associcativity;	
}

int main(void) {
  int size;
  int assoc;
  int block_size;

  cache_init(0, 0);

  block_size = get_block_size();
  size = get_cache_size(block_size);
  assoc = get_cache_assoc(size);

  printf("Cache block size: %d bytes\n", block_size);
  printf("Cache size: %d bytes\n", size);
  printf("Cache associativity: %d\n", assoc);

  return EXIT_SUCCESS;
}
