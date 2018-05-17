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
int chooseMatrix(ratio_t **, int *, int);
int divisors(int *, int, int);
int findRationalRoots(ratio_t *, int);

void init(int *, int);
void nextPerm(int *, int, int);
void matrixInput(FILE *);
void printMatrix(int, ratio_t **);
void makeCharPolynomial();
void errorHandler(int);
void printPolynomial(ratio_t *, int);
void toIntCoeffs(ratio_t *, int);
void reduceCoeffs(ratio_t *, int);

int choose(int, int);

ratio_t RecursiveLaplace(ratio_t **, int);
ratio_t **GetAMinor(ratio_t **, int, int, int);
ratio_t valueAtPoint(ratio_t *, ratio_t, int);

// Глобальные перменные
int matrixSize;
ratio_t **inputMatrix, *charPolynomial, *roots;

int main(int agrc, char *argv[]){
	FILE *f = NULL;
	int rootsAmount;

	f = fopen(argv[1], "r");
	if (!f) errorHandler(1);
	matrixInput(f);
	fclose(f);

	printf("Matrix:\n");
	printMatrix(matrixSize, inputMatrix);

	makeCharPolynomial();

	rootsAmount = findRationalRoots(charPolynomial, matrixSize + 1);

	printf("Roots:\n");
	for (int i = 0; i < rootsAmount; i++){
		if (roots[i].denominator != 1)
			printf("x_%d = %d/%d\n", i, roots[i].numerator, roots[i].denominator);
		else
			printf("x_%d = %d\n", i, roots[i].numerator);
	}
}

#pragma region CASE A

void makeCharPolynomial(){
	int i, polPower, temp, *seq = NULL;
	ratio_t sum, result, **minorMatrix = NULL;

	charPolynomial = (ratio_t*)malloc((matrixSize + 1) * sizeof(ratio_t));
	if (!charPolynomial) errorHandler(0);

	if (matrixSize % 2 != 0)
		charPolynomial[0].numerator = -1;
	else
		charPolynomial[0].numerator = 1;
	
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
	printf("Characteristic polynomial:\nF(x) = ");
	printPolynomial(charPolynomial, matrixSize + 1);
}

// Выбирает главный минор
int chooseMatrix(ratio_t **matrix, int *seq, int size){
	int j, k, j_, k_;

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

	return 0;
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

// Вернуть количество корней
int findRationalRoots(ratio_t *pol, int size){
	// pA - делители свободного члена, qA - делители первого коэффициента
	struct{
		int *divArray;
		int size;
	}pA, qA;

	int i, j, rootsAmount = 0;
	ratio_t argument;

	pA.divArray = qA.divArray = NULL;

	toIntCoeffs(pol, size);

	roots = (ratio_t*)malloc(matrixSize * sizeof(ratio_t));
	if (!roots) errorHandler(0);

	// Количество делителей стоит переписать
	qA.divArray = (int *)malloc((abs(pol[0].numerator) * 2) * sizeof(int));
	pA.divArray = (int *)malloc((abs(pol[size - 1].numerator) * 2) * sizeof(int));
	if (!qA.divArray || !pA.divArray) errorHandler(0);

	qA.size = divisors(qA.divArray, abs(pol[0].numerator), 1);
	pA.size = divisors(pA.divArray, abs(pol[size - 1].numerator), 0);

	for (i = 0; i < qA.size; i++){
		for (j = 0; j < pA.size; j++){
			argument.numerator = pA.divArray[j];
			argument.denominator = qA.divArray[i];
			if (valueAtPoint(pol, argument, size).numerator == 0){
				if (compareRoots(argument, rootsAmount) == 0){
					roots[rootsAmount] = argument;
					rootsAmount++;
				}
			}
		}
	}

	free(qA.divArray);
	free(pA.divArray);

	return rootsAmount;
}

int compareRoots(ratio_t potenialRoot, int size){
	int gcd, i;

	gcd = NumberGcd(potenialRoot.numerator, potenialRoot.denominator);
	potenialRoot.numerator /= gcd;
	potenialRoot.denominator /= gcd;

	for (i = 0; i < size; i++){
		if (roots[i].numerator == potenialRoot.numerator &&
			roots[i].denominator == potenialRoot.denominator){
			return 1;
		}
	}

	return 0;
}

ratio_t valueAtPoint(ratio_t *pol, ratio_t arg, int size){
	int i, temp, gcd; 
	ratio_t sum, valueInPower;

	sum.numerator = 0;
	sum.denominator = 1;

	for (i = 0; i < size; i++){
		valueInPower.numerator = pol[i].numerator * (int)pow(arg.numerator, size - i - 1);
		valueInPower.denominator = pol[i].denominator * (int)pow(arg.denominator, size - i - 1);

		if (valueInPower.denominator != sum.denominator){
			temp = valueInPower.denominator;
			valueInPower.numerator *= sum.denominator;
			valueInPower.denominator *= sum.denominator;
			sum.numerator *= temp;
			sum.denominator *= temp;
		}

		sum.numerator += valueInPower.numerator;
		gcd = NumberGcd(sum.numerator, sum.denominator);
		sum.numerator /= gcd;
		sum.denominator /= gcd;
	}

	return sum;
}

int divisors(int *arrayDiv, int n, int f){
	int i, j;

	for (i = 1, j = 0; i < n + 1; i++){
		if (n % i == 0){
			arrayDiv[j] = i;
			if (!f){
				arrayDiv[j + 1] = -i;
				j += 2;
			}else{
				j += 1;
			}
		}
	}

	return j;
}

void toIntCoeffs(ratio_t *pol, int size){
	int gcd, lcm, i;

	lcm = pol[0].denominator;
	for (i = 1; i < size; i++){
		gcd = NumberGcd(lcm, pol[i].denominator);
		lcm = lcm * pol[i].denominator / gcd;
	}

	for (i = 0; i < size; i++){
		pol[i].numerator *= lcm;
	}

	reduceCoeffs(pol, size);
}

void reduceCoeffs(ratio_t *pol, int size){
	int gcd, i;

	for (i = 0; i < size; i++){
		gcd = NumberGcd(abs(pol[i].numerator), (pol[i].denominator));
		pol[i].numerator /= gcd;
		pol[i].denominator /= gcd;
	}
}

#pragma endregion

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

void printPolynomial(ratio_t *pol, int size){
	int i;

	for (i = 0; i < size; i++){
		if (pol[i].denominator == 1){
			printf("%+d", pol[i].numerator);
		}else if(pol[i].numerator != 0){
			printf("%+d/%d", pol[i].numerator, pol[i].denominator);
		}

		if (i == size - 2 || i == size - 1){
			if (i == size - 2)
				printf("x");
		}
		else{
			printf("x^%d", size - i - 1);
		}
	}
	printf("\n");
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
