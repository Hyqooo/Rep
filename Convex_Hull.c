/*  
	Выпуклая оболочка. 

	Библиотеки для моделирования на C:
		1) DISLIN (Пойдет)

	Проектирование:
		Задание A (Convex Hull):
			1) Ввод из файла точек трехмерного пространства c параметром V
			2) Берет три точки и строит через них плоскость
			3) Подставляет в уравнение плоскости оставшиеся точки 
			4) Если все остальные точки имееют одинаковый знак, то эта плоскость - грань
			5) Запоминает грань 
			6) Проверяет нет ли линейных с этой гранью
			5) Выводит в поток неравенство для грани

		Задание B1 (Vertex Enumeration):
			1) Ввод из файла коэффициентов неравенств 
			2) Вывод системы в поток в форматированном виде
			3) Найденные вершины выводит поток при этом давая им имена

		Задание B2 (1-skeleton):
			1) Если из файла подаются точки то выполняет задание A,
			затем задание B1. Если подается система неравенств, то сразу B1
			2) Строится полиэдральный граф
			3) Вывести граф в двух форматах:
				a) Матрица смежности
				b) Каждой грани сопоставляются принадлежащие ей вершины и ребра
					Пример:
						Face: x + y + z <= 1
						Verticies: B, C, D
						Edges: BC, BD, CD
			
		Задание B3 (collision detection):
			1) На вход подаются два файла с вершинами двух мноогранников
			2) Считает разность Минковского 
			TODO: Дополнить
			3) Выводится сообщение о пересечении

		Задание C: 
			Делает графический вывод построенной оболочки
			TODO: Дополнить - какие данные нужны


******************************************************************************************
	
	TODO: Разобраться с особенностями библиотеки, при необходимости найти другую
	TODO: Спроектировать программу как можно тщательнее, разобраться со всеми нюансами
	работы алгоритмов и вычислений
	TODO: Обработчик ошибок
	TODO: Чтобы опеределить является ли плоскость опорной нужно n^4 действий - оптимизировать
	
	UNDONE: Ввод координат вершин для двух файлов
	UNDONE: Ввод точек, если не задан файл
	DONE: Не умеет в 8 точек, возможно связано со свободным членом


	DONE: Функция нахождения опорных плоскостей
	DONE: Функция определения линейности
	DONE: Функция, печатающая неравенства

******************************************************************************************
		
		Выяснить количество граней в многограннике на n вершинах

*/

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
void printInequalities(int);

// Глобальные переменные
int ineqCount;
char xyz[] = {'x', 'y', 'z', '\0'};
point_t *pointsArray;
ineq_t *ineqArray;

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
			pointsArray = (point_t*)malloc(arraySize * sizeof(point_t));
			if (!pointsArray) return 1;

			// Дано множество точек
			for (i = 0; fscanf(f, "%d%d%d", &point.x, &point.y, &point.z) != EOF; i++){
				pointsArray[i].x = point.x;
				pointsArray[i].y = point.y;
				pointsArray[i].z = point.z;
				printf("%d %d %d\n", pointsArray[i].x, pointsArray[i].y, pointsArray[i].z);
			}

			// Вызывает функцию нахождения граней
			determineFaces(arraySize);
		}else if (strcmp(type, "H") == 0){
			// Память под неравенства 
			ineqArray = (ineq_t*)malloc(arraySize * sizeof(ineq_t));
			if (!ineqArray) return 1;

			// Дана система неравенств
			printf("Inequalities:\n");
			for (i = 0; fscanf(f, "%d%d%d%d", &ineq.coeffs[0], &ineq.coeffs[1],
				&ineq.coeffs[2], &ineq.free) != EOF; i++){

				ineqArray[i].coeffs[0] = ineq.coeffs[0];
				ineqArray[i].coeffs[1] = ineq.coeffs[1];
				ineqArray[i].coeffs[2] = ineq.coeffs[2];
				ineqArray[i].free = ineq.free;

				// Вывод неравенств в поток
				printInequalities(arraySize);
			}
		}else{
			// Непонятно что дано на вход
			return 1;
		}
	}else{
		// Читает два файла

	}
}

void printInequalities(int arraySize){
	int i, j;

	printf("Number of faces: %d\n", ineqCount);
	for (i = 0; i < ineqCount; i++){
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

	// Вывод
	printInequalities(arraySize);

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

	


#pragma endregion

#pragma region 1-SKELETON (B2)
		
	

#pragma endregion

#pragma region COLLISION DETECTION (B3)

	

#pragma endregion

#pragma region GRAPHICS AND STUFF

	

#pragma endregion
