#include <stdio.h>
#include <stdlib.h>

typedef struct Tree{
	int info;
	struct Tree *left;
	struct Tree *right;
	struct Tree *parent;
}tree_t;

tree_t *root;

int input(FILE *);
int printTree(tree_t *, int);
int treeHeight(tree_t *, int);
int createTree(int);
int deleteVertex(int);

int main(int argc, char *argv[]){
	FILE *f = NULL;
	int value, height;

	if (argc > 1)
		f = fopen(argv[1], "r+");
	
	input(f);
	
	if (f){
		fclose(f);
	}

	printf("\nTree before:\n");
	height = treeHeight(root, 0);
	printTree(root, height);
	
	fflush(stdin);
	printf("\nInput value of vertex: ");
	scanf("%d", &value);

	deleteVertex(value);

	printf("\nTree after:\n");
	height = treeHeight(root, 0);
	printTree(root, height);

	printf("\n");
}

int input(FILE *f){
	int value;

	if (f){
		printf("Vertices sequence: ");
		while (fscanf(f, "%d", &value) != EOF){
			printf("%d ", value);
			createTree(value);
		}
	}else{
		printf("Input sequence of vertices: ");
		while (scanf("%d", &value) == 1){
			createTree(value);
		}
	}

	return 0;
}

int deleteVertex(int value){
	tree_t *cur, *temp, *prev;

	for (cur = root; cur;){
		prev = cur;
		if (value > cur->info){
			if (cur->right)
				cur = cur->right;
		}else if(value < cur->info){
			if (cur->left)
				cur = cur->left;
		}

		if (cur->info == value){
			temp = cur;
			if (cur->right){
				cur = cur->right;
				for (; cur->left; cur = cur->left)
					prev = cur;
			}else if (cur->left){
				cur = cur->left;
				for (; cur->right; cur = cur->right)
					prev = cur;
			}

			if (cur->info > prev->info){
				prev->right = NULL;
			}else if(cur->info < prev->info){
				prev->left = NULL;
			}

			if (temp != cur || temp == root){
				if (temp != root){
					if (temp->info > temp->parent->info)
						temp->parent->right = cur;
					else if (temp->info < temp->parent->info)
						temp->parent->left = cur;
				}

				if (temp->left != cur)
					cur->left = temp->left;
				if (temp->right != cur)
					cur->right = temp->right;
			}

			if (temp == root)
				root = cur;

			free(temp);
			break;
		}
	}

	return 0;
}

int printTree(tree_t *root_e, int lvl){
	int i;

	if (root_e == NULL) return 1;

	printTree(root_e->left, lvl - 1);
	printf("\n");
	for (i = 0; i < lvl; i++) printf("\t");
	printf("%d", root_e->info);
	printTree(root_e->right, lvl - 1);
	return 0;
}

int treeHeight(tree_t *root_e, int lvl){
	int height_l, height_r;

	height_l = height_r = 0;

	if (root_e == NULL) return lvl;

	height_l += treeHeight(root_e->left, lvl + 1);
	height_r += treeHeight(root_e->right, lvl + 1);
	if (height_l < height_r)
		return height_r;
	else
		return height_l;

}

int createTree(int value){
	tree_t *cur = NULL, *parent;

	if (!root){
		cur = (tree_t*)malloc(sizeof(tree_t));
		if (!cur) return 1;

		cur->info = value;
		cur->left = cur->right = cur->parent = NULL;
		root = cur;
	}else{
		for (cur = root;;){
			parent = cur;
			if (value > cur->info){
				cur = cur->right;
			}
			else{
				cur = cur->left;
			}

			if (!cur){
				cur = (tree_t*)malloc(sizeof(tree_t));
				if (!cur) return 1;

				cur->info = value;
				cur->left = cur->right = NULL;
				cur->parent = parent;
				break;
			}
		}

		if (value > parent->info){
			parent->right = cur;
		}
		else{
			parent->left = cur;
		}
	}

	return 0;
}
