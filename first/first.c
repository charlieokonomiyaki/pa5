#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
	int val;
	char* name;
	struct node* next;
};

struct gate{
	int index; //tells you what type of gate it is
	char* name;
	char** inputs; 
	int amnt;
	struct gate* next;
};
int getCommand(char* identifier);
int* getBinaryArray(int num, int numberOfInputs);
int* runArithmetic(int* inputs, struct gate* head, int numberOfInputs, char** orderOfInputs, char** outputNames, int numberOfOutputs);
void printList(struct gate* head);
void printStringArray(char** arr, int size);
struct node* insert(struct node* temp, struct node* head);
int search(char* var, char** orderOfInputs, int* inputVar, struct node* outputHead, int numberOfInputs);
void printNodeList(struct node* head);

struct node* insert(struct node* temp, struct node* head){
	struct node* ptr = head;

	if(head == NULL){
		return temp;
	}else{
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
	}
	ptr->next = temp;
	return head;
}

int search(char* var, char** orderOfInputs, int* inputVar, struct node* outputHead, int numberOfInputs){

	for(int i = 0; i<numberOfInputs; i++){
		if(strcmp(var, orderOfInputs[i]) == 0){
			return inputVar[i];
		}
	}

	struct node* ptr = outputHead;
	while(ptr != NULL){
		if(strcmp(var,ptr->name) == 0){
			return ptr->val;
		}
		ptr = ptr->next;
	}

	return -1;


}

int* runArithmetic(int* inputVar, struct gate* head, int numberOfInputs, char** orderOfInputs, char** outputNames, int numberOfOutputs){

	int* answer = malloc(numberOfOutputs*sizeof(int));
	struct gate* runner = head;
	struct node* outputHead = NULL;

	while(runner != NULL){
		//printf("%s--->", runner->name);
		if(runner->index == 1){
			int val1 = search(runner->inputs[0], orderOfInputs, inputVar, outputHead, numberOfInputs);
			struct node* temp = malloc(sizeof(struct node));
			temp->name = runner->inputs[1];
			temp->next = NULL;
			//printf("val1: %d\n", val1);

			temp->val = ~(val1);

			//printf("temp->Val: %d", temp->val);

			outputHead = insert(temp, outputHead);
		}

		if(runner->index > 1 && runner->index<8){
			int val1 = search(runner->inputs[0], orderOfInputs, inputVar, outputHead, numberOfInputs);
			int val2 = search(runner->inputs[1], orderOfInputs, inputVar, outputHead, numberOfInputs);
			struct node* temp = malloc(sizeof(struct node));
			temp->name = runner->inputs[2];
			temp->next = NULL;
			if(strcmp(runner->name, "AND") == 0){
				temp->val = val1 & val2;
				//printf("%d ", temp->val);
			} else if(strcmp(runner->name, "OR") == 0){
				temp->val = val1 | val2;
				//printf("%d ", temp->val);
			} else if(strcmp(runner->name, "NAND") == 0){
				temp->val = !(val1 & val2);
				//printf("%d ", temp->val);
			} else if(strcmp(runner->name, "NOR") == 0){
				temp->val = !(val1 | val2);
				//printf("%d ", temp->val);
			} else if(strcmp(runner->name, "XOR") == 0){
				temp->val = (val1 ^ val2);
				//printf("%d ", temp->val);
			} else if(strcmp(runner->name, "XNOR") == 0){
				temp->val = !(val1 ^ val2);
				//printf("%d ", temp->val);
			} 

			outputHead = insert(temp, outputHead); 

		}
		runner = runner->next;
	}

	//printNodeList(outputHead);

	for(int i = 0; i<numberOfOutputs; i++){
		answer[i] = search(outputNames[i], orderOfInputs, inputVar, outputHead, numberOfInputs);
	}

	//printf("\n");

	return answer;
}

//CONSRTRUCTING BINARY REPRESENTATION OF A NUMBER
int* getBinaryArray(int num, int numberOfInputs){
	int* arr = malloc(numberOfInputs*(sizeof(int)));

	for(int i = numberOfInputs-1; i>=0; i--){

		int x = 1<<i;
		if(x <= num){
			arr[numberOfInputs-(i+1)] = 1;
			num = num-x;
		} else{
			arr[numberOfInputs-(i+1)] = 0;
		}


	}

	return arr;
}

int getCommand(char* identifier){

	if(strcmp(identifier, "NOT") == 0){
		return 1;
	}
	if(strcmp(identifier, "AND") == 0){
		return 2;
	}
	if(strcmp(identifier, "OR") == 0){
		return 3;
	}
	if(strcmp(identifier, "NAND") == 0){
		return 4;
	}
	if(strcmp(identifier, "NOR") == 0){
		return 5;
	}
	if(strcmp(identifier, "XOR") == 0){
		return 6;
	}
	if(strcmp(identifier, "XNOR") == 0){
		return 7;
	}
	if(strcmp(identifier, "DECODER") == 0){
		return 8;
	}
	if(strcmp(identifier, "MULTIPLEXER") == 0){
		return 9;
	}
	return 0;
}

int main(int argc, char** argv){

	FILE* fp = fopen(argv[1], "r");

    struct gate* head = NULL;
    struct gate* current = NULL;

    int numberOfInputs = 0;
  	char** orderOfInputs;
    int numberOfOutputs = 0;
    char** outputNames;

    char identifier[64]; 
    while(fscanf(fp, "%s", identifier)>0){

    	int index = getCommand(identifier);

    	if(index != 0){
    		struct gate* temp = malloc(sizeof(struct gate));
    		temp->name = malloc(64*sizeof(char));
    		strcpy(temp->name,identifier);
    		temp->index = index;
    		temp->next = NULL;

	    	if(head == NULL){
	    		head = temp;
	    		current = head;
	    	} else{
	    		current->next = temp;
	    		current = current->next;
	    	}
	   		
	    	if(index == 1){
	    		current->inputs = malloc(2*sizeof(char*));
	    		for(int i = 0; i<2; i++){
	    			current->inputs[i] = malloc(64*sizeof(char));
	    		}
	    	}else if(index > 1 && index < 8){
	    		current->inputs = malloc(3*sizeof(char*));
	    		for(int i = 0; i<3; i++){
	    			current->inputs[i] = malloc(64*sizeof(char));
	    		}
	    	} else {
	    		int numOfGateInputs = fscanf(fp, "%d\n", &numOfGateInputs);
	    		current->inputs = malloc(numOfGateInputs*sizeof(char*));
	    		for(int i = 0; i<numOfGateInputs; i++){
	    			current->inputs[i] = malloc(64*sizeof(char));
	    		}
	    	}
	    	current->amnt = 0;


	    	//printf("%s: %d\n", identifier, current->index);

	    } 
	    else if(strcmp(identifier, "INPUTVAR") == 0){
	    	fscanf(fp, "%d", &numberOfInputs);
	    	orderOfInputs = malloc(numberOfInputs*sizeof(char*));
	    	for(int i = 0; i<numberOfInputs; i++){
	    		orderOfInputs[i] = malloc(64*sizeof(char));
	    		fscanf(fp, "%s", identifier);
	    		strcpy(orderOfInputs[i], identifier);
	    	}

	    }
	    else if(strcmp(identifier, "OUTPUTVAR") == 0){
	    	fscanf(fp, "%d", &numberOfOutputs);
	    	outputNames = malloc(numberOfOutputs*sizeof(char*));
	    	for(int i = 0; i<numberOfOutputs; i++){
	    		outputNames[i] = malloc(64*sizeof(char));
	    		fscanf(fp, "%s", identifier);
	    		strcpy(outputNames[i], identifier);
	    	}

	    }else{

		    //printf("Identifier: %s\n", identifier);
		    if(head != NULL){
		    //	printf("current index: %d\n", current->index);

		    	//current->inputs[current->amnt] = "hello cat";

		    	strcpy(current->inputs[current->amnt], identifier);

		   		current->amnt++;
		    }

		}

    }

    //printf("hello\n");

    //printf("numberOfInputs: %d\n", numberOfInputs);
    //printStringArray(orderOfInputs, numberOfInputs);

    //printf("numberOfOutputs: %d\n", numberOfOutputs);
    //printStringArray(outputNames, numberOfOutputs);

//generating grey code 
    int g;
    int n = 1<<numberOfInputs;
    for(int i = 0; i<n; i++){
    	g = i ^ (i >> 1);
	    int* array = getBinaryArray( g, numberOfInputs );
	    int* x = runArithmetic(array, head, numberOfInputs, orderOfInputs, outputNames, numberOfOutputs);
//SOULUTION IMPORTANT
	    for(int i = 0;  i<numberOfInputs; i++){
	    	printf("%d ", array[i]);
	    }
//PRINTS OUTPUT IMPORTANT
	    for(int i = 0; i<numberOfOutputs; i++){
	     	printf("%d ", x[i]);
	    }
	    printf("\n");
    	// printf("G: %d\n", g);
    }


    return 0;

}

void printList(struct gate* head){
	struct gate* ptr = head;

    while(ptr != NULL){
    	printf("Gate: %d, GateName: %s\n", ptr->index, ptr->name);
    	for(int i = 0; i<ptr->amnt; i++){
    		printf("%s ", ptr->inputs[i]);
    	}
    	printf("\n");

    	ptr = ptr->next;
    }
}

void printNodeList(struct node* head){
	struct node* ptr = head;

    while(ptr != NULL){
    	
    	printf("name: %s, val: %d---->", ptr->name, ptr->val);

    	ptr = ptr->next;
    }
    printf("\n");
}

void printStringArray(char** arr, int size){
    for(int i = 0; i<size; i++){
    	printf("%s ", arr[i]);
    }
    printf("\n");
}