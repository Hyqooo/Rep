#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
		// Splitters
		if (!CheckInput(sent[i]) && CheckInput(sent[i])){
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
		if (words[i] == NULL){
			printf("\nCan not allocate memory!\n");
			return;
		}

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

	// Go go go
	aWords = (struct Word*)malloc(wordCounter * sizeof(struct Word));

	// Вывод
	for (i = 0; i < wordCounter; i++){
		printf("%s\n", words[i]);
	}
}

// Check input
int CheckInput(char ch){
	if ((ch < 'A' || ch > 'Z') && (ch < 'a' || ch > 'z')){
		return 0;
	}else{
		return 1;
	}
}
