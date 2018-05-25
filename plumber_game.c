#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define HEIGHT 15 + 2
#define WIDTH 25
#define SPACE 32

typedef struct Figure{
	char figure[3][3];
	int characteristicVector[6];
	int figureNumber;
}figure_t;

typedef struct Position{
	int x;
	int y;
}pos_t;

typedef struct Queue{
	pos_t position;
	struct Queue *next;
}queue_t;

int controller();
int createFields(FILE *f);
int printField();
int manageAction();
int checkFinish();
int turnFigure(figure_t *);
int figureInit();
int copyFigureToRealField(char [3][3], int, int);
void enqueue(int, int);
void freeQueue();
pos_t dequeue();

queue_t *head, *end;
figure_t line, t_shaped, corner, field[HEIGHT][WIDTH];
pos_t coords;
char realField[HEIGHT][WIDTH];
int checkPath[HEIGHT][WIDTH];


int main(int argc, char *argv[]){
	FILE *f;
	int i;

	for (i = 1; i < argc; i++){
		f = NULL;
		if (!(f = fopen(argv[i], "r")))
			return 1;

		createFields(f);
		controller();
		printf("You Won! Grats!\n");
		if (i != argc - 1){
			printf("Press space to continue. Other key to exit the game\n");
			if (_getch() == SPACE)
				continue;
			else
				break;
		}
	}

	freeQueue();
}

int controller(){
	char symbol;

	coords.x = coords.y = 1;
	printField();

	while (1){
		symbol = _getch();

		if (symbol == 'a' || symbol == 'A'){
			coords.x--;
			if (coords.x <= 0) coords.x = WIDTH - 2;
		}
		if (symbol == 'd' || symbol == 'D'){
			coords.x++;
			if (coords.x >= WIDTH - 1) coords.x = 1;
		}
		if (symbol == 'w' || symbol == 'W'){
			coords.y--;
			if (coords.y <= 0) coords.y = HEIGHT - 2;
		}
		if (symbol == 's' || symbol == 'S'){
			coords.y++;
			if (coords.y >= HEIGHT - 1) coords.y = 1;
		}
		if (symbol == SPACE){
			if (manageAction() == 0){
				if (checkFinish() == 0){
					printField();
					return 0;
				}
			}
		}

		printField();
	}
}

int manageAction(){
	if (field[coords.y][coords.x].figureNumber != -1){
		turnFigure(&field[coords.y][coords.x]);
		copyFigureToRealField(field[coords.y][coords.x].figure, coords.y, coords.x);
		return 0;
	}
	return 1;
}

int turnFigure(figure_t *figure){
	int i, j, plusJ, plusI;
	char buff[3][3];

	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			buff[i][j] = (*figure).figure[i][j];
		}
	}

	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			if (i == 1 && j == 1) continue;
			(*figure).figure[i][j] = '\0';
		}
	}

	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			if (buff[i][j] == '*'){
				if ((i + j) % 2 == 0){
					if (i == 0){
						if (j == 0){
							plusI = 0;
							plusJ = 2;
						}else{
							plusI = 2;
							plusJ = 0;
						}
					}else if (i == 2){
						if (j == 0){
							plusI = -2;
							plusJ = 0;
						}else{
							plusI = 0;
							plusJ = -2;
						}
					}
				}else{
					if (i != 2){
						if (j == 1){
							plusI = 1;
							plusJ = 1;
						}else if(j == 2){
							plusI = 1;
							plusJ = -1;
						}else{
							plusI = -1;
							plusJ = 1;
						}
					}else{
						plusI = -1;
						plusJ = -1;
					}
				}
				(*figure).figure[i + plusI][j + plusJ] = buff[i][j];
			}
		}
	}

	return 0;
}

int checkFinish(){
	int i, j;
	pos_t start;

	for (i = 0; i < HEIGHT; i++){
		for (j = 0; j < WIDTH; j++){
			if (realField[i][j] == 'S'){
				start.x = j;
				start.y = i;
			}
			checkPath[i][j] = 0;
		}
	}

	while (1){
		checkPath[start.y][start.x] = 1;

		for (i = -1; i < 2; i++){
			for (j = -1; j < 2; j++){
				if (start.x + j < WIDTH && start.x + j > 0 && start.y + i < HEIGHT
					&& start.y + i > 0 && (i + j) % 2 != 0){
					if ((realField[start.y + i][start.x + j] == '*' || 
						realField[start.y + i][start.x + j] == '@' ||
						realField[start.y + i][start.x + j] == 'F') &&
						checkPath[start.y + i][start.x + j] != 1){
						// Добавляем в очередь клетку
						enqueue(start.y + i, start.x + j);
					}
				}
			}
		}

		if (!head && realField[start.y][start.x] != 'F') return 1;
		if (realField[start.y][start.x] == 'F') return 0;
		start = dequeue();
	}

	return 0;
}

int createFields(FILE *f){
	int i, j;
	char c;

	for (i = 0; i < HEIGHT; i++){
		for (j = 0; j < WIDTH; j++){
			field[i][j].figureNumber = -1;
			realField[i][j] = '\0';
		}
	}

	figureInit();
	
	for (i = 0; i < HEIGHT; i++){
		for (j = 0; j < WIDTH + 1; j++){
			c = fgetc(f);
			switch (c){
				case 't':
					field[i][j] = t_shaped;
					break;
				case 'l':
					field[i][j] = line;
					break;
				case 'c':
					field[i][j] = corner;
					break;
				case '*':
					realField[i][j] = '*';
					break;
				case 'S':
					realField[i][j] = 'S';
					break;
				case 'F':
					realField[i][j] = 'F';
					break;
				default:
					break;
			}
		}
	}

	for (i = 0; i < HEIGHT; i++){
		for (j = 0; j < WIDTH; j++){
			if (field[i][j].figureNumber == 0){
				copyFigureToRealField(line.figure, i, j);
			}else if (field[i][j].figureNumber == 1){
				copyFigureToRealField(t_shaped.figure, i, j);
			}else if (field[i][j].figureNumber == 2){
				copyFigureToRealField(corner.figure, i, j);
			}
		}
	}

	return 0;
}

int copyFigureToRealField(char figure[3][3], int _i, int _j){
	int i, j;

	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			realField[_i + i - 1][_j + j - 1] = figure[i][j];
		}
	}

	return 0;
}

int figureInit(){
	// Прямая
	line.figure[0][1] = '*';
	line.figure[1][1] = '@';
	line.figure[2][1] = '*';
	line.figureNumber = 0;

	// Т-фигура
	t_shaped.figure[0][0] = '*';
	t_shaped.figure[0][1] = '*';
	t_shaped.figure[0][2] = '*';
	t_shaped.figure[1][1] = '@';
	t_shaped.figure[2][1] = '*';
	t_shaped.figureNumber = 1;

	// Угол
	corner.figure[0][1] = '*';
	corner.figure[1][1] = '@';
	corner.figure[1][2] = '*';
	corner.figureNumber = 2;

	return 0;
}

int printField(){
	int i, j;

	system("cls");

	for (i = 0; i < HEIGHT; i++){
		for (j = 0; j < WIDTH; j++){
			if (coords.x == j && coords.y == i &&
				realField[coords.y][coords.x] != '*' && realField[coords.y][coords.x] != '@')
				printf("o");
			else if (coords.x == j && coords.y == i && realField[coords.y][coords.x] != '@')
				printf("X");
			else if (coords.x == j && coords.y == i &&
				realField[coords.y][coords.x] == '@')
				printf("?");
			else
				printf("%c", realField[i][j]);
		}
		printf("\n");
	}

	return 0;
}

// Добавляет в очередь
void enqueue(int y, int x){
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
