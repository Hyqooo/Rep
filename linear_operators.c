#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct RationalFraction{
	int numerator;
	int denominator;
};

typedef struct RationalFraction ratio_t;

// Прототипы
int Sign(int);
int NumberGcd(int, int);

void init(int *, int);
void nextPerm(int *, int, int);
void matrixInput(FILE *);
void printMatrix(int, ratio_t **);
void makeCharPolynomial();
void errorHandler(int);

int choose(int, int);

ratio_t RecursiveLaplace(ratio_t **, int);
ratio_t **GetAMinor(ratio_t **, int, int, int);

// Глобальные перменные
int matrixSize;
ratio_t **inputMatrix, *charPolynomial;

int main(int agrc, char *argv[]){
	FILE *f = NULL;
	ratio_t res;

	f = fopen(argv[1], "r");
	if (!f) errorHandler(1);
	matrixInput(f);
	fclose(f);

	printMatrix(matrixSize, inputMatrix);
	/*
	int *seq = (int*)malloc(matrixSize * sizeof(int));
	init(seq, 2);

	ratio_t** minorMatrix = (ratio_t**)malloc(matrixSize * sizeof(ratio_t));
	if (!minorMatrix) errorHandler(0);
	for (int i = 0; i < matrixSize; i++){
		minorMatrix[i] = (ratio_t*)malloc(matrixSize * sizeof(ratio_t));
		if (!minorMatrix[i]) errorHandler(0);
	}

	for (int i = 0; i < choose(matrixSize, 2); i++){
		for (int j = 0; j < 2; j++)
			printf("%d ", seq[j]);
		printf("\n%d:\n", i);
		chooseMatrix(minorMatrix, seq, matrixSize);
		printMatrix(2, minorMatrix);
		printf("\n");
		nextPerm(seq, 2, matrixSize);
	}
	*/

	makeCharPolynomial();
}

void makeCharPolynomial(){
	int i, j, polPower, temp, *seq = NULL;
	ratio_t sum, result, **minorMatrix = NULL;

	charPolynomial = (ratio_t*)malloc((matrixSize + 1) * sizeof(ratio_t));
	if (!charPolynomial) errorHandler(0);

	charPolynomial[0].numerator = -1;
	charPolynomial[0].denominator = 1;

	seq = (int*)malloc(matrixSize * sizeof(int));
	if (!seq) errorHandler(0);

	minorMatrix = (ratio_t**)malloc(matrixSize * sizeof(ratio_t));
	if (!minorMatrix) errorHandler(0);
	for (i = 0; i < matrixSize; i++){
		minorMatrix[i] = (ratio_t*)malloc(matrixSize * sizeof(ratio_t));
		if (!minorMatrix[i]) errorHandler(0);
	}

	for (polPower = 1; polPower < matrixSize + 1; polPower++){
		init(seq, polPower);

		sum.numerator = 0;
		sum.denominator = 1;
		for (i = 0; i < choose(matrixSize, polPower); i++){
			// Выбираем минор по перестановки и находим определитель
			chooseMatrix(minorMatrix, seq, matrixSize);
			result = RecursiveLaplace(minorMatrix, polPower);
			
			if (result.denominator != sum.denominator){
				temp = result.denominator;
				result.numerator *= sum.denominator;
				result.denominator *= sum.denominator;
				sum.numerator *= temp;
				sum.denominator *= temp;
			}

			sum.numerator += result.numerator;

			// Сокращаем числитель и знаменатель
			temp = NumberGcd(abs(sum.numerator), abs(sum.denominator));
			sum.numerator /= temp;
			sum.denominator /= temp;
			
			nextPerm(seq, polPower, matrixSize);
		}
		if ((matrixSize - polPower) % 2 != 0){
			sum.numerator = -sum.numerator;
		}

		charPolynomial[polPower] = sum;
	}

	// Вывод многочлена
	for (i = 0; i < matrixSize + 1; i++){
		printf("%+d/%dx^%d", charPolynomial[i].numerator, charPolynomial[i].denominator, matrixSize - i);
	}
}

// Выбирает главный минор
int chooseMatrix(ratio_t **matrix, int *seq, int size){
	int i, j, k, j_, k_;

	for (j = 0, j_ = 0; j < size; j++, j_++){
		if (j != seq[j_]){
			--j_;
			continue;
		}
		for (k = 0, k_ = 0; k < size; k++, k_++){
			if (k != seq[k_]){
				--k_;
				continue;
			}

			matrix[j_][k_] = inputMatrix[j][k];
		}
	}
}

// Число сочетаний из n по k
int choose(int n, int k) {
	int r = 1;

	for (int d = 1; d <= k; ++d) {
		r *= n--;
		r /= d;
	}
	return r;
}

void init(int *seq, int size){
	for (int i = 0; i < size; i++)
		seq[i] = i;
}

void nextPerm(int *seq, int	sizeSelect, int size){
	int i, j, k;

	if (seq[sizeSelect - 1] != size - 1){
		seq[sizeSelect - 1] += 1;
	}else{
		for (i = sizeSelect - 2; i > -1;){
			if (seq[i] != size - sizeSelect + i){
				seq[i] += 1;
				for (k = seq[i] + 1, j = i + 1; j < sizeSelect; j++, k++){
					seq[j] = k;
				}
				break;
			}else{
				i--;
			}
		}
	}
}

#pragma region INPUT/OUTPUT AND UTILITY

void matrixInput(FILE *f){
	int i, j;
	char c;

	fscanf(f, "%d", &matrixSize);

	inputMatrix = (ratio_t**)malloc(matrixSize * sizeof(ratio_t*));
	if (!inputMatrix) errorHandler(0);
	for (i = 0; i < matrixSize; i++){
		inputMatrix[i] = (ratio_t*)malloc(matrixSize * sizeof(ratio_t));
		if (!inputMatrix[i]) errorHandler(0);
	}

	for (i = 0; i < matrixSize; i++){
		for (j = 0; j < matrixSize; j++){
			fscanf(f, "%d", &inputMatrix[i][j].numerator);
			c = fgetc(f);
			if (c == '/')
				fscanf(f, "%d", &inputMatrix[i][j].denominator);
			else
				inputMatrix[i][j].denominator = 1;
		}
	}
}

void printMatrix(int size, ratio_t **matrix){
	int i, j;

	for (i = 0; i < size; i++){
		for (j = 0; j < size; j++){
			if (matrix[i][j].denominator != 1){
				printf("%d/%d ", matrix[i][j].numerator, matrix[i][j].denominator);
			}else{
				printf("%d ", matrix[i][j].numerator);
			}
		}
		printf("\n");
	}
}

void errorHandler(int errorCode){
	switch (errorCode){
	case 0:
		printf("There's no memory here!\n");
		exit(1);
		break;
	case 1:
		printf("Cannot open the file!\n");
		exit(1);
		break;
	
	default:
		printf("Unhandled error!\n");
		exit(1);
		break;
	}
}

#pragma endregion

#pragma region DETERMINANT

ratio_t RecursiveLaplace(ratio_t **A, int size){
	int j, temp;
	ratio_t result, firstTerm, secondTerm, retResult;

	result.denominator = 1;
	result.numerator = 0;

	if (size == 1){
		return A[0][0];
	}

	if (size == 2){
		firstTerm.numerator = A[0][0].numerator * A[1][1].numerator;
		firstTerm.denominator = A[0][0].denominator * A[1][1].denominator;
		secondTerm.numerator = A[1][0].numerator * A[0][1].numerator;
		secondTerm.denominator = A[1][0].denominator * A[0][1].denominator;
		
		/* Можно вынести в функцию */
		if (firstTerm.denominator != secondTerm.denominator){
			temp = firstTerm.denominator;
			firstTerm.numerator *= secondTerm.denominator;
			firstTerm.denominator *= secondTerm.denominator;
			secondTerm.numerator *= temp;
			secondTerm.denominator *= temp;
		}

		result.numerator = firstTerm.numerator - secondTerm.numerator;
		result.denominator = firstTerm.denominator;

		temp = NumberGcd(abs(result.numerator), abs(result.denominator));
		result.numerator /= temp;
		result.denominator /= temp;

		return result;
	}

	for (j = 0; j < size; j++){
		if (A[0][j].numerator == 0) continue;

		retResult = RecursiveLaplace(GetAMinor(A, 0, j, size), size - 1);
		retResult.numerator *= A[0][j].numerator;
		retResult.denominator *= A[0][j].denominator;
	
		if (retResult.denominator != result.denominator){
			temp = retResult.denominator;
			retResult.numerator *= result.denominator;
			retResult.denominator *= result.denominator;
			result.numerator *= temp;
			result.denominator *= temp;
		}
		
		result.numerator += Sign(j) * retResult.numerator;
	}

	return result;
}

int Sign(int power){
	if (power % 2 == 0){
		return 1;
	}
	else{
		return -1;
	}
}

ratio_t** GetAMinor(ratio_t **A, int str, int column, int size){
	int i, j, i_, j_;
	ratio_t **B = NULL;

	B = (ratio_t **)malloc((size - 1) * sizeof(ratio_t*));
	if (!B) errorHandler(0);
	for (i = 0; i < size; i++){
		B[i] = (ratio_t*)malloc((size - 1) * sizeof(ratio_t));
		if (!B[i]) errorHandler(0);
	}

	for (i = 0, i_ = 0; i < size; i++, i_++){
		for (j = 0, j_ = 0; j < size; j++, j_++){
			if (i == str){
				i_--;
				break;
			}

			if (j == column){
				j_--;
				continue;
			}

			B[i_][j_] = A[i][j];
		}
	}

	return B;
}

int NumberGcd(int firstNumber, int secondNumber){
	int rmd, temp;

	if (firstNumber < secondNumber){
		temp = secondNumber;
		secondNumber = firstNumber;
		firstNumber = temp;
	}

	if (secondNumber == 0) return 1;

	while (1){
		if ((rmd = firstNumber % secondNumber) != 0){
			firstNumber = secondNumber;
			secondNumber = rmd;
		}else{
			return secondNumber;
		}
	}
}

#pragma endregion
