#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

static int start;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict() {
	int victim = start;
	while (1) {
		// if victim is not referenced
		if (!(coremap[victim].pte->frame & PG_REF))
			break;
		
		// if victim is referenced
		coremap[victim].pte->frame &= ~PG_REF;
		victim = (victim+1)%memsize;
	}
	start = (victim+1)%memsize;
	return victim;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	// "p->frame |= PG_REF" is already done in pagetable.c no need to repeat
	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {
	start = 0;
}
