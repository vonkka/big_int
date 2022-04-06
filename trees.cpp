#include "trees.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
using namespace std;

typedef struct node {
	int value;
	node* left;
	node* right;
};

typedef struct sb {
	int num;
	int dnum;
	sb* left;
	sb* middle;
	sb* right;
};



node* create_node(int value) {
	node* root = (node*)malloc(sizeof(node));
	root->value = value;
	root->left = root->right = NULL;
	return root;
}

void add_node(node* cur, int value) {
	while (true) {
		if (value < cur->value) {
			if (!cur->left) {
				cur->left = create_node(value);
				break;
			}
			else cur = cur->left;
		}
		else {
			if (!cur->right) {
				cur->right = create_node(value);
				break;
			}
			else cur = cur->right;
		}
	}
	return;
}

node* create_tree(int len, int* list) {
	node* root = create_node(list[0]);
	for (int i = 1; i < len; ++i) add_node(root, list[i]);
	return root;
}

void get_sorted_list(node* root, int** mas) {
	if (root) {
		get_sorted_list(root->left, mas);
		**mas = root->value;
		++(*mas);
		get_sorted_list(root->right, mas);
	}
	return;
}

void print_tree(node* root) {
	if (root) {
		print_tree(root->left);
		cout << root->value << ' ';
		print_tree(root->right);
	}
	return;
}

sb* sb_create_node(int num, int dnum) {
	sb* node = (sb*)malloc(sizeof(sb));
	node->num = num;
	node->dnum = dnum;
	node->left = node->middle = node->right = NULL;
	return node;
}

void sb_add_node(sb* cur, sb* prev_left, sb* prev_right, int depth) {
	if (depth > 0) {
		cur->left = sb_create_node(prev_left->num + cur->num, prev_left->dnum + cur->dnum);
		cur->right = sb_create_node(prev_right->num + cur->num, prev_right->dnum + cur->dnum);
		sb_add_node(cur->left, prev_left, cur, depth - 1);
		sb_add_node(cur->right, cur, prev_right, depth - 1);
	}
	else return;
}

sb* sb_create_tree(int depth) {
	if (depth > 0) {
		sb* root = sb_create_node(1, 1);
		for (int i = 1; i <= depth; ++i) sb_add_node(root, i);
	}
}
