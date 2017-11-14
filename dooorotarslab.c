#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define PATH "/test.txt"

typedef struct List{
	char *info; 
	struct List *next;
}List;

void Print(const List *);
void IsAllocated(void *);
int Write(char **, char);
int CheckInput(char);

void main(){
	char *sent = NULL, *scheme = NULL, c, type, **words = NULL;
	List *list = NULL;
	int i, j = 0, wordCounter = 0, oldLen = 0;


	/* Вводим шаблон */
	for (i = 0;; i++){
		scheme = (char*)realloc(scheme, (i + 1) * (sizeof(char)));
		scanf("%c", &c);
		*(scheme + i) = c;
		if (c == '.') break;
	}

	fflush(stdin);
	printf("\nChoose type of input: f - file, k - keyboard\n");
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
}

// Проверяет была ли выделена память.
void IsAllocated(void *mem){
	if (mem == NULL){
		printf("\nCan not allocate memory!\n");
		exit(0);
	}
}

void Print(const List *head) {
	while (head) {
		printf("%s ", head->info);
		head = head->next;
	}
	printf("\n");
}

// Проверка ввода.
int CheckInput(char ch){
	if ((ch < 'A' || ch > 'Z') && (ch < 'a' || ch > 'z')){
		return 0;
	}
	else{
		return 1;
	}
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
