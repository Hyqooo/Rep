#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NOT_OK 1
#define OK 0
#define MAX_LENGTH 20

int pop(int);
int top(int);
int print(int);
int clear(int);
int push(int, int);
void commandManager();

typedef struct Stack{
	int value;
	struct Stack *next;
}Stack_t;

Stack_t *top_1, *top_2, *stackHead_1, *stackHead_2;

int main(int argc, char *argv[]){
	commandManager();
}

void commandManager(){
	char command[MAX_LENGTH];
	int stackNumber, value;

	printf("Usage: <command> <stack number>\n");
	
	while (1){
		printf("Enter command: ");
		if (scanf("%s%d", command, &stackNumber) != 2){
			printf("Usage: <command> <stack number>\n");
			fflush(stdin);
		}

		if (stackNumber != 1 && stackNumber != 2){
			printf("There's no stack with this number.\n");
			continue;
		}

		/* Команды */
		if (!strcmp(command, "push")){
			printf("Input a value: ");
			scanf("%d", &value);
			if (push(value, stackNumber)) printf("\nOops! Something went wrong.\n");
		}else if (!strcmp(command, "pop")){
			if (pop(stackNumber)) printf("\nStack %d is empty!\n", stackNumber);
		}else if (!strcmp(command, "top")){
			if (top(stackNumber)) printf("\nStack %d is empty!\n", stackNumber);
		}else if (!strcmp(command, "print")){
			print(stackNumber);
		}else if (!strcmp(command, "clear")){
			clear(stackNumber);
			printf("Stack now is clear\n");
		}else if (!strcmp(command, "exit")){
			break;
		}else{
			printf("\nUnknown command!\n");
		}

		for (int i = 0; i < MAX_LENGTH; i++) command[i] = '\0';
	}

	clear(1); 
	clear(2);
}

int push(int el, int stack_num){
	Stack_t *temp = NULL;

	if (stack_num == 1){

		/* Если в стеке нет ни одного элемента */
		if (!stackHead_1){
			stackHead_1 = (Stack_t*)malloc(sizeof(Stack_t));
			if (!stackHead_1) return NOT_OK;
			stackHead_1->value = el;
			stackHead_1->next = NULL;

			top_1 = stackHead_1;
			return OK;
		}

		temp = (Stack_t*)malloc(sizeof(Stack_t));
		if (!temp) return NOT_OK;

		temp->value = el;
		temp->next = top_1;
		top_1 = temp;
	}else{
		temp = top_2;

		if (!stackHead_2){
			stackHead_2 = (Stack_t*)malloc(sizeof(Stack_t));
			if (!stackHead_2) return NOT_OK;
			stackHead_2->value = el;
			stackHead_2->next = NULL;

			top_2 = stackHead_2;
			return OK;
		}

		temp = (Stack_t*)malloc(sizeof(Stack_t));
		if (!temp) return NOT_OK;

		temp->value = el;
		temp->next = top_2;
		top_2 = temp;
	}

	return OK;
}

int pop(int stack_num){
	Stack_t *temp;
	
	if (stack_num == 1){
		temp = top_1;
		if (!temp) return NOT_OK;
		top_1 = top_1->next;

		free(temp);
	}else{
		temp = top_2;
		if (!temp) return NOT_OK;
		top_2 = top_2->next;

		free(temp);
	}

	return OK;
}

int top(int stackNumber){
	if (stackNumber == 1){
		if (!top_1) return NOT_OK;
		printf("\nTop value of stack 1 is %d\n", top_1->value);
	}else{
		if (!top_2) return NOT_OK;
		printf("\nTop value of stack 2 is %d\n", top_2->value);
	}

	return OK;
}

int print(int stackNumber){
	Stack_t *temp;

	if (stackNumber == 1){
		for (temp = top_1; temp; temp = temp->next)
			printf("%d ", temp->value);
	}else{
		for (temp = top_2; temp; temp = temp->next)
			printf("%d ", temp->value);
	}

	printf("\n");
	
	return OK;
}

int clear(int stackNumber){
	Stack_t *temp;

	if (stackNumber == 1){
		for (temp = top_1; temp; temp = top_1){
			top_1 = top_1->next;
			free(temp);
		}
	}else{
		for (temp = top_2; temp; temp = top_2){
			top_2 = top_2->next;
			free(temp);
		}
	}

	return OK;
}
