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
	int line_num;
	struct line_struct *next;
} Node;

typedef struct line_container {
	Node *head;
	Node *tail;
} Container;

Container ***firstLevel;

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
void allocate_node(int line_number, addr_t vaddr) {
	// find the secondLevel of container
	Container **secondLevel = firstLevel[PGDIR_INDEX(vaddr)&PGTBL_MASK];
	Container *container_ptr;
	Node *newLine;

	// initilize secondLevle if necessary
	if (secondLevel == NULL) {
		secondLevel = (Container **)calloc(PTRS_PER_PGTBL, sizeof(Container *));
		if (secondLevel == NULL) {
		perror("Initializing secondLevel of hash table failed: ");
		exit(1);
		}
	}

	// find the corresponding container of vaddr
	container_ptr = secondLevel[PGTBL_INDEX(vaddr)];

	// initilize container_ptr if necessary
	if (container_ptr == NULL) {
		container_ptr = (Container *)calloc(1, sizeof(Container));
		if (container_ptr == NULL) {
		perror("Initializing container_ptr of hash table failed: ");
		exit(1);
		}

		container_ptr->head = NULL;
		container_ptr->tail = NULL;
	}

	// initialize new line Node
	if((newLine = (Node *)malloc(sizeof(Node)))==NULL) {
		perror("Initializing new line Node failed: ");
		exit(1);
	}
	newLine->line_num = line_number;
	newLine->next = NULL;

	// insert it into tail of the container
	// if it is an empty container
	if (container_ptr->head == NULL) {
		container_ptr->head = newLine;
		container_ptr->tail = newLine;
	} else {
		container_ptr->tail->next = newLine;
		container_ptr->tail = newLine;
	}

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

	// initialize first level
	firstLevel = (Container ***)calloc(PTRS_PER_PGDIR, sizeof(Container **));
	if (firstLevel == NULL) {
		perror("Initializing firstLevel of hash table failed: ");
		exit(1);
	}
	
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
			allocate_node(line_number, vaddr);
		} else {
			continue;
		}
		line_number++;
	}
}

