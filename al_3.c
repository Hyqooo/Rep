/*  
	Лабораторная по алгебре "Локализация корней многочлена"
	

	Алгоритм:
		1) Программа из потока получает входные параметры:
			степень и коэффициенты многочлена и параметр точности аппроксимации
			Точность аппроксимации - корни многочлена точности аппроксимации
		2) Программа сокращает кратные корни многочлена:
			f(x) / (gcd(f(x), f'(x)))
		3) Через ряд Штурма локализует корни на отрезке
		4) Дальше методом биссекции находит корни с точностью аппроксимации
		5) На выход в поток ряд Штурма и значения действительных корней

	Доп. задание А:
		Аналитический ввод многочлена:
			x^2 + x + 1

	Доп. задание B:
		Программа с начала перебирает все рациональные корни и только затем запускает
		ряд Штурма
*/

/*  
	TODO: Сделать рассчет производной, можно встроить в деление
	TODO: Реализовать алгоритм Евклида(рекурсия?)
	TODO: Написать функцию Штурма, локализующую корни, при этом держать в голове доп. задание B
	TODO: Не забыть освободить память из под многочлена
*/


#include <stdio.h>
#include <stdlib.h>

/* Прототипы */
void Input(int, int);

/* Глобальные переменные */
int power, *coeff, approx;
int *eu_1, *eu_2;

int main(){
	/* 
		type_1 - тип ввода аналитический/коэффициенты
		type_2 - тип для поиска рациональных корней 
	*/
	char type_1, type_2;

	/* Тип ввода и поиска корней */
	printf("\nChoose type of input of the polynomial(A - analytics, C - coefficients)\n");
	scanf("%c%*c", &type_1);
	printf("\nDo you want to find rational roots? Y/N\n");
	scanf("%c", &type_2);

	Input(type_1, type_2);

	printf("\n");
	return 0;
}

/* Ввод многочлена */
void Input(int type_1){
	int i, j;

	if (type_1 == 'C'){
		/* Коэффициентный ввод */
		printf("\nInput power of the polynomial: ");
		if (!scanf("%d", &power)){
			printf("\nPower should be a number!");
			exit(0);
		}

		/* Выделение памяти для многочлена */
		coeff = (int*)malloc((power + 1) * sizeof(int));
		if (!coeff){
			printf("\nCannot allocate a memory!");
			exit(0);
		}

		/* Ввод коэффициентов */
		printf("\nEnter coefficients of the polynomial in descending order:\n");
		for (i = 0; i < power + 1; i++){
			if (!scanf("%d", &coeff[i])){
				printf("\nWrong coefficient!");
				exit(0);
			}
		}

		/* Вывод введенного многочлена */
		printf("\nPolynomial: ");
		for (i = 0, j = power; i < power + 1; i++, j--){
			if (coeff[i] == 0) continue;
		
			if (j)
				printf("%dx^%d", coeff[i], j);
			else
				printf("%d", coeff[i]);
		}
	}else if(type_1 == 'A'){
		/* Аналитический ввод */
		printf("Input polynomial:\n");

		/*  
			TODO: доделать аналитический ввод
			NOTE: память для первого коэффициента должна быть выделена
		*/
	}else{
		/* Неизвестный тип ввода */
		printf("\nUndefined type of input!");
		exit(0);
	}

	/* Параметр аппроксимации */
	printf("Enter parameter of the approximation: ");
	if (!scanf("%d", &approx)){
		printf("\nWrong parameter!");
		exit(0);
	}
}

/* Анализ строки UNDONE */
void StringAnalyser(){
	
}

/* 
	devend - делимое 
	dever - делитель 
*/
void AlgEuclid(int *devend, int *dever){

}

