#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_NUMBER_LENGTH 6 + 1

int manage(FILE *, FILE *);
int parse(char[MAX_NUMBER_LENGTH]);
void reverse(char [MAX_NUMBER_LENGTH]);

int main(int argc, char *argv[]){
	FILE *fToRead = NULL, *fToWrite = NULL;
	
	if (!(fToRead = fopen(argv[1], "r")) || !(fToWrite = fopen(argv[2], "w")))
		return 1;

	if (manage(fToRead, fToWrite))
		printf("Unacceptable symbols!\n");
}

int manage(FILE *f2Read, FILE *f2Write){
	int result = 0;
	char number[MAX_NUMBER_LENGTH];

	while (fscanf(f2Read, "%s", number) != EOF){
		if (parse(number) == -1) return 1;
		fprintf(f2Write, "%s ", number);
	}
	fprintf(f2Write, "\n");
	fseek(f2Read, SEEK_SET, 0);

	while (fscanf(f2Read, "%s", number) != EOF){
		reverse(number);
		fprintf(f2Write, "%s ", number);
		result += parse(number);
	}
	fprintf(f2Write, "\n%d", result);

	return 0;
}

void reverse(char number[MAX_NUMBER_LENGTH]){
	char buff[MAX_NUMBER_LENGTH];
	int i, j, length;

	length = strlen(number);

	for (i = 0; i < length; i++){
		buff[i] = number[i];
	}

	for (i = length - 1, j = 0; i > -1; i--, j++){
		number[j] = buff[i];
	}
}

int parse(char number[MAX_NUMBER_LENGTH]){
	int i, length, result = 0;

	length = strlen(number);

	for (i = 0; i < length; i++){
		if (number[i] - '0' > 9) return -1;
		result += (int)((number[i] - '0') * pow(10, length - i - 1));
	}
	
	return result;
}
