#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct List{
	int info;
	struct List *next;
}list_t;

typedef struct Tree{
	int info;
	struct Tree *left;
	struct Tree *right;
}tree_t;

list_t *first, *last;
tree_t *root;

int powerOfTwo(int);
int createStartList();
int tournament(tree_t *);
int clearNumInTree(tree_t *, int);

void clearList();

tree_t *formTree(tree_t *, int);
tree_t *freeTree(tree_t *);

void main(){
	int count, i, num;

	count = createStartList();
	
	// До какого уровня формировать дерево
	root = formTree(root, (int)log2(powerOfTwo(count)) + 1);

	for (i = 0; i < count; i++){
		num = tournament(root);
		printf("%d ", num);
		clearNumInTree(root, num);
	}
	printf("\n");

	freeTree(root);
	clearList();
}

int clearNumInTree(tree_t *node, int num){

	if (!node)
		return 0;

	if (node->info == num)
		node->info = -1;

	clearNumInTree(node->left, num);
	clearNumInTree(node->right, num);

	return 0;
}

int tournament(tree_t *node){

	if (!node)
		return 0;

	tournament(node->left);
	tournament(node->right);
	
	if (node->left && node->right){
		if (node->left->info != -1 && node->right->info != -1){
			if (node->left->info < node->right->info)
				node->info = node->left->info;
			else
				node->info = node->right->info;
		}else{
			if (node->left->info == -1){
				node->info = node->right->info;
			}else{
				node->info = node->left->info;
			}
		}
	}else if(node->left || node->right){
		if (node->left){
			node->info = node->left->info;
		}else{
			node->info = node->right->info;
		}
	}
	
	return node->info;
}

// Дополняет до степени двойки
int powerOfTwo(int num){
	int i, power;
	
	for (i = 0;;){
		power = (int)pow(2, i);
		if (power < num){
			i++;
		}else{
			break;
		}
	}

	return power;
}

tree_t* formTree(tree_t *node, int toLvl){
	if (toLvl == 0){
		return NULL;
	}

	node = (tree_t *)malloc(sizeof(tree_t));
	if (!node) return NULL;

	if (toLvl == 1){
		if (last){
			node->info = last->info;
			last = last->next;
		}else{
			node->info = -1;
			return NULL;
		}
	}else{
		node->info = -1;
	}

	node->left = formTree(node->left, toLvl - 1);	
	node->right = formTree(node->right, toLvl - 1);

	return node;
}

int createStartList(){
	list_t *cur;
	int temp, count = 1;

	printf("Input sequence: ");
	first = (list_t *)malloc(sizeof(list_t));
	if (!first) return 1;
	first->next = NULL;
	if (scanf("%d", &first->info) != 1)
		return 1;
	cur = first;

	while (scanf("%d", &temp)){
		cur->next = (list_t *)malloc(sizeof(list_t));
		if (!cur->next)
			return 1;

		cur->next->info = temp;
		cur = cur->next;
		cur->next = NULL;
		count++;
	}

	last = first;
	return count;
}


tree_t* freeTree(tree_t *node){
	if (node == NULL) return NULL;

	freeTree(node->left);
	freeTree(node->right);
	free(node);
	return NULL;
}

void clearList(){
	list_t *cur, *temp;

	for (cur = first; cur;){
		temp = cur;
		cur = cur->next;
		free(temp);
	}
	first = NULL;
}
