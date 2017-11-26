// Определитель через метод Гаусса
long int DetFruGauss(long int **A, int size){
	int i, sign = 1;
	long int comp = 1;
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

	return (long int)det * sign;
}

// Приводит матрицу к треугольному виду
long int Gauss(long int **A, int size, int *sign){
	int i, j, k, mult = 0, lead, i_, j_;
	long int comp = 1;

	for (i = 0; i < size; i++){
		*sign *= FindMin(A, size, i);
		if (*sign == ERR || *sign == -ERR) return 0;
		lead = A[i][i];

		if (lead > 10);

		for (j = i; j < size; j++){
			if (j + 1 < size){
				if (A[j + 1][i] % lead != 0){
					for (k = i; k < size; k++){
						A[j + 1][k] *= lead;
					}
					comp *= lead;
				}

				/* Вычитание строки */
				mult = A[j + 1][i] / lead;
				A[j + 1][i] -= mult * lead;
				for (k = i + 1; k < size; k++){
					if (mult >= 0){
						A[j + 1][k] -= mult * A[i][k];
					}else{
						A[j + 1][k] += -mult * A[i][k];
					}
				}

				/* Вывод шага */
				printf("\n(1/%li) * \n\n", comp);
				for (i_ = 0; i_ < size; i_++){
					for (j_ = 0; j_ < size; j_++){
						printf("%8li ", A[i_][j_]);
					}
					printf("\n");
				}
			}
		}
	}

	return comp;
}

// Находит минимальное значение в столбце
int FindMin(int **A, int size, int lvl){
	int i, j, min = 0, str = lvl, row = lvl;

	for (i = lvl; i < size; i++){
		for (j = lvl; j < size; j++){
			if (A[i][j] != 0){
				min = A[i][j];
				str = i;
				row = j;
			}
		}
	}

	if (min == 0) return ERR;

	for (i = lvl; i < size; i++){
		for (j = lvl; j < size; j++){
			if (abs(A[i][j]) < abs(min) && A[i][j] != 0){
				min = A[i][j];
				str = i;
				row = j;
			}
		}
	}

	return Swap(A, size, lvl, str, lvl, row);
}

// Меняет строки/столбцы
int Swap(int **A, int size, int s1, int s2, int r1, int r2){
	int t, i, sign = 1;
	
	if (s1 != s2){
		for (i = 0; i < size; i++){
			t = A[s1][i];
			A[s1][i] = A[s2][i];
			A[s2][i] = t;
		}
		sign *= -1;
	}

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
