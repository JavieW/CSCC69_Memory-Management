#include <stdio.h>
#include <stdlib.h>

#define TUPLE_SIZE      5
#define ENTRY_RANGE      5
#define TEST_SIZE       5000

typedef int Tuple[TUPLE_SIZE];

typedef struct TupleNode_struct {
    Tuple tuple;
    struct TupleNode_struct *next;    
} TupleNode;

void bucketSort(Tuple tuples[], int num_tuples)
{
    int bucket_size = 1;
    for (int i=0; i<TUPLE_SIZE; i++)
        bucket_size = bucket_size * ENTRY_RANGE;
    // creat buckets
    TupleNode **buckets = calloc(bucket_size, sizeof(TupleNode *));
    int i, idx;
    TupleNode *new_node=NULL;
    TupleNode *p=NULL;
    TupleNode *q=NULL;

    for (i=0; i<num_tuples; i++) {
        idx = 0;
        for (int j=0; j < TUPLE_SIZE; j++) {
            idx = idx*ENTRY_RANGE + tuples[i][j];
        }
        // printf("%d\n", idx);

        new_node = (TupleNode *)calloc(1, sizeof(TupleNode));

        for (int j=0; j < TUPLE_SIZE; j++)
            new_node->tuple[j] = tuples[i][j];

        // insert new_node at head of the corresponding bucket
        new_node->next = buckets[idx];
        buckets[idx] = new_node;
    }

    for (i=0; i<bucket_size; i++) {
        p = buckets[i];
        while (p!=NULL) {
            for (int j=0; j < TUPLE_SIZE; j++)
                tuples[i][j] = p->tuple[j];
            i++;
            q = p->next;
            free(p);
            p = q;
        }
    }
}

int main(int argc, char* argv[])
{
    int ran;
    Tuple large_tupleset[TEST_SIZE];
    // genereate a larger set
    for (int i=0; i<TEST_SIZE; i++) {
        for (int j=0; j < TUPLE_SIZE; j++) {
            ran = random()%ENTRY_RANGE;
            large_tupleset[i][j] = ran;
        }            
    }
    bucketSort(large_tupleset, TEST_SIZE);  // calls your sorting code

    // printf("%d, %d, %d\n", large_tupleset[0][0],large_tupleset[0][1], large_tupleset[0][2]);
    // printf("%d, %d, %d\n", large_tupleset[1][0],large_tupleset[1][1], large_tupleset[1][2]);
    // printf("%d, %d, %d\n", large_tupleset[2][0],large_tupleset[2][1], large_tupleset[2][2]);
    // printf("%d, %d, %d\n", large_tupleset[3][0],large_tupleset[3][1], large_tupleset[3][2]);
    // printf("%d, %d, %d\n", large_tupleset[4][0],large_tupleset[4][1], large_tupleset[4][2]);

}
