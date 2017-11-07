#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
	Че, пацаны, ониме?
*/

// Информация о слове.
typedef struct Word{
	char *word;
	char fLet;
	char lLet;
}Word_t;

// Список слов.
typedef struct List{
	struct Word *word;
	struct List *next;
}List_t;

// Слова, ожидающие своей очереди.
typedef struct Queue{
	struct Word *word;
	struct Queue *next;
}Queue_t;

// Прототипы.
int CheckInput(char);
void IsAllocated(void *);
void AddToEnd_Queue(Queue_t *, Word_t *);
struct List * AddToBegin_List(List_t *, Word_t *);
struct List * AddToEnd_List(List_t *, Word_t *);
void * AddBet_List(List_t *, List_t *, Word_t *);

void main(){
	Word_t *aWords = NULL;
	List_t *listHead = NULL;
	Queue_t *queueHead = NULL;

	char *sent = NULL, **words = NULL;
	int i, j = 0, wordCounter = 0, oldLen = 0;

	
	// Записываем слова в предложение
	for (i = 0;; i++){
		sent = (char*)realloc(sent, (i + 1) * sizeof(char));
		scanf("%c", &sent[i]);
		if (sent[i] == '.'){
			sent[i] = '\0';
			wordCounter++;
			break;
		}

		// Разделители
		if (!CheckInput(sent[i]) && CheckInput(sent[i - 1]) && ((i - 1) >= 0)){
			sent[i] = '\0';
			wordCounter++;
		}
	}


	// Записывает слова из предложения в новый массив
	words = (char**)malloc(wordCounter * sizeof(char*));
	for (i = 0; i < wordCounter; i++){
		// Считает количество букв в слове
		for (j += 1; sent[j - 1] != '\0'; j++);
		// Выделяется память и копируется слово
		words[i] = (char*)malloc((j - oldLen) * sizeof(char));
		IsAllocated(words[i]);

		memcpy(*(words + i), sent + oldLen, j - oldLen);

		oldLen = j;
	}

#pragma region List
	// Память для массива из структур Word.
	aWords = (Word_t*)malloc(wordCounter * sizeof(Word_t));
	IsAllocated(aWords);
	
	// Запись каждого слова. 
	for (i = 0; i < wordCounter; i++){
		(aWords + i)->word = *(words + i);
		(aWords + i)->fLet = **(words + i);
		// Последняя буква в слове.
		(aWords + i)->lLet = words[i][(strlen(*(words + i)) - 1)];
	}

/*	for (i = 0; i < wordCounter; i++){
		// Первый элемент списка слов.
		if (listHead == NULL){
			listHead = (struct List*)malloc(sizeof(struct List));
			IsAllocated(listHead);
			
			listHead->word = aWords;
			listHead->next = NULL;
		}else{
			if ((aWords + i)->lLet == listHead->word->fLet){
				// Проверяем первую букву.
				AddToBeginList(listHead, aWords);
			}else if ((aWords + i)->fLet == listHead->word->lLet){
				// Проверяем последнюю букву.
				AddToEndList(listHead, aWords + i);
			}else{
				// Слово отправляется ждать.
				if (queueHead == NULL){
					queueHead = (struct Queue*)malloc(sizeof(struct Queue));
					IsAllocated(queueHead);

					queueHead->word = (aWords + i);
					queueHead->next = NULL;
				}else{
					AddToEndQueue(queueHead, aWords + i);
				}
			}
		}
	}
*/

	

#pragma endregion

	// Вывод
	printf("\nList: ");
	while (listHead != NULL){
		printf("%s ", listHead->word->word);
		listHead = listHead->next;
	}

	printf("\nQueue: ");
	while (queueHead != NULL){
		printf("%s ", queueHead->word->word);
		queueHead = queueHead->next;
	}
	printf("\n");

#pragma region FreeMemory
	for (i = 0; i < wordCounter; i++){
		free(words[i]);
	}
	
	free(sent);
	free(words);
#pragma endregion
}

// Проверка ввода.
int CheckInput(char ch){
	if ((ch < 'A' || ch > 'Z') && (ch < 'a' || ch > 'z')){
		return 0;
	}else{
		return 1;
	}
}

// Проверяет была ли выделена память.
void IsAllocated(void *mem){
	if (mem == NULL){
		printf("\nCan not allocate memory!\n");
		exit(0);
	}
}

// Добавление между элементами.
void * AddBet_List(List_t *prev, List_t *next, Word_t *word){
	List_t *new_el = (List_t*)malloc(sizeof(List_t));
	IsAllocated(new_el);

	prev->next = new_el;
	new_el->word = word;
	new_el->next = next;

	return prev;
}

// Добавление слова в конец списка.
List_t * AddToEnd_List(List_t *head, Word_t *word){
	List_t *current = head;

	if (current == NULL){
		current = (List_t *)malloc(sizeof(List_t));
		IsAllocated(current);
		current->word = word;
		current->next = NULL;

		return current;
	}

	while (current->next != NULL){
		current = current->next;
	}

	current->next = (List_t*)malloc(sizeof(List_t));
	IsAllocated(current->next);

	current->next->word = word;
	current->next->next = NULL;

	return head;
}

// Добавление слова в начало списка.
List_t * AddToBegin_List(List_t *head, Word_t *word){
	List_t *new_el = (List_t*)malloc(sizeof(List_t));
	IsAllocated(new_el);

	new_el->word = word;
	new_el->next = head;
	head = new_el;

	return new_el;
}

// Добавление слова в очередь ожидающих.
void AddToEnd_Queue(Queue_t *head, Word_t *word){
	Queue_t *current = head;
	while (current->next != NULL){
		current = current->next;
	}

	current->next = (Queue_t*)malloc(sizeof(Queue_t));
	IsAllocated(current->next);

	current->next->word = word;
	current->next->next = NULL;
}
