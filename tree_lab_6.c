#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OK 0
#define NOT_OK 1
#define MAX_LENGTH 11

typedef struct Tree{
	char info[MAX_LENGTH];
	struct Tree *lSon;
	struct Tree *rSon;
}Tree_t;

typedef struct Fields{
	int self_id;
	char value[MAX_LENGTH];
	int parent_id;
}fields_t;

Tree_t *root;

int preorder(Tree_t *, int);
int inorder(Tree_t *, int);
int postorder(Tree_t *, int);
int treeHeight(Tree_t *, int);
Tree_t* freeTree(Tree_t *);
Tree_t* formTree(Tree_t *, FILE *, int);

int height;

int main(int argc, char *argv[]){
	FILE *f = NULL;
	char order[MAX_LENGTH];

	f = fopen(argv[1], "r+");
	if (!f) return NOT_OK;
	
	if ((root = formTree(root, f, 0)) == NULL){
		printf("Something went wrong!\n");
		return;
	}
	
	height = treeHeight(root, 0);

	printf("Input type of order\n(in - inorder, post - postorder, pre - preorder): ");
	scanf("%s", order);
	if (strcmp(order, "in") == 0)
		inorder(root, height);
	else if (strcmp(order, "post") == 0)
		postorder(root, height);
	else if (strcmp(order, "pre") == 0)
		preorder(root, height);
	else
		printf("Unknown order!");

	fclose(f);
	freeTree(root);
	printf("\n");
}


Tree_t* formTree(Tree_t *node, FILE *f, int pos){
	fields_t cur, parent;
	Tree_t *temp = node;
	int i, parentPos, childPos;

	fseek(f, pos, SEEK_SET);
	if (fscanf(f, "%d%s%d", &parent.self_id, parent.value, &parent.parent_id) == EOF) return NULL;

	temp = (Tree_t*)malloc(sizeof(Tree_t));
	if (!temp) return NULL;
	temp->lSon = temp->rSon = NULL;
	strcpy(temp->info, parent.value);


	for (i = 0; i < 2;){
		parentPos = ftell(f);
		if (fscanf(f, "%d%s%d", &cur.self_id, cur.value, &cur.parent_id) == EOF) 
			return temp;
		childPos = ftell(f);

		if (cur.parent_id == parent.self_id){
			if (!i){
				temp->lSon = formTree(temp->lSon, f, parentPos);
			}else{
				temp->rSon = formTree(temp->rSon, f, parentPos);
			}
			i++;
			fseek(f, childPos, SEEK_SET);
		}
	}

	return temp;
}	

int preorder(Tree_t *root_e, int lvl){
	int i;

	if (!root_e) return NOT_OK;

	printf("\n");
	if (root->rSon == root_e){
		lvl += height / 2;
		printf("\n\n");
	}
	for (i = 0; i < lvl; i++) printf("\t");
	printf("%s", root_e->info);
	preorder(root_e->lSon, lvl - 1);
	preorder(root_e->rSon, lvl + 1);
	return OK;
}

int inorder(Tree_t *root_e, int lvl){
	int i;

	if (root_e == NULL) return NOT_OK;

	inorder(root_e->lSon, lvl - 1);
	printf("\n");
	for (i = 0; i < lvl; i++) printf("\t");
	printf("%s", root_e->info);
	inorder(root_e->rSon, lvl - 1);
	return OK;
}

int postorder(Tree_t *root_e, int lvl){
	int i;

	if (root_e == NULL) return NOT_OK;

	postorder(root_e->rSon, lvl - 1);
	postorder(root_e->lSon, lvl + 1);
	printf("\n");
	for (i = 0; i < lvl; i++) printf("\t");
	printf("%s", root_e->info);
	if (lvl = height - 1) printf("\n\t");
	return OK;
}

int treeHeight(Tree_t *root_e, int lvl){
	int height_l, height_r;
	
	height_l = height_r = 0;

	if (root_e == NULL) return lvl;

	height_l += treeHeight(root_e->lSon, lvl + 1);
	height_r += treeHeight(root_e->rSon, lvl + 1);
	if (height_l < height_r)
		return height_r;
	else
		return height_l;

}

Tree_t* freeTree(Tree_t *root_e){
	if (root_e == NULL) return NULL;
	
	freeTree(root_e->lSon);
	freeTree(root_e->rSon);
	free(root_e);
	return NULL;
}
