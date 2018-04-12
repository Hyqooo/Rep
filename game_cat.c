#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_HEIGTH 10
#define MAX_WEIDTH 20
#define SPACE 32

typedef struct Position{
	int x;
	int y;
}pos_t;

typedef struct Queue{
	pos_t position;
	struct Queue *next;
}queue_t;

/* Глобальные переменные */
char field[MAX_HEIGTH][MAX_WEIDTH];
int pathField[MAX_HEIGTH][MAX_WEIDTH];
pos_t cat, player;
queue_t *head, *end;
const int border_lX = 2, border_rX = 17, border_uY = 2, border_dY = 7;

/* Прототипы */
int findPath();
int catMovement();
int changeField();
int restorePath(pos_t);
void game();
void init(int);
void freeQueue();
void printField();
void enqueue(int, int);
void putWalls(int, int);
pos_t dequeue();

int main(){
	game();
}

void game(){
	int difficulty, isCatWon;
	char symbol;

	printf("Choose difficulty(1, 2, 3, 4): ");
	scanf("%d", &difficulty);
	if (difficulty <= 0) return;
	init(difficulty);
	printField();
	
 	while (1){
		symbol = _getch();
		if (symbol == '1') break;
		
		if (symbol == 'a' || symbol == 'A'){
			player.x--;
			if (player.x <= 0) player.x = MAX_WEIDTH - 2;
		}
		if (symbol == 'd' || symbol == 'D'){
			player.x++;
			if (player.x >= MAX_WEIDTH - 1) player.x = 1;
		}
		if (symbol == 'w' || symbol == 'W'){
			player.y--;
			if (player.y <= 0) player.y = MAX_HEIGTH - 2;
		}
		if (symbol == 's' || symbol == 'S'){
			player.y++;
			if (player.y >= MAX_HEIGTH - 1) player.y = 1;
		}
		if (symbol == SPACE){
			if (changeField() != 1){
				isCatWon = catMovement();
				if (isCatWon == 1){
					printField();
					printf("\nCat won! Shame on you!\n");
					break;
				}else if (isCatWon == 0){
					printf("\nYou won! Good for you!\n");
					break;
				}
			}
		}

		printField();
	}
}

int catMovement(){
	int i, j;

	if(findPath() == 1)
		return 0;

	for (i = -1; i < 2; i++){
		for (j = -1; j < 2; j++){
			if (pathField[cat.y + i][cat.x + j] == -1){
				cat.x += j;
				cat.y += i;
				break;
			}
		}
		if (j != 2) break;
	}

	if (cat.x == border_lX - 1 || cat.x == border_rX + 1 ||
		cat.y == border_uY - 1 || cat.y == border_dY + 1){
		return 1;
	}

	return -1;
}

int findPath(){
	pos_t start = cat, end;
	int i, j, distance = 0;
	
	for (i = 0; i < MAX_HEIGTH; i++){
		for (j = 0; j < MAX_WEIDTH; j++){
			pathField[i][j] = 0;
		}
	}

	freeQueue();

	pathField[start.y][start.x] = -2;

	while (1){
		// Добавляем соседние клетки в очередь
		for (i = -1; i < 2; i++){
			for (j = -1; j < 2; j++){
				if (field[start.y + i][start.x + j] != '#' && 
					pathField[start.y + i][start.x + j] == 0){

					pathField[start.y + i][start.x + j] = distance + 1;
					enqueue(start.x + j, start.y + i);
				}
			}
		}

		// Достаем из очереди
		if (!head) return 1;
		start = dequeue();
		distance = pathField[start.y][start.x];

		if (start.x == border_lX - 1 || start.x == border_rX + 1||
			start.y == border_uY - 1|| start.y == border_dY + 1){
			end.x = start.x;
			end.y = start.y;
			break;
		}
	}

	restorePath(end);

	return 0;
}

int restorePath(pos_t end){
	int distance, i, j;
	pos_t currentPos;

	distance = pathField[end.y][end.x];
	pathField[end.y][end.x] = -1;
	currentPos = end;

	while (1){
		for (i = -1; i < 2; i++){
			for (j = -1; j < 2; j++){
				if (pathField[currentPos.y + i][currentPos.x + j] == distance - 1 &&
					pathField[currentPos.y + i][currentPos.x + j] != 0){
					distance--;
					currentPos.y += i;
					currentPos.x += j;
					pathField[currentPos.y][currentPos.x] = -1;
					if (currentPos.x == cat.x && currentPos.y == cat.y)
						break;
				}

				if (pathField[currentPos.y + i][currentPos.x + j] == -2)
					return 1;
			}
		}
	}
}

// Добавляет в очередь
void enqueue(int x, int y){
	queue_t *temp = NULL;

	temp = (queue_t*)malloc(sizeof(queue_t));
	if (!temp) return;

	temp->position.x = x;
	temp->position.y = y;
	temp->next = end;
	if (!end){
		temp->next = NULL;
		end = temp;
		head = temp;
	}
	end = temp;
}

pos_t dequeue(){
	queue_t *cur, *temp;
	pos_t value;

	for (cur = end; cur->next != head; cur = cur->next){
		if (cur->next == NULL){
			temp = cur;
			value = temp->position;
			head = NULL;
			end = head;
			free(temp);
			return value;
		}
	}

	temp = cur->next;
	value = temp->position;
	head = cur;
	head->next = NULL;
	free(temp);

	return value;
}

void freeQueue(){
	queue_t *temp, *cur;

	for (cur = end; cur != head;){
		temp = cur;
		cur = cur->next;
		free(temp);
	}

	end = head = NULL;
}

/* Инициализирует поле */
void init(int difficulty){
	int i, j, max_walls;

	/* Стартовые координаты игрока и кота */
	player.x = MAX_WEIDTH / 2;
	player.y = MAX_HEIGTH / 2;
	cat.x = player.x - 1;
	cat.y = player.y;

	for (i = 0; i < MAX_HEIGTH; i++){
		for (j = 0; j < MAX_WEIDTH; j++){
			if (i == 0 || i == MAX_HEIGTH - 1){
				field[i][j] = '#';
			}
			if (j == 0 || j == MAX_WEIDTH - 1){
				field[i][j] = '#';
			}
		}
	}

	/* Максимальное количество стенок */
	max_walls = (int)(2 * MAX_HEIGTH * 0.8 + 2 * MAX_WEIDTH * 0.6);

	putWalls(max_walls, difficulty);
}

/* Отрисовка игрового поля */
void printField(){
	int i, j;

	system("cls");

	for (i = 0; i < MAX_HEIGTH; i++){
		for (j = 0; j < MAX_WEIDTH; j++){
			if (player.x == cat.x && player.y == cat.y && i == player.y && j == player.x ||
				field[player.y][player.x] == '#' && i == player.y && j == player.x){
				printf("X");
			}else{
				if (i == cat.y && j == cat.x){
					printf("@");
				}else if (i == player.y && j == player.x)
					printf("?");
				else
					printf("%c", field[i][j]);
			}
		}
		printf("\n");
	}

	for (i = 0; i < MAX_HEIGTH; i++){
		for (j = 0; j < MAX_WEIDTH; j++){
			printf("%d", pathField[i][j]);
		}
		printf("\n");
	}
}

/* Вносит изменения на поле */
int changeField(){
	if ((player.x != cat.x || player.y != cat.y) && field[player.y][player.x] != '#'){
		field[player.y][player.x] = '#';
		return 0;
	}
	return 1;
}

void putWalls(int max_walls, int difficulty){
	int act_walls, old_walls, rand_num, i, j;
	int count = 0;

	srand((unsigned int)time(NULL));

	/* Выбирает в зависимости от уровня слонжости количество стенок */
	switch (difficulty){
	case 1:
		act_walls = (int)(max_walls * 0.5);
		break;
	case 2:
		act_walls = (int)(max_walls * 0.3);
		break;
	case 3:
		act_walls = (int)(max_walls * 0.20);
		break;
	default:
		act_walls = (int)(max_walls * 0.15);
		break;
	}

	/* Ставит стенки */
	old_walls = act_walls;
	while (1){
		rand_num = rand() % max_walls;
		count = 0;
		for (i = border_uY; i < border_dY + 1; i++){
			if (act_walls != old_walls){
				old_walls = act_walls;
				break;
			}
			for (j = border_lX; j < border_rX + 1; j++){
				if (i != border_uY && i != border_dY && j != border_lX && j != border_rX) 
					continue;
				count++;
				if (count == rand_num)
					if (field[i][j] != '#'){
						field[i][j] = '#';
						act_walls--;
					}
			}
		}
		if (act_walls == 0) break;
	}
}
