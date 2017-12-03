#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define ERR -2

/* Laplace func */
int Sign(int);
int RecursiveLaplace(int**, int);
int FindZeroes(int **, int, int *);
int** Transpose(int **, int);
int** GetAMinor(int **, int, int, int);

/* Gauss func */
int FindMin(int **, int, int);
int Swap(int **, int, int, int, int, int);
long int DetFruGauss(long int **, int);
long int Gauss(long int **, int, int *);

/* Leibniz func */
int AmInv(int *, int);
int NextPerm(int *, int);
int LeibnizDet(long int **, int);
void InitPerm(int *, int);
void SwapItems(int *, int, int);
unsigned long Factorial(unsigned long);

/* Utility func */
void IsAllocated(void *);
void FreeMemory(void **, int);
void DisplayMatrix(long int **, int);

/* Global var */
int num;
long int comp = 1;

void main(int argc, char *argv[]){
	int size, i, j, sign = 1;
	char type;
	long int det;
	long int **A = NULL;
	FILE *f = NULL;

	if (argc < 3){
		printf("Choose method to calculate determinant.\n");
		printf("G - Gauss, R - Laplace, L - Leibniz\n");
		scanf("%c", &type);
	}else if (argc > 1){
		type = *argv[2];
	}else{
		printf("\nNot enougn parameters!\n");
		return;
	}

#pragma region FILEOPEN
	
	f = fopen(argv[1], "rt");
	if (f == NULL){
		printf("\nCannot open the file.\n");
		return;
	}

	fscanf(f, "%d", &size);

	A = (int **)malloc(size * sizeof(int*));
	IsAllocated(A);
	for (i = 0; i < size; i++){
		A[i] = (int*)malloc(size * sizeof(int));
		IsAllocated(A[i]);
	}

	for (i = 0; i < size; i++){
		for (j = 0; j < size; j++){
			if (!fscanf(f, "%d", &A[i][j])){
				printf("\nInvalid value!\n");
				return;
			}
		}
	}

	fclose(f);

#pragma endregion
	

	/* Output */
	printf("Matrix A:\n");
	DisplayMatrix(A, size);
	printf("\n");
	num = size;

	/* G - Gauss, R - Recursive Laplace, L - Leibniz */
	if (type == 'G'){
		det = DetFruGauss(A, size);
	}else if (type == 'L'){
		det = LeibnizDet(A, size);
	}else if (type == 'R'){
		det = RecursiveLaplace(A, size);
	}else{
		printf("\nUndefined type of method!\n");
		return;
	}
	printf("\nDeterminant: %d\n", det);

	/* Free memory */
	FreeMemory(A, size);
}

#pragma region LEIBNIZ

int LeibnizDet(long int **A, int size){
	int i, j, fact, det = 0, sl = 1, column, *perms = NULL;

	/* Number of terms */
	fact = Factorial(size);

	/* Array of permutations */
	perms = (int *)malloc(size * sizeof(int));
	IsAllocated(perms);

	/* Initializing permutations */
	InitPerm(perms, size);

	for (i = 0; i < fact; i++){
		sl = 1;
		if (size < 4){
			if (AmInv(perms, size) > 0)
				printf(" + 1");
			else
				printf(" - 1");
		}

		for (j = 0; j < size; j++){
			column = perms[j];
			sl *= A[j][column];
			if (size < 4) printf("*%d", A[j][column]);
		}
		det += sl * AmInv(perms, size);
		NextPerm(perms, size);
	}

	/* Free Memory */
	free(perms);

	return det;
}

void InitPerm(int *seq, int size){
	int i;
	for (i = 0; i < size; i++) 
		seq[i] = i;
}

/* Next permutation */
int NextPerm(int *seq, int size) {
	int i = size, j;

	do {
		if (i < 2) return 0; 
		i--;
	} while (seq[i - 1] > seq[i]);

	for (j = size; j > i && seq[i - 1] > seq[--j];);
	SwapItems(seq, i - 1, j);

	for (j = size; i < j;){
		j--;
		SwapItems(seq, i++, j);
	}

	return 1;
}

void SwapItems(int *seq, int ind_1, int ind_2){
	int t;
	t = seq[ind_1];
	seq[ind_1] = seq[ind_2];
	seq[ind_2] = t;
}

/* Number of inversions */
int AmInv(int *perms, int size){
	int i, j, temp, count = 0;

	for (i = 0; i < size; i++){
		temp = perms[i];
		for (j = i; j < size; j++){
			if (perms[j] < temp){
				count++;
			}
		}
	}

	if (count % 2 == 0)
		return 1;
	else
		return -1;
}

unsigned long Factorial(unsigned long a){
	if (a > 12) return ERR;
	if (a == 0) return 1;

	return a * Factorial(a - 1);
}

#pragma endregion

#pragma region GAUSS

long int DetFruGauss(long int **A, int size){
	int i, sign = 1;
	long double det = 1;

	comp = Gauss(A, size, &sign);
	if (comp == 0) return 0;

	printf("\n(%d", sign);
	for (i = 0; i < size; i++){
		det *= A[i][i];
		if (i == 1) det /= comp;
		printf(" * %li", A[i][i]);
	}
	printf(")/%li", comp);

	return (long)(det * sign);
}

long int Gauss(long int **A, int size, int *sign){
	int i, j, k, mult = 0, lead;

	for (i = 0; i < size; i++){
		*sign *= FindMin(A, size, i);
		if (*sign == ERR || *sign == -ERR) return 0;
		lead = A[i][i];

		for (j = i; j < size; j++){
			if (j + 1 < size){

				if (A[j + 1][i] % lead != 0){
					for (k = i; k < size; k++){
						A[j + 1][k] *= lead;
					}
					comp *= lead;
				}

				/* Rows subtraction */
				mult = A[j + 1][i] / lead;
				A[j + 1][i] -= mult * lead;
				for (k = i + 1; k < size; k++){
					if (mult >= 0){
						A[j + 1][k] -= mult * A[i][k];
					}else{
						A[j + 1][k] += -mult * A[i][k];
					}
				}

				/* Step output */
				printf("\n(1/%li) * \n\n", comp);
				DisplayMatrix(A, size);
			}
		}
	}

	return comp;
}

int FindMin(int **A, int size, int lvl){
	int i, j, min = 0, row = lvl, column = lvl;

	/* Find first non-zero value */
	for (i = lvl; i < size; i++){
		for (j = lvl; j < size; j++){
			if (A[i][j] != 0){
				min = A[i][j];
				row = i;
				column = j;
				break;
			}
		}
	}

	if (min == 0) return ERR;

	for (i = lvl; i < size; i++){
		for (j = lvl; j < size; j++){
			if (abs(A[i][j]) < abs(min) && A[i][j] != 0){
				min = A[i][j];
				row = i;
				column = j;
			}
		}
	}

	return Swap(A, size, lvl, row, lvl, column);
}

int Swap(int **A, int size, int s1, int s2, int r1, int r2){
	int t, i, sign = 1;
	
	/* Swap rows */
	if (s1 != s2){
		for (i = 0; i < size; i++){
			t = A[s1][i];
			A[s1][i] = A[s2][i];
			A[s2][i] = t;
		}
		sign *= -1;
	}

	/* Swap columns */
	if (r1 != r2){
		for (i = 0; i < size; i++){
			t = A[i][r1];
			A[i][r1] = A[i][r2];
			A[i][r2] = t;
		}
		sign *= -1;
	}

	return sign;
}

#pragma endregion

#pragma region LAPLACE

int** Transpose(int **A, int size){
	int i, j, **B = NULL;
	
	B = (int **)malloc((size - 1) * sizeof(int*));
	IsAllocated(B);
	for (i = 0; i < size; i++){
		B[i] = (int*)malloc((size - 1) * sizeof(int));
		IsAllocated(B[i]);
	}
	
	for (i = 0; i < size; i++){
		for (j = 0; j < size; j++){
			B[i][j] = A[j][i];
		}
	}

	return B;
}

int FindZeroes(int **A, int size, int *str){
	int i, j, isStr = 0, max = 0, countS = 0, countR = 0, num = 0;

	for (i = 0; i < size; i++){
		countS = 0;
		countR = 0;
		for (j = 0; j < size; j++){
			if (A[i][j] == 0){
				countS++;
			}
			if (A[j][i] == 0){
				countR++;
			}
		}

		if (countS >  countR){
			if (countS > max){
				max = countS;
				isStr = 1;
				num = i;
			}
		}
		else{
			if (countR > max){
				max = countR;
				isStr = 0;
				num = i;
			}
		}
	}

	*str = num;

	return isStr;
}

int RecursiveLaplace(int **A, int size){
	int j, result = 0, str = 0;

	if (size == 2){
		return A[0][0] * A[1][1] - A[0][1] * A[1][0];
	}

	/* Find maximum amount of zeros in row/column */
	if (!FindZeroes(A, size, &str)) A = Transpose(A, size);
	
	for (j = 0; j < size; j++){
		if (A[str][j] == 0) continue;

		if (num == size) printf("%d\n", A[str][j]);
		result += Sign(str + j) * A[str][j] * RecursiveLaplace(GetAMinor(A, str, j, size), 
			size - 1);
	}

	return result;
}

int Sign(int power){
	if (power % 2 == 0){
		return 1;
	}else{
		return -1;
	}
}

int** GetAMinor(int **A, int str, int column, int size){
	int i, j, **B = NULL, i_, j_;

	B = (int **)malloc((size - 1) * sizeof(int*));
	IsAllocated(B);
	for (i = 0; i < size; i++){
		B[i] = (int*)malloc((size - 1) * sizeof(int));
		IsAllocated(B);
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
	if (num == size){
		DisplayMatrix(B, size - 1);
		printf("\n");
	}
	return B;
}

#pragma endregion 

#pragma region UTILITY

void DisplayMatrix(long int **A, int size){
	int i, j;
	
	for (i = 0; i < size; i++){
		for (j = 0; j < size; j++){
			printf("%8li ", A[i][j]);
		}
		printf("\n");
	}
}

void IsAllocated(void *mem){
	if (mem == NULL){
		printf("\nCannot allocate memory!\n");
		exit(0);
	}
}

void FreeMemory(void **mem, int size){
	int i;
	for (i = 0; i < size; i++){
		free(mem[i]);
	}
	free(mem);
}

#pragma endregion
