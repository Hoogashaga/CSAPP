// Written by Porter Jones (pbjones@cs.washington.edu)
//
// This is a file for managing a store of various aisles,
// represented by an array of 64-bit integers

#include <stddef.h>  // To be able to use NULL
#include "aisle_manager.h"
#include "store_client.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];


/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Priotizes and fills sections with lower addresses
 * first. Sections with lower addresses should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom() {
  // TODO: implement this function
  for (int i=0; i<10; i++) {
    for (int j=0; j<4; j++) {
      int empty = 10 - num_items(&aisles[i], j);
      unsigned short id = get_id(&aisles[i],j);
      add_items(&aisles[i], j, stockroom[id]);
      if (stockroom[id]<= empty) {
        stockroom[id] = 0;
      } else {
	  stockroom[id]-= empty;    
        }    
    }	    
  }
}

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short id, int num) {
  // TODO: implement this function
  int removed = 0;
  int left = 0;
  for (int i=0; i<10; i++) {
    if (removed < num) {
      for (int j=0; j<4; j++) {
        if (removed < num) {
	  if (get_id(&aisles[i], j) == id) {
	    int num_section = (int) num_items(&aisles[i], j);
	    if ((num - removed) < num_section) {
	      remove_items(&aisles[i], j, num - removed);
              removed = num;
	    } else {
		remove_items(&aisles[i], j, num);    
		removed += num_section;;
	      }
	  }
	  left = num - removed;
	}
      }
    }
  }

  if (left > 0) {
    if (stockroom[id] >= left) {
      stockroom[id] -= left;
      removed = num;
    } else {
	removed += stockroom[id];
        stockroom[id] = 0;
      }
  }

  return removed;
}

/* Return a pointer to the first section in the aisles with the given item id
 * that has no items in it or NULL if no such section exists. Only consider
 * items stored in sections in the aisles (i.e., ignore anything in the
 * stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* empty_section_with_id(unsigned short id) {
  // TODO: implement this function
  unsigned short* res;
  for (int i=0; i<10; i++) {
    for (int j=0; j<4; j++) {
      if ((get_id(&aisles[i], j) == id) && (num_items(&aisles[i], j)==0)) {
        res = (unsigned short*) (&aisles[i]);
	res = (unsigned short*) (&res[j]);
        return res;	
      }
    }
  }


  return NULL;
}

/* Return a pointer to the section with the most items in the store. Only
 * consider items stored in sections in the aisles (i.e., ignore anything in
 * the stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* section_with_most_items() {
  // TODO: implement this function
  unsigned short* res = (unsigned short*) aisles;
  unsigned short max = 0;
  for (int i=0; i<10; i++) {
    for (int j=0; j<4; j++) {
      unsigned short temp = num_items(&aisles[i], j);
      if (temp > max) {
        res = (unsigned short*) (&aisles[i]);
	res = (unsigned short*) (&res[j]);
	max = temp;
      } 	      
    }
  }
  return res;
}
