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
void AddBet(List_t *, List_t *, Word_t *);
void DeleteSpec(List_t  **, List_t *);
struct List * AddToEnd(List_t *, Word_t *);

void main(){
	Word_t *aWords = NULL, *curWord = NULL;
	List_t **listHead = NULL, *queueHead = NULL, *currentList = NULL, *curQueue = NULL;
	
	char *sent = NULL, **words = NULL;
	int i, j = 0, k = 0, wordCounter = 0, oldLen = 0, fAdded = 0, listsA = 0;

	// Записываем слова в предложение.
	for (i = 0;; i++){
		sent = (char*)realloc(sent, (i + 1) * sizeof(char));
		scanf("%c", &sent[i]);
		if (sent[i] == '.'){
			sent[i] = '\0';
			wordCounter++;
			break;
		}

		// Разделители.
		if (!CheckInput(sent[i]) && CheckInput(sent[i - 1]) && ((i - 1) >= 0)){
			sent[i] = '\0';
			wordCounter++;
		}
	}


	// Записывает слова из предложения в новый массив.
	words = (char**)malloc(wordCounter * sizeof(char*));
	for (i = 0; i < wordCounter; i++){
		// Считает количество букв в слове.
		for (j += 1; sent[j - 1] != '\0'; j++);
		// Выделяется память и копируется слово.
		words[i] = (char*)malloc((j - oldLen) * sizeof(char));
		IsAllocated(words[i]);

		memcpy(*(words + i), sent + oldLen, j - oldLen);

		oldLen = j;
	}

#pragma region List
	// Память для массива из структур Word.
	aWords = (Word_t*)malloc(wordCounter * sizeof(Word_t));
	IsAllocated(aWords);

	listHead = (List_t **)calloc(1, sizeof(List_t *));
	IsAllocated(listHead);
	*(listHead + k) = NULL;

	// Запись каждого слова. 
	for (i = 0; i < wordCounter; i++){
		(aWords + i)->word = *(words + i);
		(aWords + i)->fLet = **(words + i);
		// Последняя буква в слове.
		(aWords + i)->lLet = words[i][(strlen(*(words + i)) - 1)];

		queueHead = AddToEnd(queueHead, aWords + i);
	}

	for (i = 0; queueHead != NULL; i++){

		if (queueHead != NULL && i != 0){
			k++;
			listHead = (List_t**)realloc(listHead, (k + 1) * sizeof(List_t*));
			IsAllocated(listHead);
			*(listHead + k) = NULL;
		}

		curQueue = queueHead;
		currentList = *(listHead + k);

		// Инициализируем голову списка
		if (*(listHead + k) == NULL){
			*(listHead + k) = (List_t*)calloc(1, sizeof(List_t));
			IsAllocated(*listHead + k);
			(*(listHead + k))->word_S = curQueue->word_S;
			currentList = *(listHead + k);

			DeleteSpec(&queueHead, curQueue);
			curQueue = queueHead;
			listsA++;
		}
		/* 
			Исправить бесконечный цикл если остаются разные элементы
			Так же, возможно, появится проблема со вторым условием
		*/
		while (curQueue != NULL){
			if (currentList->word_S->fLet == curQueue->word_S->lLet){
				AddToBegin(&currentList, curQueue->word_S);
				DeleteSpec(&queueHead, curQueue);
			}
			else if (currentList->word_S->lLet == curQueue->word_S->fLet &&
				currentList->next->word_S->fLet == curQueue->word_S->lLet){
				if (currentList != NULL && currentList->next != NULL){
					AddBet(currentList, currentList->next, curQueue->word_S);
					DeleteSpec(&queueHead, curQueue);
				}
			}else if (currentList->word_S->lLet == curQueue->word_S->fLet){
				AddToEnd(currentList, curQueue->word_S);
				DeleteSpec(&queueHead, curQueue);
			}else{
				AddToEnd(queueHead, curQueue->word_S);
				DeleteSpec(&queueHead, curQueue);
			}
			
			curQueue = curQueue->next;
		}
		// Continue...
	}

	
#pragma endregion

	// Вывод.
	for (i = 0; i < listsA; i++){
		printf("\nList #%d: ", i + 1);
		printf("%s ", (*(listHead + i))->word_S->word);
		currentList = (*(listHead + i));
		while (currentList->next != NULL){
			printf("%s ", currentList->next->word_S->word);
			currentList = currentList->next;
		}
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

void DeleteSpec(List_t **head, List_t *del_el){
	List_t *t = del_el;
	List_t *current = *head;
	if (t == *head){
		*head = t->next;
		del_el = NULL;
		free(del_el);
	}else{
		if (current->next != NULL){
			while (current->next != del_el){
				current = current->next;
			}
		}
		if (!(t->next)){
			current->next->next = NULL;
		}
		else{
			current->next = del_el->next;
		}
		t = NULL;
		free(t);
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
