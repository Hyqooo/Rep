#include <stdio.h>
#include <stdlib.h>

typedef struct List{
	int val;
	struct List *next;
}List;

void Delete(List **, List *);

void main(){
	List *first = NULL, *cur = NULL, *prev = NULL;
	int value, a, b;
	char c;


	/* Концы интервала. */
	scanf("%i", &a);
	scanf("%i", &b);
	
	if (cur == NULL){
		cur = (List *)malloc(sizeof(List));
		if (cur == NULL) return;
		scanf("%i", &value);
		cur->val = value;
		cur->next = NULL;
	}

	first = cur;

	while (1){
		cur->next = (List *)malloc(sizeof(List));
		if (cur->next == NULL) return;
		scanf("%i%c", &value, &c);
		cur->next->val = value;
		cur->next->next = NULL;

		if (c == '.') break;

		cur = cur->next;
	}

	cur = first;

	/* Идем по списку ищем неподходящие элементы */
	while (cur != NULL){
		if (cur->val < a || cur->val > b){
			Delete(&first, cur);
			cur = first;
		}else{
			cur = cur->next;
		}
	}

	cur = first;
	
	while (cur != NULL){
		printf("%i ", cur->val);
		cur = cur->next;
	}
	
	printf("\n");
}

void Delete(List **first, List *del){
	List *t = *first;

	if (*first == del){
		*first = (*first)->next;
	}else{
		// Дошли до предыдущего у удаляемого элемента. 
		while (t->next != del){
			t = t->next;
		}

		if (del->next != NULL){
			t->next = del->next;
		}else{
			t->next = NULL;
		}
	}

	free(del);
}


