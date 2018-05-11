#include<stdio.h>
#include<stdlib.h>

struct node
{
	int data;
	struct node* left;
	struct node* right;
};

int maxi(int inorder[], int strt, int end);

struct node* newNode(int data);

struct node* buildTree(int inorder[], int start, int end)
{
	if (start > end)
		return NULL;

	int i = maxi(inorder, start, end);

	// Максимальный элемент в списке становится корнем
	struct node *root = newNode(inorder[i]);

	if (start == end)
		return root;

	// Строит левое и правое поддеревья
	root->left = buildTree(inorder, start, i - 1);
	root->right = buildTree(inorder, i + 1, end);

	return root;
}

// Находит индекс максимального элемента 
int maxi(int arr[], int strt, int end)
{
	int i, max = arr[strt], maxind = strt;
	for (i = strt + 1; i <= end; i++)
	{
		if (arr[i] > max)
		{
			max = arr[i];
			maxind = i;
		}
	}
	return maxind;
}

// Выделяет память для нового элемента
struct node* newNode(int data)
{
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->data = data;
	node->left = NULL;
	node->right = NULL;

	return node;
}

int inorderF(struct node *root_e, int lvl)
{
	int i;

	if (root_e == NULL) return 1;

	inorderF(root_e->left, lvl - 1);
	printf("\n");
	for (i = 0; i < lvl; i++) printf("\t");
	printf("%d", root_e->data);
	inorderF(root_e->right, lvl - 1);
	return 0;
}

int treeHeight(struct node *root_e, int lvl)
{
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

int main()
{
	int *seq = NULL, i, len = 20;

	seq = (int *)malloc(20 * sizeof(int));
	if (!seq) return 1;

	printf("Input sequence: ");
	for (i = 0; i < 20; i++){
		if (scanf("%d", &seq[i]) != 1){
			len = i;
			break;
		}
	}

	struct node *root = buildTree(seq, 0, len - 1);

	printf("\nTree:\n");

	inorderF(root, treeHeight(root, 0));

	printf("\n");
	return 0;
}
