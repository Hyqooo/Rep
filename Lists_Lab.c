#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define OK 0
#define NOT_OK 1;
#define MAX_LENGTH 15

typedef struct List{
	int size;
	char name[MAX_LENGTH];
	int date[3];
	struct List *next;
}list_t;

int sort();
int input();
int checkName(char *);
void print();
void clearList();
void deleteLessDate();
void deleteFile(list_t *);

list_t* insert(list_t *, list_t *, list_t *, list_t *);
int sizeComp(list_t *, list_t *);
int dateComp(list_t *, list_t *);
int nameComp(list_t *, list_t *);
int sortBy(int(*)(list_t *, list_t*));

list_t *first, *last;

void main(){
	char command[MAX_LENGTH];
	printf("Input command(input, sort, delete, print, quit)");
	while (1){
		printf("\n: ");
		scanf("%s", command);
		if (strcmp(command, "input") == 0){
			input();
			print();
		}else if (strcmp(command, "sort") == 0){
			sort();
			print();
		}else if (strcmp(command, "delete") == 0){
			deleteLessDate();
			print();
		}else if (strcmp(command, "print") == 0){
			print();
		}else if (strcmp(command, "quit") == 0){
			break;
		}
	}

	clearList();
}

int input(){
	int size, i;
	int date[3];
	char name[MAX_LENGTH];
	list_t *temp = NULL, *cur = last;
	
	printf("Usage: <name> <date of creation(dd.mm.yyyy)> <size>\nTo stop input quit\n");
	while (1){
		if(!scanf("%s", name)) return NOT_OK;
		if (strcmp(name, "quit") == 0) break;
		if (!scanf("%d.%d.%d", &date[0], &date[1], &date[2])) return NOT_OK;
		if (!scanf("%d", &size)) return NOT_OK;

		if (checkName(name)){
			printf("File with this name is already exists!\n");
			continue;
		}

		temp = (list_t*)malloc(sizeof(list_t));
		if (!temp){
			printf("File cannot be added\n");
			return OK;
		}else{
			printf("File added!\n");
		}
		
		strcpy(temp->name, name);
		for (i = 0; i < 3; i++)
			temp->date[i] = date[i];
		temp->size = size;
		temp->next = NULL;

		if (!first){
			first = temp;
			cur = first;
			last = cur;
		}else{
			cur->next = temp;
			cur = cur->next;
			last = cur;
		}
	}
	return OK;
}

int sort(){
	char command[MAX_LENGTH];
	if (!first) return NOT_OK;
	printf("Take a parameter to sort(name, date, size): ");
	scanf("%s", command);
	if (strcmp(command, "name") == 0){
		sortBy(nameComp);
	}else if (strcmp(command, "date") == 0){
		sortBy(dateComp);
	}else if (strcmp(command, "size") == 0){
		sortBy(sizeComp);
	}

	return OK;
}

int sortBy(int (*cmp)(list_t*, list_t*)){
	list_t *cur, *lastAdded = NULL, *start = first, *min, *prev;

	while (1){
		if (!start->next){
			last = start;
			return OK;
		}
		min = start;

		/* Находим минимальный элемент от старта */
		for (cur = start; cur; cur = cur->next){
			if ((*cmp) (cur, min) == 0){
				min = cur;
			}
		}

		prev = lastAdded;
		for (cur = start; cur; cur = cur->next){
			if (cur == min){
				lastAdded = insert(prev, cur->next, cur, lastAdded);
				start = lastAdded->next;
				break;
			}
			prev = cur;
		}
	}
}

list_t* insert(list_t *prev, list_t *next, list_t *element, list_t *lastAdded){
	list_t *temp;

	if (!prev) return element;

	if (!lastAdded){
		prev->next = next;
		element->next = first;
		first = element;
		return first;
	}

	prev->next = next;
	temp = lastAdded->next;
	lastAdded->next = element;
	element->next = temp;

	return element;
}

int nameComp(list_t *name, list_t *name_2_cmp){
	unsigned int length = strlen(name->name), i;

	if (length > strlen(name_2_cmp->name)) length = strlen(name_2_cmp->name);

	for (i = 0; i < length; i++){
		if (*(name->name + i) < *(name_2_cmp->name + i)){
			return OK;
		}
		else if (*(name->name + i) > *(name_2_cmp->name + i)){
			return NOT_OK;
		}
	}

	if (*(name->name + i) == '\0')
		return OK;
	else
		return NOT_OK;
}

int dateComp(list_t *date, list_t *date_2_cmp){
	if (date->date[2] < date_2_cmp->date[2])
		return OK;
	else if (date->date[2] != date_2_cmp->date[2])
		return NOT_OK;
	
	if (date->date[1] < date_2_cmp->date[1]) 
		return OK;
	else if (date->date[1] != date_2_cmp->date[1])
		return NOT_OK;

	if (date->date[0] < date_2_cmp->date[0]) 
		return OK;
	else if (date->date[0] != date_2_cmp->date[0])
		return NOT_OK;

	return NOT_OK;
}

int sizeComp(list_t *size, list_t *size_2_cmp){
	if (size->size < size_2_cmp->size) return OK;
	return NOT_OK;
}

void print(){
	list_t *cur;

	printf("\nName\t\tDate\t\tSize\n");
	if (!first) printf("List is empty!\n");
	for (cur = first; cur; cur = cur->next)
		printf("%s\t\t%d.%d.%d\t%d\n", cur->name, cur->date[0], cur->date[1], 
		cur->date[2], cur->size);
}

int checkName(char *name){
	list_t *cur;

	for (cur = first; cur; cur = cur->next){
		if (strcmp(name, cur->name) == 0) return NOT_OK;
	}
	return OK;
}

void deleteFile(list_t *el){
	list_t *cur, *temp = first;

	for (cur = first; cur; cur = cur->next){
		if (cur == el){
			temp->next = cur->next;
			if (el == first) first = cur->next;
			free(cur);
			for (cur = first; cur; cur = cur->next)
				last = cur;

			return;
		}
		temp = cur;
	}
}

void deleteLessDate(){
	list_t *cur, *temp, date;
	printf("\nInput date(dd.mm.yyyy): ");
	scanf("%d.%d.%d", &date.date[0], &date.date[1], &date.date[2]);
	for (cur = first; cur;){
		temp = cur->next;
		if (dateComp(cur, &date) == 0)
			deleteFile(cur);
		cur = temp;
	}
}

void clearList(){
	list_t *cur, *temp;

	for (cur = first; cur;){
		temp = cur;
		cur = cur->next;
		free(temp);
	}
	first = NULL;
}
