#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PATH "/test.txt"

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
int Write(char **, char);
void IsAllocated(void *);
void AddToBegin(List_t **, Word_t *);
void AddBet(List_t *, List_t *, Word_t *);
void DeleteSpec(List_t  **, List_t *);
void ClearMemory(List_t **);
struct List * AddToEnd(List_t *, Word_t *);

void main(){
	Word_t *aWords = NULL, **lastWord = NULL;
	List_t **listHead = NULL, *queueHead = NULL, *currentList = NULL;
	
	char *sent = NULL, **words = NULL, type;
	int i, j = 0, k = 0, wordCounter = 0, oldLen = 0, fAdded = 0, listsA = 0;

	printf("Choose type of input: f - file, k - keyboard\n");
	scanf("%c", &type);
	fflush(stdin);
	wordCounter = Write(&sent, type);


	// Записывает слова из предложения в новый массив.
	words = (char**)malloc(wordCounter * sizeof(char*));
	for (i = 0; i < wordCounter; i++){
		// Считает количество букв в слове.
		for (j += 1; sent[j - 1] != '\0'; j++);

		words[i] = (char*)malloc((j - oldLen) * sizeof(char));
		IsAllocated(words[i]);

		memcpy(*(words + i), sent + oldLen, j - oldLen);

		oldLen = j;
	}

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

		// Новый список
		if (queueHead != NULL && i != 0){
			k++;
			listHead = (List_t**)realloc(listHead, (k + 1) * sizeof(List_t*));
			IsAllocated(listHead);
			*(listHead + k) = NULL;
		}

		currentList = *(listHead + k);

		// Инициализируем голову списка
		if (*(listHead + k) == NULL){
			*(listHead + k) = (List_t*)calloc(1, sizeof(List_t));
			IsAllocated(*listHead + k);
			(*(listHead + k))->word_S = queueHead->word_S;
			currentList = *(listHead + k);

			DeleteSpec(&queueHead, queueHead);
			lastWord = &(queueHead->word_S);
			listsA++;
		}

		while (queueHead != NULL){

			fAdded = 0;

			if (queueHead->word_S->lLet == (*(listHead + k))->word_S->fLet){
				AddToBegin((listHead + k), queueHead->word_S);
				DeleteSpec(&queueHead, queueHead);
				
				lastWord = &(queueHead->word_S);
				fAdded = 1;
			}
			
			currentList = *(listHead + k);

			while (currentList->next != NULL){
				if (queueHead != NULL){
					if (queueHead->word_S->lLet == currentList->word_S->fLet &&
						queueHead->word_S->fLet == currentList->word_S->lLet){

						AddBet(currentList, currentList->next, queueHead->word_S);
						DeleteSpec(&queueHead, queueHead);

						lastWord = &(queueHead->word_S);
						fAdded = 1;
					}
				}
				currentList = currentList->next;
			}

			if (queueHead != NULL){
				if (queueHead->word_S->fLet == currentList->word_S->lLet){
					AddToEnd(*(listHead + k), queueHead->word_S);
					DeleteSpec(&queueHead, queueHead);
				
					lastWord = &(queueHead->word_S);
					fAdded = 1;
				}else{
					AddToEnd(queueHead, queueHead->word_S);
					DeleteSpec(&queueHead, queueHead);
				}
			}

			if (queueHead != NULL){
				if (*lastWord == (queueHead->word_S) && !fAdded) break;
			}
		}

	}

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


	// Освобождаем память.
	for (i = 0; i < listsA; i++){
		ClearMemory(listHead + i);
	}

	for (i = 0; i < wordCounter; i++){
		free(words[i]);
	}
	
	free(sent);
	free(words);
	free(aWords);
	free(listHead);

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

// Чистит память от списков.
void ClearMemory(List_t **head){
	List_t *current = *head;
	List_t *next;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
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
				if (current->next == NULL) break;
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

// Запись слов.
int Write(char **sent, char type){
	int i = 0, wordCounter = 0;

	if (type == 'f'){
		char c;
		FILE *fl = NULL;


		fl = fopen(PATH, "rt");

		if (fl == NULL){
			printf("\nFile cannot be opened!\n");
			exit(0);
		}

		c = fgetc(fl);
		(*sent) = (char *)malloc(sizeof(char));

		while (c != EOF){
			(*sent) = (char *)realloc((*sent), (i + 1) * sizeof(char));
			*(*sent + i) = c;
			c = fgetc(fl);

			if (*(*sent + i) == '.'){
				*(*sent + i) = '\0';
				wordCounter++;
				break;
			}

			// Разделители.
			if (!CheckInput(*(*sent + i)) && CheckInput(*(*sent + i - 1)) && ((i - 1) >= 0)){
				*(*sent + i) = '\0';
				wordCounter++;
			}
			i++;
		}

		*(*sent + i) = '\0';

		fclose(fl);
	}
	else if (type == 'k'){
		printf("Input sentence.\n");

		// Записываем слова в предложение.
		for (i = 0;; i++){
			(*sent) = (char*)realloc((*sent), (i + 1) * sizeof(char));
			scanf("%c", &*(*sent + i));
			if (*(*sent + i) == '.'){
				*(*sent + i) = '\0';
				wordCounter++;
				break;
			}

			if (!CheckInput(*(*sent + i)) && CheckInput(*(*sent + i - 1)) && ((i - 1) >= 0)){
				*(*sent + i) = '\0';
				wordCounter++;
			}
		}

	}
	else{
		printf("Undefined type of input!\n");
		exit(0);
	}

	return wordCounter;
}
