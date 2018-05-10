#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 32

int createVocabulary(FILE *, FILE *);
int printText(FILE *, FILE *);

void main(int argc, char *argv[]){
	FILE *f_1 = NULL, *f_2 = NULL;

	f_1 = fopen(argv[1], "r");
	f_2 = fopen(argv[2], "w+");
	if (!f_1 || !f_2){
		printf("Cannot open the file!\n");
		return;
	}

	createVocabulary(f_1, f_2);
	fclose(f_1);
	fclose(f_2);
	
	f_1 = f_2 = NULL;
	f_1 = fopen(argv[2], "r+");
	f_2 = fopen(argv[3], "w+");
	if (!f_1 || !f_2){
		printf("Cannot open the file!\n");
		return;
	}

	printText(f_1, f_2);

	fclose(f_1);
	fclose(f_2);
}

int createVocabulary(FILE *f2Read, FILE *f2Write){
	char tempWord[MAX_WORD_LENGTH], word2cmp[MAX_WORD_LENGTH];
	int count = 0, pos;

	while (fscanf(f2Read, "%s", tempWord) != EOF){
		fseek(f2Write, SEEK_SET, 0);
		
		while (1){
			pos = ftell(f2Write);
			if (fscanf(f2Write, "%s", word2cmp) == EOF){
				fprintf(f2Write, "\n%s %d", tempWord, 1);
				break;
			}else{
				fscanf(f2Write, "%d", &count);
			}

			if (strcmp(word2cmp, tempWord) == 0){
				fseek(f2Write, pos, SEEK_SET);
				fprintf(f2Write, "\n%s %d", tempWord, count + 1);
				break;
			}
		}
	}

	return 0;
}

int printText(FILE *f2Read, FILE *f2Write){
	char tempWord[MAX_WORD_LENGTH];
	int count;

	while (fscanf(f2Read, "%s%*d", tempWord) != EOF)
		fprintf(f2Write, "%s ", tempWord);

	fseek(f2Read, SEEK_SET, 0);

	while (fscanf(f2Read, "%s%d", tempWord, &count) != EOF){
		if (count != 1){
			fprintf(f2Write, "\n%s - %d", tempWord, count);
		}
	}

	return 0;
}
