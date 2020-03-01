#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    A = 0,
    C = 1,
    G = 2,
    T = 3,
} Nucleotide;  // these are just the integers 0, 1, 2, 3 with fancy names!
typedef Nucleotide Codon[3];

typedef struct CodonNode_struct {
    Codon codon;
    struct CodonNode_struct *next;    
} CodonNode;

/**
 * Sort the codons array in place, in ascending order.
 *
 * EX: sort_codons({{A, A, C}, {A, A, A}}, 2) => {{A, A, A}, {A, A, C}}
 *
 * NOTE: The codons should be sorted based on their own value,
 *          NOT the value of their associated amino acids.
 *
 * REQ:
 *  - The "codons" array points to an array of codons with exactly num_codons in it
 *  - Each codon contains exactly 3 Nucleotides
 *  - Each nucleotide can only take on a value in the set {A, C, G, T}
 *
 * INPUT:
 *  - codons: An array of codons, likely unsorted
 *  - num_codons: The number of codons that are in the array
 *
 * OUTPUT:
 *  - codons: The same array of codons, now sorted
 */
void sortCodons(Codon codons[], int num_codons)
{
    /**
     * TODO: Implement this function, with O(num_codons) worst case time complexity.
     *
     * HINTS:
     *  - How many possible codons are there? How many buckets do you need?
     *  - Are any 2 of the same type in the list different in some way? No.
     *      - That is {A, C, G} == {A, C, G}, even if they are not at the same memory location.
     *  - So, copying data is the same as recreating it elsewhere....
     *
     * NOTE: For this exercise, you can assume that:
     *  - there will only ever be 4 nucleotides
     *  - there will always be 3 nucleotides per codon.
     */

    // creat 64 buckets (4*4*4 = 64 conbinations)
    CodonNode **buckets = calloc(64, sizeof(CodonNode *));
    int i, j, idx;
    CodonNode *new_node=NULL;
    CodonNode *p=NULL;
    CodonNode *q=NULL;

    for (i=0; i<num_codons; i++) {
        idx = codons[i][0]*16 + codons[i][1]*4 + codons[i][2];
        new_node = (CodonNode *)calloc(1, sizeof(CodonNode));
        new_node->codon[0] = codons[i][0];
        new_node->codon[1] = codons[i][1];
        new_node->codon[2] = codons[i][2];
        // insert new_node at head of the corresponding bucket
        new_node->next = buckets[idx];
        buckets[idx] = new_node;
    }

    i = 0;
    for (j=0; j<64; j++) {
        p = buckets[j];
        while (p!=NULL) {
            codons[i][0] = p->codon[0];
            codons[i][1] = p->codon[1];
            codons[i][2] = p->codon[2];
            i++;
            q = p->next;
            free(p);
            p = q;
        }
    }
}

int main()
{
    Codon test_codons[25] = {
        {A, T, G}, {A, A, C}, {T, A, A}, {C, T, G}, {A, G, C},
        {T, T, A}, {A, T, G}, {G, T, C}, {C, C, A}, {C, C, C},
        {A, G, C}, {T, T, A}, {A, T, G}, {A, A, C}, {T, G, A},
	{G, G, G}, {A, G, C}, {A, T, A}, {A, T, G}, {G, T, C},
        {C, C, A}, {G, C, C}, {T, G, C}, {A, T, A}, {G, C, C}
    };
    // run this program 4 times
    for (int i=0; i<4; i++)
    	sortCodons(test_codons, 3);  // calls your sorting code
}
