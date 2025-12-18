#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _node* Position;

typedef struct _node {
	int val;
	Position left;
	Position right;
} Node;

Position CreateNode(int value);
Position insert(Position root, int value);
int inorder(Position root);
int preorder(Position root);
int postorder(Position root);
void levelOrder(Position root);
Position find(Position root, int value);
Position findMin(Position root);
Position delete(Position root, int value);

int main() {

	Position root = NULL;
	int choice = 0;
	int value = 0;

	do {
		printf("\n1 Insert");
		printf("\n2 Inorder");
		printf("\n3 Preorder");
		printf("\n4 Postorder");
		printf("\n5 Level order");
		printf("\n6 Find");
		printf("\n7 Delete");
		printf("\n0 Exit");
		printf("\nChoice: ");
		scanf("%d", &choice);

		switch (choice) {

		case 1:
			printf("Value: ");
			scanf("%d", &value);
			root = insert(root, value);
			break;

		case 2:
			inorder(root);
			break;

		case 3:
			preorder(root);
			break;

		case 4:
			postorder(root);
			break;

		case 5:
			levelOrder(root);
			break;

		case 6:
			printf("Value: ");
			scanf("%d", &value);
			if (find(root, value))
				printf("Found");
			else
				printf("Not found");
			break;

		case 7:
			printf("Value: ");
			scanf("%d", &value);
			root = delete(root, value);
			break;
		}

	} while (choice != 0);

	return 0;
}


Position CreateNode(int value) {
	Position newNode = (Position)malloc(sizeof(Node));
	newNode->val = value;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

Position insert(Position root, int value) {
	if (root == NULL)
		return CreateNode(value);

	if (value < root->val)
		root->left = insert(root->left, value);
	if (value > root->val)
		root->right = insert(root->right, value);

	return root;
}

int inorder(Position root) {
	if (root) {
		inorder(root->left);
		printf(" %d", root->val);
		inorder(root->right);
	}
	return EXIT_SUCCESS;
}

int preorder(Position root) {
	if (root) {
		printf(" %d", root->val);
		preorder(root->left);
		preorder(root->right);
	}
	return EXIT_SUCCESS;
}

int postorder(Position root) {
	if (root) {
		postorder(root->left);
		postorder(root->right);
		printf(" %d", root->val);
	}
	return EXIT_SUCCESS;
}

void levelOrder(Position root) {
	if (root == NULL)
		return;

	Position queue[100];
	int front = 0, rear = 0;
	queue[rear++] = root;

	while (front < rear) {
		Position current = queue[front++];
		printf(" %d", current->val);

		if (current->left)
			queue[rear++] = current->left;
		if (current->right)
			queue[rear++] = current->right;
	}
}

Position find(Position root, int value) {
	if (root == NULL || root->val == value)
		return root;

	if (value < root->val)
		return find(root->left, value);
	else
		return find(root->right, value);
}

Position findMin(Position root) {
	while (root && root->left)
		root = root->left;
	return root;
}

Position delete(Position root, int value) {
	if (root == NULL)
		return root;

	if (value < root->val)
		root->left = delete(root->left, value);
	else if (value > root->val)
		root->right = delete(root->right, value);
	else {
		if (root->left == NULL) {
			Position temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			Position temp = root->left;
			free(root);
			return temp;
		}

		Position temp = findMin(root->right);
		root->val = temp->val;
		root->right = delete(root->right, temp->val);
	}
	return root;
}


