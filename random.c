#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
	int val;
	struct node* next;
};

int main(int argc, char** argv){

	int array[] = {1 , 2, 3 , 4, 5};

	int arraySize = 5;

	struct node* head = NULL;
	struct node* current = NULL;

	for(int i = 0; i<arraySize; i++){
		struct node* temp = malloc(sizeof(struct node));
		temp->val = array[i];
		temp->next = NULL;

		if(head == NULL){
			head = temp;
			current = head;
		} else{
			current->next = temp;
			current = current->next;
		}
	}

	struct node* ptr = head;

    while(ptr != NULL){
    	printf("Value: %d\n", ptr->val);
    	ptr = ptr->next;
    }


}