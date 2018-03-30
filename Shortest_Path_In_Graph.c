#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define NOT_OK 1
#define MAX_NAME_LENGTH 11
#define MAX_VERTEX 10

/* Вершины */
typedef struct Vertex{
	char name[MAX_NAME_LENGTH];
	int distance;
}vertex_t;

/* Очередь */
typedef struct Queue{
	struct Vertex *element;
	struct Queue *next;
}queue_t;

/* Глобальные переменные */
int adjacency_matrix[MAX_VERTEX][MAX_VERTEX], countOpened;
char table[MAX_VERTEX][MAX_NAME_LENGTH];
char openedVertices[MAX_VERTEX][MAX_NAME_LENGTH];
vertex_t matrix[MAX_VERTEX][MAX_VERTEX];
queue_t *head, *end;

/* Прототипы */
int checkVertex(char *);
void fullTable(FILE *);
void formGraph(FILE *);
void enqueue(vertex_t *);
void deleteQueue();
void restorePath(char *, char *);
vertex_t dequeue();

int main(int argc, char *argv[]){
	FILE *f = NULL;

	f = fopen(argv[1], "r");
	if (!f) return NOT_OK;

	fullTable(f);
	formGraph(f);
	findPath();
	fclose(f);

	deleteQueue();
	printf("\n");
}

int findPath(){
	char start[MAX_NAME_LENGTH], finish[MAX_NAME_LENGTH];
	int i, start_num, end_num, cur;
	vertex_t vert;
	printf("Input start vertex: ");
	scanf("%s", start);
	printf("Input finish vertex: ");
	scanf("%s", finish);

	/* Найдем стартовую и конечную вершины */
	for (i = 0; i < MAX_VERTEX; i++){
		if (strcmp(start, table[i]) == 0) start_num = i;
		if (strcmp(finish, table[i]) == 0) end_num = i;
	}

	cur = start_num;
	strcpy(openedVertices[countOpened++], table[start_num]);
	vert = matrix[0][start_num];

	while (1){
		for (i = 0; i < MAX_VERTEX; i++){
			if (adjacency_matrix[cur][i] != 0 && checkVertex(table[i]) == 1){
				matrix[cur][i].distance = vert.distance + 1;
				matrix[i][cur].distance = vert.distance + 1;
				strcpy(openedVertices[countOpened++], table[i]);

				/* Добавляем вершину в очередь */
				enqueue(&matrix[cur][i]);
			}
		}

		/* Берем первый элемент очереди */
		vert = dequeue();
		if (strcmp(vert.name, table[end_num]) == 0){
			restorePath(start, finish);
			return;
		}
		for (i = 0; i < MAX_VERTEX; i++){
			if (strcmp(table[i], vert.name) == 0){
				cur = i;
			}
		}
	}
}

int checkVertex(char *name){
	int i;
	for (i = 0; i < MAX_VERTEX; i++){
		if (strcmp(name, openedVertices[i]) == 0) return 0;
	}

	return 1;
}

void restorePath(char *start, char *end){
	int distance = 0, i, j, num_1, num_2;

	/* Нашли последний элемент элемент */
	for (i = 0; i < MAX_VERTEX; i++){
		for (j = 0; j < MAX_VERTEX; j++){
			if (strcmp(end, matrix[i][j].name) == 0){
				if (matrix[i][j].distance > distance){
					distance = matrix[i][j].distance;
					num_1 = i;
					num_2 = j;
				}
			}
		}
	}

	printf("Vertices you should go: ");
	/* Смотрим номер строки, затем номер столбца и снова */
	while (1){
		for (i = 0; i < MAX_VERTEX; i++){
			if (matrix[i][num_2].distance == distance){
				distance -= 1;
				printf("%s ", matrix[i][num_2].name);
				num_2 = i;
				if (strcmp(matrix[0][num_2].name, start) == 0){
					printf("%s", matrix[0][num_2].name);
					return;
				}
			}
		}
	}

}

/* Заполняет матрицу смежности */
void formGraph(FILE *f){
	int i, num_1, num_2;
	char vertex_1[MAX_NAME_LENGTH], vertex_2[MAX_NAME_LENGTH];

	fseek(f, SEEK_SET, SEEK_SET);

	/* Матрица смежности */
	while (fscanf(f, "%s%s", vertex_1, vertex_2) != EOF){
		if (strcmp(vertex_1, vertex_2) == 0) continue;
		/* Находим в таблице номер вершины */
		for (i = 0; i < MAX_VERTEX; i++){
			if (strcmp(vertex_1, table[i]) == 0){
				num_1 = i;
			}
			if (strcmp(vertex_2, table[i]) == 0){
				num_2 = i;
			}
		}

		/* В таблице смежности заполняем соответствующие строки/столбцы */
		adjacency_matrix[num_1][num_2] = 1;
		adjacency_matrix[num_2][num_1] = 1;
	}

	for (i = 0; i < MAX_VERTEX; i++){
		for (int j = 0; j < MAX_VERTEX; j++){
			strcpy(matrix[i][j].name, table[j]);
		}
	}
}

/* Сопоставляет каждой вершине номер */
void fullTable(FILE *f){
	char vertex[MAX_VERTEX];
	int i, count = 0;

	while (fscanf(f, "%s", &vertex) != EOF){
		for (i = 0; i < MAX_VERTEX; i++){
			if (strcmp(vertex, table[i]) == 0)
				break;
		}
		if (i != MAX_VERTEX) continue;
		strcpy(table[count], vertex);
		count++;
	}
}

/* Добавляет в очередь */
void enqueue(vertex_t *elem){
	queue_t *temp = NULL;

	temp = (queue_t *)malloc(sizeof(queue_t));
	if (!temp) return;

	temp->element = elem;
	temp->next = end;
	if (!end){
		temp->next = NULL;
		end = temp;
		head = temp;
	}
	end = temp;
}

/* Удаляет из очереди */
vertex_t dequeue(){
	queue_t *cur, *temp;
	vertex_t value;

	for (cur = end; cur->next != head; cur = cur->next){
		if (cur->next == NULL){
			temp = cur;
			value = *(temp->element);
			head = NULL;
			end = head;
			free(temp);
			return value;
		}
	}

	temp = cur->next;
	value = *(temp->element);
	head = cur;
	head->next = NULL;
	free(temp);

	return value;
}

/* На случай если в очереди останется что-нибудь */
void deleteQueue(){
	queue_t *temp, *cur;

	for (cur = end; cur != head;){
		temp = cur;
		cur = cur->next;
		free(temp);
	}
}
