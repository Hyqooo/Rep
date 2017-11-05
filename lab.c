#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Prototypes
int CheckInput(char);
int IsAllocated(void *);

// Info about word
struct Word{
	char *word;
	char fLet;
	char lLet;
};

// List of words.
struct List{
	struct Word *word;
	struct List *next;
};

// Queue of waiting words.
struct Queue{
	struct Word *word;
	struct Queue *next;
};

void main(){
	struct Word *aWords = NULL;
	struct List *pFirstL = NULL;
	struct Queue *pQ = NULL;
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

		// SpFirstLitters
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
		if (!IsAllocated(words[i])) 
			return;

		memcpy(*(words + i), sent + oldLen, j - oldLen);

		oldLen = j;
	}



	/*
		Вот какая идея - использовать дополнительный список, названный очередью.
		Пытаемся включить слово в список, если слово подходит добавляем в список,
		если нет добавляем его в очередь, следующее слово пытаемся добавить, если попадает в
		список - проверяем слово в очереди подходит или нет, если нет то в очередь и так далее, 
		когда слова закончились, но в очереди остались слова берем первое слово и повторяем
		процедуру.

		Замечание: очередное слово проверяет в голове первую, но не смотрит на предыдущее -
		у конца наоборот.
	*/

#pragma region List
	// Allocate memory for array of struct words.
	aWords = (struct Word*)malloc(wordCounter * sizeof(struct Word));

	if (!IsAllocated(aWords))
		return;
	
	// Setting the each word. 
	for (i = 0; i < wordCounter; i++){
		(aWords + i)->word = *(words + i);
		(aWords + i)->fLet = **(words + i);
		(aWords + i)->lLet = words[i][(strlen(*(words + i)) - 1)];
	}

	for (i = 0; i < wordCounter; i++){
		// Setting head word of List.
		if (pFirstL == NULL){
			pFirstL = (struct List*)malloc(sizeof(struct List));
			if (!IsAllocated(pFirstL))
				return;
			
			pFirstL->word = (aWords + i);
			pFirstL->next = NULL;
		}else{
			// Check the start letter.
			if ((aWords + i)->lLet == pFirstL->word->fLet){
				pFirstL->word = (aWords + i);
			}
			// Check the last letter.
//			if (){

//			}
		}


	}

#pragma endregion

	// Вывод
	for (i = 0; i < wordCounter; i++){
		printf("%s\n", (aWords + i)->word);
		printf("%c\n", (aWords + i)->fLet);
		printf("%c\n", (aWords + i)->lLet);
	}
}

// Check input.
int CheckInput(char ch){
	if ((ch < 'A' || ch > 'Z') && (ch < 'a' || ch > 'z')){
		return 0;
	}else{
		return 1;
	}
}

int IsAllocated(void *mem){
	if (mem == NULL){
		printf("\nCan not allocate memory!\n");
		return 0;
	}
	return 1;
}


// Add item to list.
void AddToList(){

}
