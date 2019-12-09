#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
	int val;
	struct node* next;
};

struct gate{
	int index; //tells you what type of gate it is
	int* inputs; 
	int amnt;
	struct gate* next;
};
int getCommand(char* identifier);
int* getBinaryArray(int num, int numberOfInputs);
int runArithmetic(int* inputs, struct gate* head);

int runArithmetic(int* inputs, struct gate* head){

	return 0;
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
    int numberOfOutputs = 0;

    char identifier[64]; 
    while(fscanf(fp, "%s\n", identifier)>0){

    	int index = getCommand(identifier);

    	if(index != 0){
	    	if(head == NULL){
	    		head = malloc(sizeof(struct gate));
	    		head->index = index;
	    		current = head;
	    	} else{
	    		current = current->next;
	    		current = malloc(sizeof(struct gate));
	    		current->index = index;
	    	}
	    	
	    	if(index == 1){
	    		current->inputs = malloc(2*sizeof(int));
	    	}else if(index > 1 && index < 8){
	    		current->inputs = malloc(3*sizeof(int));
	    	} else {
	    		int numOfGateInputs = fscanf(fp, "%d\n", &numOfGateInputs);
	    		current->inputs = malloc(numOfGateInputs*sizeof(int));
	    	}
	    	current->amnt = 0;


	    	printf("%s: %d\n", identifier, current->index);
	    	current->next = NULL;
	    } 
	    else if(strcmp(identifier, "INPUTVAR") == 0){
	    	fscanf(fp, "%d", &numberOfInputs);
	    }
	    else if(strcmp(identifier, "OUTPUTVAR") == 0){
	    	fscanf(fp, "%d", &numberOfOutputs);

	    }else{
		    char* s = identifier;
		    int i = 0;
		    while(i < s[i]){
		    	i++;
		    }
		    i--;
		    int num = s[i]-'0';
		    printf("Identifier: %s, %d\n", identifier, num);


		    if(head != NULL){
		    	current->inputs[current->amnt] = num;
		   		current->amnt++;
		   		printf("current->amnt: %d\n", current->amnt);
		    }

		}

    }

    printf("numberOfInputs: %d\n", numberOfInputs);
    printf("numberOfOutputs: %d\n", numberOfOutputs);

//generating grey code 
    int g;
    int n = 1<<numberOfInputs;
    for(int i = 0; i<n; i++){
    	g = i ^ (i >> 1);
	    int* array = getBinaryArray( g, numberOfInputs );
	    int x = runArithmetic(array, head);

	    for(int i = 0;  i<numberOfInputs; i++){
	    	printf("%d ", array[i]);
	    }

	    printf("%d ", x);

    	printf("G: %d\n", g);
    }

    return 0;

}