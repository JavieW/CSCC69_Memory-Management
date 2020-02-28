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
	int i;
	// loop through the coremap
	for (i = start; ; i++) {
		// if the page's reference bit is 1, make it 0
		if (coremap[i].pte->frame & PG_REF) {
			coremap[i].pte->frame &= ~PG_REF;
		} 
		else {
			// set the ref bit to 1
			coremap[i].pte->frame |= PG_REF;
			// update the start index
			if (i + 1 == memsize)
				start = 0;
			else
				start = i + 1;
			break;
		}
		if (i + 1 == memsize)
			i = 0;
	}
	return i;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	p->frame |= PG_REF;
	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {
	start = 0;
}
