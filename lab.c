#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Прототипы.
int CheckInput(char);
void IsAllocated(void *);
void AddToEndList(struct List *, struct Word *);
void AddToEndQueue(struct Queue *, struct Word *);
struct List * AddToBeginList(struct List *, struct Word *);

// Информация о слове.
struct Word{
	char *word;
	char fLet;
	char lLet;
};

// Список слов.
struct List{
	struct Word *word;
	struct List *next;
};

// Слова, ожидающие своей очереди.
struct Queue{
	struct Word *word;
	struct Queue *next;
};

void main(){
	struct Word *aWords = NULL;
	struct List *listHead = NULL;
	struct Queue *queueHead = NULL;
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
	aWords = (struct Word*)malloc(wordCounter * sizeof(struct Word));

	IsAllocated(aWords);
	
	// Запись каждого слова. 
	for (i = 0; i < wordCounter; i++){
		(aWords + i)->word = *(words + i);
		(aWords + i)->fLet = **(words + i);
		// Последняя буква в слове.
		(aWords + i)->lLet = words[i][(strlen(*(words + i)) - 1)];
	}

	for (i = 0; i < wordCounter; i++){
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

// Была ли выделена память.
void IsAllocated(void *mem){
	if (mem == NULL){
		printf("\nCan not allocate memory!\n");
		exit(0);
	}
}

// Добавление слова в конец списка.
void AddToEndList(struct List *head, struct Word *word){
	struct List *current = head;
	while (current->next != NULL){
		current = current->next;
	}

	current->next = (struct List *)malloc(sizeof(struct List));
	IsAllocated(current->next);

	current->next->word = word;
	current->next->next = NULL;
}

// Добавление слова в начало списка.
struct List * AddToBeginList(struct List *head, struct Word *word){
	struct List *new_el = (struct List*)malloc(sizeof(struct List));
	IsAllocated(new_el);

	new_el->word = word;
	new_el->next = head;
	head = new_el;

	return new_el;
}

// Добавление слова в очередь ожидающих.
void AddToEndQueue(struct Queue *head, struct Word *word){
	struct Queue *current = head;
	while (current->next != NULL){
		current = current->next;
	}

	current->next = (struct Queue*)malloc(sizeof(struct Queue));
	IsAllocated(current->next);

	current->next->word = word;
	current->next->next = NULL;
}
