#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
	char* name;
	int index;
	struct node* next;
};

struct gate{
	char* name;
	int numberOfInputs;
	char** namesOfInputs;
	int numberOfOutputs;
};

int main(int argc, char** argv){

	FILE* fp = fopen(argv[1], "r");

    struct node* head = NULL;
    struct node* current = NULL;
    int count = 0;

    char identifier[64]; 
    while(fscanf(fp, "%s\n", identifier)>0){
    	if(head == NULL){
    		head = malloc(sizeof(struct node));
    		head->name = identifier;
    		head->index = count;
    		current = head->next;
    	} else{
    		current = malloc(sizeof(struct node));
    		current->name = identifier;
    		current->index = count;
    		current = current->next;
    	}
    	count++;
    	// } else{
    	// 	current->next = malloc(sizeof(struct node));
    	// 	current->next->name = identifier;
    	// 	current = current->next;
    	// }
    	// printf("%s\n", head->name);
    	// current->next = NULL;
    	// current->index = count;
    	// count++;
    }

    struct node* ptr = head;

    while(ptr != NULL){
    	printf("index: %d, string: %s\n", ptr->index, ptr->name);
    	ptr = ptr->next;
    }

    return 0;

}