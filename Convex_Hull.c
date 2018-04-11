#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "dislin.h"

#define EPSILON 0.00000001

// Точка
typedef struct Point{
	int x;
	int y;
	int z;
}point_t;

// Неравенство 
typedef struct Inequality{
	int coeffs[3];
	int free;
}ineq_t;

// Прототипы
int manage(int, char *[]);
int consoleInput();
int fileInput(int, char *[]);
int determineFaces(int);
int linearDep(int, ineq_t);
int swap(int **, int, int, int);
int findMin(int **, int, int);
int skeleton();
void printInequalities(int, int);
void reverse(int **);
void vertexEnum();
void printPoints();
void printAdjacencyMatrix();
void printFaceInfo();
int gauss(int **);
int checkPoint(point_t);
int graph();
point_t determinePoints(ineq_t, ineq_t, ineq_t);

// Глобальные переменные
int ineqCount, pointsCount;
char xyz[] = {'x', 'y', 'z', '\0'};
point_t *pointsArray;
ineq_t *ineqArray;
int **adjacencyMatrix;

int main(int argc, char *argv[]){
	manage(argc, argv);
	return 0;
}

// Определяет какие функции будут вызваны
int manage(int argc, char *argv[]){	
	if (argc == 1)
		// Не читает файлов
		consoleInput();
	else{
		// Read files
		fileInput(argc, argv);
	}


	// Освобождаем память
	free(pointsArray);
	free(ineqArray);
}

#pragma region INPUT AND OUTPUT

// Консольный ввод
int consoleInput(){

}

// Файловый ввод
int fileInput(int argc, char *argv[]){
	FILE *f = NULL;
	int arraySize, i;
	char type[10];
	point_t point;
	ineq_t ineq;
	
	if (argc == 2){
		// Читает один файл  
		f = fopen(argv[1], "r");
		if (!f) return 1;

		if(fscanf(f, "%s%d", type, &arraySize) != 2) return 1;

		if (strcmp(type, "V") == 0){
			// Память под массив точек
			pointsArray = (point_t*)malloc((arraySize + 1) * sizeof(point_t));
			if (!pointsArray) return 1;

			pointsCount = arraySize;

			// Дано множество точек
			for (i = 0; fscanf(f, "%d%d%d", &point.x, &point.y, &point.z) != EOF; i++){
				pointsArray[i].x = point.x;
				pointsArray[i].y = point.y;
				pointsArray[i].z = point.z;
				printf("%d %d %d\n", pointsArray[i].x, pointsArray[i].y, pointsArray[i].z);
			}

			// Вызывает функцию нахождения граней
			determineFaces(arraySize);
			
			// Вывод
			printInequalities(0, ineqCount);

			// Строим граф
			skeleton();
		}else if (strcmp(type, "H") == 0){
			// Память под неравенства 
			ineqArray = (ineq_t*)malloc(arraySize * sizeof(ineq_t));
			if (!ineqArray) return 1;

			ineqCount = arraySize;

			// Дана система неравенств
			for (i = 0; fscanf(f, "%d%d%d%d", &ineq.coeffs[0], &ineq.coeffs[1],
				&ineq.coeffs[2], &ineq.free) != EOF; i++){

				ineqArray[i].coeffs[0] = ineq.coeffs[0];
				ineqArray[i].coeffs[1] = ineq.coeffs[1];
				ineqArray[i].coeffs[2] = ineq.coeffs[2];
				ineqArray[i].free = ineq.free;
			}

			// Вывод неравенств в поток
			printInequalities(0, ineqCount);
			vertexEnum();
			printPoints();
		}else{
			// Непонятно что дано на вход
			return 1;
		}
	}else{
		// Читает два файла

	}
}

void printInequalities(int start, int arraySize){
	int i, j;

	printf("\n\nFace:\n");
	for (i = start; i < arraySize; i++){
		for (j = 0; j < 3; j++){
			if (ineqArray[i].coeffs[j] == 1 || ineqArray[i].coeffs[j] == -1){
				if (ineqArray[i].coeffs[j] == 1){
					printf("+%c", xyz[j]);
				}
				else if (ineqArray[i].coeffs[j] == -1){
					printf("-%c", xyz[j]);
				}
			}
			else if (ineqArray[i].coeffs[j] != 0){
				printf("%+d%c", ineqArray[i].coeffs[j], xyz[j]);
			}
		}
		printf(" <= %d\n", -ineqArray[i].free);
	}
}

#pragma endregion

#pragma region CONVEX HULL (A1)(DONE)

// Находит неравенства 
int determineFaces(int arraySize){
	int i, j, k, t, sign = 0, res, flag;
	// Неожиданно, вектора - это точки
	point_t vect_1, vect_2;
	ineq_t temp;
	
	// Память под неравенства
	ineqArray = (ineq_t*)calloc(arraySize + 5, sizeof(ineq_t));
	if (!ineqArray) return 1;

	for (i = 0; i < arraySize; i++){
		for (j = 0; j < arraySize; j++){
			if (i == j) continue;
			// Берем первую точку
			vect_1.x = pointsArray[j].x - pointsArray[i].x;
			vect_1.y = pointsArray[j].y - pointsArray[i].y;
			vect_1.z = pointsArray[j].z - pointsArray[i].z;
			
			for (k = 0; k < arraySize; k++){
				if (k == j || k == i) continue;
				vect_2.x = pointsArray[k].x - pointsArray[i].x;
				vect_2.y = pointsArray[k].y - pointsArray[i].y;
				vect_2.z = pointsArray[k].z - pointsArray[i].z;

				// Задаем плоскость
				temp.coeffs[0] = vect_1.y * vect_2.z - vect_1.z * vect_2.y;
				temp.coeffs[1] = -(vect_1.x * vect_2.z - vect_1.z * vect_2.x);
				temp.coeffs[2] = vect_1.x * vect_2.y - vect_1.y * vect_2.x;
				temp.free = -(
					temp.coeffs[0] * pointsArray[i].x +
					temp.coeffs[1] * pointsArray[i].y +
					temp.coeffs[2] * pointsArray[i].z);

				sign = 0;
				flag = 0;
				for (t = 0; t < arraySize; t++){
					if (t == i || t == j || t == k) continue;
					res =
						temp.coeffs[0] * pointsArray[t].x +
						temp.coeffs[1] * pointsArray[t].y +
						temp.coeffs[2] * pointsArray[t].z +
						temp.free;
					
					if (sign != 0 && res != 0 && sign != res / abs(res)){
						flag = 1;
						break;
					}
					
					if (res > 0){
						sign = 1;
					}else if(res != 0){
						sign = -1;
					}
				}

				// Проверить не является ли неравенство линейно зависимым
				if (linearDep(arraySize, temp) == 0 && !flag){
					if (sign > 0)
						sign = -1;
					else
						sign = 1;

					ineqArray[ineqCount].coeffs[0] = sign * temp.coeffs[0];
					ineqArray[ineqCount].coeffs[1] = sign * temp.coeffs[1];
					ineqArray[ineqCount].coeffs[2] = sign * temp.coeffs[2];
					ineqArray[ineqCount].free = sign * temp.free;
					ineqCount++;
				}
			}
		}
	}

	return 0;
}

// Определяет линейную независимость
int linearDep(int arraySize, ineq_t cmp){
	int i, j, fNotLinear = 0;
	double ratio;

	for (i = 0; i < ineqCount; i++){
		fNotLinear = 0;
		for (j = 0; j < 3; j++){
			if (ineqArray[i].coeffs[j] != 0){
				ratio = cmp.coeffs[j] / ineqArray[i].coeffs[j];
			}
		}
		if (ineqArray[i].free != 0 && j == 3){
			ratio = cmp.free / ineqArray[i].free;
		}

		for (j = 0; j < 3; j++){
			if (ineqArray[i].coeffs[j] != 0){
				if (ratio != cmp.coeffs[j] / ineqArray[i].coeffs[j]){
					fNotLinear = 1;
					break;
				}
			}
			else if (cmp.coeffs[j] != 0){
				fNotLinear = 1;
				break;
			}
		}

		if (ineqArray[i].free != 0){
			if (ratio != cmp.free / ineqArray[i].free){
				continue;
			}
		}
		else if (cmp.free != 0){
			continue;
		}
		if (fNotLinear == 0){
			return 1;
		}
	}
	return 0;
}

#pragma endregion

#pragma region VERTEX ENUMERATION (B1)

// Пересечь по тройкам все возможные неравенства 
void vertexEnum(){
	int i, j, k;
	point_t point;

	pointsArray = (point_t*)calloc((ineqCount * 2), sizeof(point_t));
	if (!pointsArray) return;

	for (i = 0; i < ineqCount; i++){
		for (j = 0; j < ineqCount; j++){
			if (j <= i) continue;
			for (k = 0; k < ineqCount; k++){
				if (k <= i || k <= j) continue;
				point = determinePoints(ineqArray[i], ineqArray[j], ineqArray[k]);
				if (checkPoint(point) == 0){
					printf("\nGOT IT!%d %d %d\n", point.x, point.y, point.z);
					pointsArray[pointsCount] = point;
					pointsCount++;
				}
			}
		}
	}
}

// Проверяет есть ли уже такая точка
int checkPoint(point_t point){
	int i;

	for (i = 0; i < pointsCount; i++){
		if (pointsArray[i].x == point.x && pointsArray[i].y == point.y &&
			pointsArray[i].z == point.z)
			return 1;
	}

	return 0;
}

point_t determinePoints(ineq_t ineq_1, ineq_t ineq_2, ineq_t ineq_3){
	int **tempArray = NULL, i, j;
	point_t point;
	
	// Память
	tempArray = (int**)malloc(ineqCount * sizeof(int*));
	if (!tempArray) /* Вызвать функцию обработки ошибок */;
	for (i = 0; i < ineqCount; i++){
		tempArray[i] = (int*)malloc(3 * sizeof(int));
		if (!tempArray[i]) /* Обработчик ошибок */;
	}

	// Перепишем в массив
	for (i = 0; i < 3; i++){
		tempArray[0][i] = ineq_1.coeffs[i];
		tempArray[1][i] = ineq_2.coeffs[i];
		tempArray[2][i] = ineq_3.coeffs[i];
	}
	tempArray[0][i] = ineq_1.free;
	tempArray[1][i] = ineq_2.free;
	tempArray[2][i] = ineq_3.free;

	if (gauss(tempArray) == 1){
		point = pointsArray[pointsCount];
	}else{
		for (i = 0; i < 3; i++){
			for (j = 0; j < 3; j++){
				if (tempArray[i][j] != 0){
					if (i == 0){
						point.x = tempArray[0][3] / tempArray[i][j];
					}else if (i == 1){
						point.y = tempArray[1][3] / tempArray[i][j];
					}else if (i == 2){
						point.z = tempArray[2][3] / tempArray[i][j];
					}
				}
			}
		}
	}

	// Освободить память
	for (i = 0; i < 3; i++){
		free(tempArray[i]);
	}
	free(tempArray);

	return point;
}

int gauss(int **tempArray){
	int i, j, k, mult, lead;

	printf("\nBefore\n");
	for (i = 0; i < 3; i++){
		for (j = 0; j < 4; j++){
			printf("%d ", tempArray[i][j]);
		}
		printf("\n");
	}

	for (i = 0; i < 3; i++){
		if(findMin(tempArray, 3, i) == 0) continue;
		lead = tempArray[i][i];
		for (j = i; j < 3; j++){
			if (j + 1 < 3){
				if (tempArray[j + 1][i] % lead != 0){
					for (k = i; k < 4; k++){
						tempArray[j + 1][k] *= lead;
					}
				}

				mult = tempArray[j + 1][i] / lead;
				tempArray[j + 1][i] -= mult * lead;
				for (k = i + 1; k < 4; k++){
					if (mult >= 0){
						tempArray[j + 1][k] -= mult * tempArray[i][k];
					}else{
						tempArray[j + 1][k] += -mult * tempArray[i][k];
					}
				}
			}
		}
	}

	// Придумать способ получше
	for (i = 1; i < 3; i++){
		if (findMin(tempArray, 3, i) == 0) continue;
		lead = tempArray[i][i];
		for (j = i; j > 0; j--){
			if (tempArray[j - 1][i] % lead != 0){
				for (k = 0; k < 4; k++){
					tempArray[j - 1][k] *= lead;
				}
			}

			mult = tempArray[j - 1][i] / lead;
			tempArray[j - 1][i] -= mult * lead;
			for (k = i + 1; k < 4; k++){
				if (mult >= 0){
					tempArray[j - 1][k] -= mult * tempArray[i][k];
				}else{
					tempArray[j - 1][k] += -mult * tempArray[i][k];
				}
			}
		}
	}

	printf("\nAfter_2\n");
	for (i = 0; i < 3; i++){
		for (j = 0; j < 4; j++){
			printf("%d ", tempArray[i][j]);
		}
		printf("\n");
	}

	// Проверить имеет ли система решения
	for (i = 0; i < 3; i++){
		if (tempArray[i][0] == 0 && tempArray[i][1] == 0 && tempArray[i][2] == 0)
			return 1;
	}

	return 0;
}

// Распечатывает точки с индексами
void printPoints(){
	int i, j = 0;
	char c = 'A';

	printf("\nPoints: %d\n", pointsCount);
	for (i = 0; i < pointsCount; i++){
		printf("%c(%d): %d %d %d\n", c++, j, pointsArray[i].x, pointsArray[i].y, pointsArray[i].z);
		if (i == 27 * (j + 1) - 2){
			c = 'A';
			j++;
		}
	}
}

int findMin(int **A, int size, int lvl){
	int i, min = 0, row = lvl, column = lvl;

	// Первый ненулевой элемент
	for (i = lvl; i < size; i++){
		if (A[i][lvl] != 0){
			min = A[i][lvl];
			row = i;
			column = lvl;
			break;
		}
	}

	if (min == 0) return 0;
	
	return swap(A, size, lvl, row);
}

int swap(int **A, int size, int s1, int s2){
	int t, i;

	/* Swap rows */
	if (s1 != s2){
		for (i = 0; i < size + 1; i++){
			t = A[s1][i];
			A[s1][i] = A[s2][i];
			A[s2][i] = t;
		}
	}

	return 1;
}

#pragma endregion

#pragma region 1-SKELETON (B2)

int skeleton(){
	int i;

	// Память
	adjacencyMatrix = (int**)calloc(pointsCount, sizeof(int*));
	if (!adjacencyMatrix) return 1;
	for (i = 0; i < pointsCount; i++){
		adjacencyMatrix[i] = (int*)calloc(pointsCount, sizeof(int));
		if (!adjacencyMatrix) return 1;
	}

	graph();

	return 0;
}

int graph(){
	int i, j, k, res_1, res_2, count;
	int edgePoints[15];

	for (i = 0; i < ineqCount; i++){
		printInequalities(i, i + 1);
		count = 0;
		printf("\nEdges: ");
		for (j = 0; j < ineqCount; j++){
			if (i == j) continue;
			for (k = 0; k < pointsCount; k++){
				res_1 = 
					ineqArray[i].coeffs[0] * pointsArray[k].x + 
					ineqArray[i].coeffs[1] * pointsArray[k].y +
					ineqArray[i].coeffs[2] * pointsArray[k].z +
					ineqArray[i].free;
				res_2 =
					ineqArray[j].coeffs[0] * pointsArray[k].x +
					ineqArray[j].coeffs[1] * pointsArray[k].y +
					ineqArray[j].coeffs[2] * pointsArray[k].z +
					ineqArray[j].free;

				if (res_1 == 0 && res_2 == 0){
					edgePoints[count] = k;
					count++;
				}
			}
			if (count % 2 == 0 && count != 0){
				adjacencyMatrix[edgePoints[count - 2]][edgePoints[count - 1]] = 1;
				adjacencyMatrix[edgePoints[count - 1]][edgePoints[count - 2]] = 1;
			}else if (count != 0){
				count--;
				edgePoints[count] = -1;
			}
		}

		printFaceInfo(edgePoints, count);
		for (j = 0; j < 15; j++) edgePoints[j] = -1;
	}

	printAdjacencyMatrix();

	return 0;
}

void printAdjacencyMatrix(){
	int i, j;

	printf("\n\nAdjacency matrix:\n  ");
	for (i = 0; i < pointsCount; i++) printf("%c ", 'A' + i);
	for (i = 0; i < pointsCount; i++){
		printf("\n%c ", 'A' + i);
		for (j = 0; j < pointsCount; j++){
			printf("%d ", adjacencyMatrix[i][j]);
		}
	}
	printf("\n");
}

void printFaceInfo(int edgePoints[15], int count){
	int i, j;

	for (i = 0; i < count; i++){
		if (i % 2 == 0){
			if (edgePoints[i] != -1 && edgePoints[i + 1] != -1)
				printf("%c%c ", 'A' + edgePoints[i], 'A' + edgePoints[i + 1]);
			for (j = i + 1; j < count; j++){
				if (edgePoints[j] == edgePoints[i] && edgePoints[j + 1] == edgePoints[i + 1])
					edgePoints[j] = -1;
			}
		}
	}

	printf("\nVertices: ");
	for (i = 0; i < count; i++){
		if (edgePoints[i] != -1)
			printf("%c ", 'A' + edgePoints[i]);
		for (j = i + 1; j < count; j++){
			if (edgePoints[j] == edgePoints[i]){
				edgePoints[j] = -1;
			}
		}
	}
}

#pragma endregion

#pragma region COLLISION DETECTION (B3)

	

#pragma endregion

#pragma region GRAPHICS AND STUFF

	

#pragma endregion
