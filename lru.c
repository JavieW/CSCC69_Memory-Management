#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

static int timeStamp;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int lru_evict() {
	int victim = 0;

	// victim will have the earliest timeStamp of the last reference
	for (int i = 1; i < memsize; i++) {
		if (coremap[i].time < coremap[victim].time)
			victim = i;
	}
	return victim;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
	// uodate timeStamp after each reference
	coremap[p->frame>>PAGE_SHIFT].time = timeStamp;
	timeStamp++;
	return;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
	timeStamp = 0;
}
