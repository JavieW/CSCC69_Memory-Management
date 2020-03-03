#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "sim.h"
#include "pagetable.h"

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

Container ***hash_table;

/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {
	int victim = 0;
	int late_row, late_col;
	int curr_row, curr_col;
	for (int i=1; i<memsize; i++) {

		late_row = coremap[victim].hash_row;
		late_col = coremap[victim].hash_col;
		curr_row = coremap[i].hash_row;
		curr_col = coremap[i].hash_col;

		// if the vaddr list at "victim" frame is empty
		if (hash_table[late_row][late_col]->head == NULL) {
			return victim;
		// if the vaddr list at current frame is empty
		} else if (hash_table[curr_row][curr_col]->head == NULL) {
			return i;
		// if we found a better victim
		} else if (hash_table[curr_row][curr_col]->head->line_num >	\
					hash_table[late_row][late_col]->head->line_num) {
			victim = i;
		}
	}
	return victim;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {
	int row = coremap[p->frame>>PAGE_SHIFT].hash_row;
	int col = coremap[p->frame>>PAGE_SHIFT].hash_col;

	// uptade hash_table after finished a refernece
	Node *ptr = hash_table[row][col]->head;
	assert(ptr!=NULL);
	hash_table[row][col]->head = ptr->next;
	free(ptr);
}

/* Helper function for initializing hash function
 * Insert a Node contain line number into a corresponding container of vaddr
 * Input: current line number, vitual address
 */
void allocate_node(int line_number, addr_t vaddr) {
	int row = PGDIR_INDEX(vaddr)&PGTBL_MASK;
	int col = PGTBL_INDEX(vaddr);
	Container **hash_table_row;
	Container *hash_table_cell;
	Node *newLine;

	// initialize hash_table_row if necessary
	hash_table_row = hash_table[row];
	if (hash_table_row == NULL) {
		hash_table_row = (Container **)calloc(PTRS_PER_PGTBL, sizeof(Container *));
		if (hash_table_row == NULL) {
			perror("Initializing hash_table_row of hash table failed: ");
			exit(1);
		}
		hash_table[row] = hash_table_row;
	}
	assert(hash_table[row] != NULL);

	// initialize hash_table_cell if necessary
	hash_table_cell = hash_table_row[col];
	if (hash_table_cell == NULL) {
		hash_table_cell = (Container *)calloc(1, sizeof(Container));
		if (hash_table_cell == NULL) {
			perror("Initializing hash_table_cell of hash table failed: ");
			exit(1);
		}
		hash_table_cell->head = NULL;
		hash_table_cell->tail = NULL;
		hash_table_row[col] = hash_table_cell;
	}
	assert(hash_table_row[col] != NULL);

	// initialize new line Node
	if((newLine = (Node *)malloc(sizeof(Node)))==NULL) {
		perror("Initializing new line Node failed: ");
		exit(1);
	}
	newLine->line_num = line_number;
	newLine->next = NULL;

	// insert new line Node into tail of the container
	if (hash_table_cell->head == NULL) {
		hash_table_cell->head = newLine;
		hash_table_cell->tail = newLine;
	} else {
		hash_table_cell->tail->next = newLine;
		hash_table_cell->tail = newLine;
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

	// initialize hash_table
	hash_table = (Container ***)calloc(PTRS_PER_PGDIR, sizeof(Container **));
	if (hash_table == NULL) {
		perror("Initializing hash_table of hash table failed: ");
		exit(1);
	}
	
	// open tracefile
	if((tfp = fopen(tracefile, "r")) == NULL) {
		perror("Error opening tracefile:");
		exit(1);
	}

	// read each line of the tracefile, and fill in the hash table
	while(fgets(buf, MAXLINE, tfp) != NULL) {
		if(buf[0] != '=') {
			sscanf(buf, "%c %lx", &type, &vaddr);
			if(debug)  {
				printf("%c %lx\n", type, vaddr);
			}
			allocate_node(line_number, vaddr);
		} else {
			continue;
		}
		line_number++;
	}
}

