#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "sim.h"
#include "pagetable.h"

//extern int memsize;

extern int debug;

extern char *tracefile;

extern struct frame *coremap;

typedef struct line_struct {
	int lineNum;
	struct line_struct *next;
} Node;

typedef struct line_container {
	Node head;
	Node tail;
} Container;

Container *firstLevel[PTRS_PER_PGDIR];

/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {
	
	return 0;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {

	return;
}

/* Insert a Node contain line number into a corresponding container of vaddr
 * Input: current line number, vitual address
 */
void allocate_node(line, vaddr) {
	;
}



/* Initializes any data structures needed for this
 * replacement algorithm.
 */
void opt_init() {
	FILE *tfp;
	char buf[MAXLINE];
	addr_t vaddr = 0;
	char type;
	int line_number = 1;
	
	if((tfp = fopen(tracefile, "r")) == NULL) {
		perror("Error opening tracefile:");
		exit(1);
	}

	while(fgets(buf, MAXLINE, tfp) != NULL) {

		if(buf[0] != '=') {
			sscanf(buf, "%c %lx", &type, &vaddr);
			if(!debug)  {
				printf("%c %lx\n", type, vaddr);
			}
			allocate_node(line, vaddr);
		} else {
			continue;
		}
		line_number++;
	}
}

