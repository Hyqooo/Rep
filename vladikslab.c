#include <stdio.h>
#include <stdlib.h>

typedef struct List{
	int val;
	struct List *next;
}List;

void AddItem(int);

void main(){
	List *first = NULL, *cur = NULL;
	int value;
	char c;
	
	if (cur == NULL){
		cur = (List *)malloc(sizeof(List));
		scanf("%i", &value);
		cur->val = value;
		cur->next = NULL;
	}

	first = cur;

	while (1){
		cur->next = (List *)malloc(sizeof(List));
		scanf("%i%c", &value, &c);
		cur->next->val = value;
		cur->next->next = NULL;

		if (c == '.') break;

		cur = cur->next;
	}

	cur = first;

	while (cur != NULL){
		printf("%i ", cur->val);
		cur = cur->next;
	}
	printf("\n");
}

