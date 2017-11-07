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

// Список.
typedef struct List{
	struct Word *word_S;
	struct List *next;
}List_t;

// Прототипы.
int CheckInput(char);
void IsAllocated(void *);
struct List * AddToBegin(List_t *, Word_t *);
struct List * AddToEnd(List_t *, Word_t *);
void * AddBet(List_t *, List_t *, Word_t *);
void * AddFromQueue(List_t *, List_t *, List_t *, enum Mod);

enum Mod { Both, Right, Left };

void main(){
	Word_t *aWords = NULL;
	List_t *listHead = NULL, *queueHead = NULL, *currentList = NULL;
	
	char *sent = NULL, **words = NULL, *current = NULL;
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

	for(i = 0;; i++){

		// Инициализируем голову списка.
		if (listHead == NULL){
			listHead = AddToEnd(listHead, aWords);
			currentList = (List_t *)malloc(sizeof(List_t));
			IsAllocated(currentList);
			currentList = listHead;
			continue;
		}
		
		// Смотрим возможность добавления между словами.
		while (currentList->next != NULL){
			if ((aWords + i)->fLet == currentList->word_S->lLet && 
				(aWords + i)->lLet == currentList->next->word_S->fLet){
				AddBet(currentList, currentList->next, aWords + i);
				// Проверяем возможность добавить слово из зала.
				AddFromQueue(currentList, currentList->next, queueHead, Both);
			}

			currentList = currentList->next;
		}

		if ((aWords + i)->fLet == currentList->word_S->lLet){
			// В конец списка.
			AddToEnd(listHead, (aWords + i));

			// Проверяем возможность добавить слово из зала.
			AddFromQueue(listHead, listHead->next, queueHead, Right);

		}else if ((aWords + i)->lLet == listHead->word_S->fLet){
			// В начало списка.
			listHead = AddToBegin(listHead, (aWords + i));

			// Проверяем возможность добавить слово из зала.
			AddFromQueue(listHead, listHead->next, queueHead, Right);

		}else{
			// Инициализируем голову зала ожидания.
			if (queueHead == NULL){
				queueHead = AddToEnd(queueHead, (aWords + i));
				continue;
			}

			// В зал ожидания.
			AddToEnd(queueHead, (aWords + i));
		}
	}

#pragma endregion

	// Вывод
	printf("\nList: ");
	while (listHead != NULL){
		printf("%s ", listHead->word_S->word);
		listHead = listHead->next;
	}

	printf("\nQueue: ");
	while (queueHead != NULL){
		printf("%s ", queueHead->word_S->word);
		queueHead = queueHead->next;
	}
	printf("\n");

	for (i = 0; i < wordCounter; i++){
		free(words[i]);
	}
	
	free(sent);
	free(words);

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

// Добавляет слова из очереди
void * AddFromQueue(List_t *prev, List_t *next, List_t *queueHead, enum Mod mode){
	
	while (queueHead != NULL){
		switch (mode){
		case Both:
			// Проверяет с обеих сторон.
			if (queueHead->word_S->fLet == prev->word_S->lLet &&
				queueHead->word_S->lLet == next->word_S->fLet)
			{
				AddBet(prev, next, queueHead->word_S);
			}
			queueHead = queueHead->next;
			break;

		case Right:
			// Только крайнее правое слово.
			if (queueHead->word_S->fLet == prev->word_S->lLet){
				AddToEnd(prev, queueHead->word_S);
			}
			queueHead = queueHead->next;
			break;

		case Left:
			// Крайнее левое слово.
			if (queueHead->word_S->lLet == prev->word_S->fLet){
				AddToBegin(next, queueHead->word_S);
			}
			queueHead = queueHead->next;
			break;
		}
	}

	return queueHead;
}

// Добавление между элементами.
void * AddBet(List_t *prev, List_t *next, Word_t *word){
	List_t *new_el = (List_t*)malloc(sizeof(List_t));
	IsAllocated(new_el);

	prev->next = new_el;
	new_el->word_S = word;
	new_el->next = next;

	return prev;
}

// Добавление слова в конец списка.
List_t * AddToEnd(List_t *head, Word_t *word){
	List_t *current = head;

	if (current == NULL){
		current = (List_t *)malloc(sizeof(List_t));
		IsAllocated(current);

		current->word_S = word;
		current->next = NULL;

		return current;
	}

	while (current->next != NULL){
		current = current->next;
	}

	current->next = (List_t*)malloc(sizeof(List_t));
	IsAllocated(current->next);

	current->next->word_S = word;
	current->next->next = NULL;

	return head;
}

// Добавление слова в начало списка.
List_t * AddToBegin(List_t *head, Word_t *word){
	List_t *new_el = (List_t*)malloc(sizeof(List_t));
	IsAllocated(new_el);

	new_el->word_S = word;
	new_el->next = head;
	head = new_el;

	return new_el;
}
