#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
	int val;
	struct node* next;
};

int main(int argc, char** argv){

	int array[] = {1 , 2, 3 , 4, 5};

    int* arraytwo = malloc(5*sizeof(int));
    
    for(int i = 0; i<5; i++){
        arraytwo[i] = array[i];
    }
    
    int* arraythree = malloc(5*sizeof(int));
    
    for(int i = 0; i<5; i++){
        arraythree[i] = arraytwo[i];
    }
    
    
    arraytwo[0]++;
    
    for(int i=0; i<5; i++){
        printf("%d ", arraytwo[i]);
    }
    
    printf("\n");
    
    for(int i = 0; i<5; i++){
        printf("%d ", arraythree[i]);
    }
    
    printf("\n");

}
