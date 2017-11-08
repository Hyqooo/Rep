#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
void AddToBegin(List_t **, Word_t *);
struct List * AddToEnd(List_t *, Word_t *);
void AddBet(List_t *, List_t *, Word_t *);
void AddFromQ(List_t *, List_t *, List_t *);

void main(){
	Word_t *aWords = NULL;
	List_t *listHead = NULL, *queueHead = NULL, *currentList = NULL;
	
	char *sent = NULL, **words = NULL;
	int i, j = 0, wordCounter = 0, oldLen = 0, fAdded = 0;

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

	for (i = 0; i < wordCounter; i++){

		fAdded = 0;

		// Инициализируем голову списка.
		if (listHead == NULL){
			listHead = AddToEnd(listHead, aWords);
			currentList = (List_t *)malloc(sizeof(List_t));
			IsAllocated(currentList);
			currentList = listHead;
			continue;
		}

		while (currentList->next != NULL){
			if ((aWords + i)->fLet == currentList->word_S->lLet &&
				(aWords + i)->lLet == currentList->next->word_S->fLet){

				AddBet(currentList, currentList->next, aWords + i);
				AddFromQ(listHead, listHead->next, queueHead);
				currentList = currentList->next;
				fAdded = 1;
				break;
			}

			currentList = currentList->next;
		}

		if (fAdded) continue;

		if ((aWords + i)->fLet == currentList->word_S->lLet){
			AddToEnd(listHead, aWords + i);
			AddFromQ(listHead, listHead->next, queueHead);
		}else if ((aWords + i)->lLet == currentList->word_S->fLet){
			AddToBegin(&listHead, aWords + i);
			AddFromQ(listHead, listHead->next, queueHead);
		}else{
			if (queueHead == NULL){
				queueHead = AddToEnd(queueHead, aWords + i);
				continue;
			}

			AddToEnd(queueHead, aWords + i);
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

// Добавление из очереди.
void AddFromQ(List_t *first, List_t *second, List_t *queueHead){
	List_t *currentQ = queueHead;
	List_t *currentP = first;
	List_t *currentN = second;

	while (currentN->next != NULL){
		if (currentQ->word_S->fLet == currentP->word_S->lLet &&
			currentQ->word_S->lLet == currentN->word_S->fLet){
			
			AddBet(currentP, currentN, currentQ->word_S);
			currentQ = currentQ->next;
		}else if (currentQ != NULL){
			currentQ = queueHead;
			currentP = currentN;
			currentN = currentN->next;
		}
	}

	if (currentQ->word_S->fLet == currentN->word_S->lLet){
		AddToEnd(first, queueHead->word_S);
	}else if (currentQ->word_S->lLet == first->word_S->fLet){
		AddToBegin(&first, queueHead->word_S);
	}
}

// Добавление между элементами.
void AddBet(List_t *prev, List_t *next, Word_t *word){
	List_t *new_el = (List_t*)malloc(sizeof(List_t));
	IsAllocated(new_el);

	prev->next = new_el;
	new_el->word_S = word;
	new_el->next = next;
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
void AddToBegin(List_t **head, Word_t *word){
	List_t *new_el = (List_t*)malloc(sizeof(List_t));
	IsAllocated(new_el);

	new_el->word_S = word;
	new_el->next = *head;
	*head = new_el;
}
