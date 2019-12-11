#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct node{
	int val;
	char* name;
	struct node* next;
};

struct gate{
	int index; //tells you what type of gate it is
	char* name;
	char** inputs;
	int gateInputs; //used mainly for multiplexer and decoder 
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
void printIntArray(int* arr, int size);
int searchTwo(struct gate* ptr, char** orderOfInputs, struct node* outputFront, int numberOfInputs);
struct gate* appendToEnd(struct gate* dnode, struct gate* head);

struct gate* appendToEnd(struct gate* dnode, struct gate* head){

	//printf("hello------------>from appendToEnd\n");

	struct gate* ptr = head;
	struct gate* prev = NULL;

	if(dnode == head){
		head = head->next;
	}else{
		//delete
		while(ptr!=dnode){
			prev = ptr;
			ptr = ptr->next;
		}

		prev->next = ptr->next;
	}
	while(ptr != NULL){
		prev = ptr;
		ptr = ptr->next;
	}

	prev->next = dnode;
	dnode->next = NULL;

	//append to end

	return head;
}

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

int searchTwo(struct gate* ptr, char** orderOfInputs, struct node* outputFront, int numberOfInputs){
	int inputsFound = 0;
	
	//printf("Name: %s\n", ptr->name);

	for(int i = 0; i<ptr->gateInputs; i++){

		//printf("inputName: %s\n", ptr->inputs[i]);

		for(int j = 0; j<numberOfInputs; j++){
			if(strcmp(ptr->inputs[i], orderOfInputs[j]) == 0){
				inputsFound++;
			}
		}

		struct node* outputPtr = outputFront;
		while(outputPtr != NULL){
			if(strcmp(ptr->inputs[i], outputPtr->name) == 0){
				inputsFound++;
			}
			outputPtr = outputPtr->next;
		}

		if(strcmp(ptr->inputs[i], "0") == 0 || strcmp(ptr->inputs[i], "1") == 0){
			inputsFound++;
		}

	}

	if(inputsFound == ptr->gateInputs){
		return 1;
	}

	return 0;

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

	int itself = atoi(var);

	return itself; //this will return the number itself as a digit

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
		if(runner->index == 9){

			struct node* temp = malloc(sizeof(struct node));
			temp->name = runner->inputs[runner->amnt-1];
			temp->next = NULL;

			//printf("hello\n");
			//printf("gateInputs:%d amnt: %d\n", runner->gateInputs, runner->amnt);

			//printf("---------------GRAY CODE ARRAY--------------------\n");

			int* values = malloc((runner->amnt-1)*sizeof(int));
			int* tempValues = malloc((runner->amnt-1)*sizeof(int));


			for(int i = 0; i<(runner->amnt-1); i++){
				values[i] = search(runner->inputs[i], orderOfInputs, inputVar, outputHead, numberOfInputs);
			}

			//printf("Initial: \n");
			//printIntArray(values, runner->amnt-1);
			int* total = malloc(runner->gateInputs*sizeof(int));

			int g;
			int logOfgateInputs = log2(runner->gateInputs);
			int n = 1<<logOfgateInputs;
			for(int i = 0; i<n; i++){ //THE GRAYCODE LOOP

				//copying values

				for(int j = 0; j<(runner->amnt-1); j++){
					tempValues[j] = values[j];
				}

				//end of copyingvalues

				g = i ^ (i>>1);
				int* array = getBinaryArray(g, logOfgateInputs);

				//printf("G: %d\n", g);
				//printIntArray(array, logOfgateInputs);
				
				for(int j = 0; j<logOfgateInputs; j++){
					if(array[j] == 0){
						tempValues[runner->gateInputs+j] = !(tempValues[runner->gateInputs+j]);
					} 
				}
				//printf("RESULT\n");
	    		//printIntArray(tempValues, runner->amnt-1);

	    		total[i] = tempValues[i];

	    		//printf("This is the or:\n");
	    		//printf("%d ", total[i]);

	    		for(int j = 0; j<logOfgateInputs; j++){
	    			total[i] = total[i] & tempValues[runner->gateInputs+j];
	    			//printf("%d ", tempValues[runner->gateInputs+j]);
	    		}

	    		//printf("\n");
			}

			int multiOutput = 0;

			for(int i = 0; i<runner->gateInputs; i++){
				multiOutput =  multiOutput|total[i];
			}

			temp->val = multiOutput;
			//printf("multiOutput: %d\n", multiOutput);

			//printf("---------------END OF GRAY CODE ARRAY-------------\n");

			//printf("Inserting %d into %s\n", temp->val, temp->name);
			outputHead = insert(temp, outputHead);
		}//ENDOF MULTIPLEXER

		if(runner->index == 8){
			//printf("hello\n");

			int* values = malloc(runner->gateInputs*sizeof(int));
			int* tempValues = malloc(runner->gateInputs*sizeof(int));

			for(int i = 0; i<runner->gateInputs; i++){
				values[i] = search(runner->inputs[i], orderOfInputs, inputVar, outputHead, numberOfInputs);
			}

			//printStringArray(runner->inputs, runner->gateInputs);
			//printIntArray(values, runner->gateInputs);
			//printStringArray(outputNames, numberOfOutputs);

			int g;
			int n = 1<<(runner->gateInputs);

			for(int i = 0; i<n; i++){

				for(int j = 0; j<(runner->gateInputs); j++){
					tempValues[j] = values[j];
				}

				struct node* temp = malloc(sizeof(struct node));
				temp->name = runner->inputs[runner->gateInputs+i]; //think about changing this to strcpy
				//printf("OUTPUT INDEX: %d", runner->gateInputs+i);
				//printf("Name: %s\n", runner->inputs[runner->gateInputs+i]);
				temp->next = NULL;
				g = i ^ (i>>1);

				int* array = getBinaryArray(g, runner->gateInputs);
				//printf("G: %d\n", g);
				//printIntArray(array, runner->gateInputs);

				for(int j=0; j<runner->gateInputs; j++){
					if(array[j] == 0){
						tempValues[j] = !(tempValues[j]);
					} 
				}

				int decoderOutput = 1;
				for(int j = 0; j<runner->gateInputs; j++){
					decoderOutput = decoderOutput & tempValues[j];
				}	

				temp->val = decoderOutput;

				outputHead = insert(temp, outputHead);
			} //end of gray loop
		}//ENDOF DECODER

		runner = runner->next;
	}

	//printNodeList(outputHead);

	for(int i = 0; i<numberOfOutputs; i++){
		answer[i] = search(outputNames[i], orderOfInputs, inputVar, outputHead, numberOfInputs);
	}

	//printNodeList(outputHead);

	//printStringArray(outputNames, numberOfOutputs);

	//printIntArray(answer, numberOfOutputs);

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

	if(fp == NULL){
		printf("error\n");
		exit(0);
	}

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
	    		temp->gateInputs = 1;
	    	}else if(index > 1 && index < 8){
	    		current->inputs = malloc(3*sizeof(char*));
	    		for(int i = 0; i<3; i++){
	    			current->inputs[i] = malloc(64*sizeof(char));
	    		}
	    		temp->gateInputs = 2;
	    	} else if (index == 9){ //mutliplexer
	    		//printf("helllo\n");

	    		int numOfGateInputs;
	    		fscanf(fp, "%d\n", &numOfGateInputs);

	    		//printf("%d", numOfGateInputs);

	    		temp->gateInputs = numOfGateInputs;

	    		int numOfMultiInputs = numOfGateInputs+log2(numOfGateInputs) + 1;

	    		current->inputs = malloc(numOfMultiInputs*sizeof(char*));
	    		for(int i = 0; i<numOfMultiInputs; i++){
	    			current->inputs[i] = malloc(64*sizeof(char));
	    		}

	    	} else if(index == 8){
	    		int numOfGateInputs;
	    		fscanf(fp, "%d\n", &numOfGateInputs);

	    		temp->gateInputs = numOfGateInputs;
	    		int numOfDecoderInputs = (numOfGateInputs) + (1<<numOfGateInputs); //inputs + outputs

	    		current->inputs = malloc(numOfDecoderInputs*sizeof(char*));
	    		for(int i = 0; i<numOfDecoderInputs; i++){
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

    // printf("----------------INITIAL--------------------\n");
    // printList(head);
    // printf("----------------INTIAL END--------------------\n");

   	//do some stuff here
    struct gate* ptr = head;

    struct gate* prev = NULL;

    struct node* outputFront = NULL;

    while(ptr != NULL){
    	if(searchTwo(ptr,orderOfInputs, outputFront,numberOfInputs) == 1){
    		//printf("found!\n");

    		for(int e = ptr->gateInputs; e<(ptr->amnt); e++){
    			struct node* tmp = malloc(sizeof(struct node));
    			tmp->name = malloc(64*sizeof(char));


    			tmp->name = ptr->inputs[e];

    			tmp->next = NULL;
    			tmp->val = 0;

    			outputFront = insert(tmp, outputFront);

    		}

    		//printNodeList(outputFront);

    	} else{
    		head = appendToEnd(ptr, head);
    		//printf("Cannot compute\n");
    		ptr = prev;
    	}
    	if(prev == NULL){
    		prev = head;
    	} else{
    		prev = ptr;
    	}
    	ptr = ptr->next;
    }

    // printf("----------------FINAL--------------------\n");
    // printList(head);


    //printf("hello\n");

    //printf("numberOfInputs: %d\n", numberOfInputs);
    //printStringArray(orderOfInputs, numberOfInputs);

    //printf("numberOfOutputs: %d\n", numberOfOutputs);
    //printStringArray(outputNames, numberOfOutputs);


//-------------------------------------------UNCOMMENT ALL OF BOTTOM------------------------------------
//generating grey code 
    int g;
    int n = 1<<numberOfInputs;
    for(int i = 0; i<n; i++){
    	g = i ^ (i >> 1);
	    int* array = getBinaryArray( g, numberOfInputs );
//printf("--------RUNNING ARITHMATIC-------------\n");
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

    fclose(fp);
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

	if(ptr == NULL){
		printf("list is empty");
	}

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

void printIntArray(int* arr, int size){
	for(int i = 0;  i<size; i++){
	    printf("%d ", arr[i]);
	}

	printf("\n");
}