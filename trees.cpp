#include "trees.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
using namespace std;

node* create_node(int value) {
	node* root = (node*)malloc(sizeof(node));
	root->value = value;
	root->left = root->right = NULL;
	return root;
}

void add_node(node* cur, int value) {
	while (cur) {
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

node* create_tree(int* arr, int len) {
	node* root = create_node(arr[0]);
	for (int i = 1; i < len; ++i) add_node(root, arr[i]);
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

int* tree_sort(int* arr, int len) {
	node* root = create_tree(arr, len);
	int* mas = (int*)malloc(len * sizeof(int));
	int* temp = mas;
	get_sorted_list(root, &mas);
	mas = temp;
	return temp;	
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
	node->left = node->right = NULL;
	return node;
}

void sb_add_node(sb* cur, sb* prev_left, sb* prev_right, int depth) {
	if (depth) {
		cur->left = sb_create_node(prev_left->num + cur->num, prev_left->dnum + cur->dnum);
		cur->right = sb_create_node(prev_right->num + cur->num, prev_right->dnum + cur->dnum);
		sb_add_node(cur->left, prev_left, cur, depth - 1);
		sb_add_node(cur->right, cur, prev_right, depth - 1);
	}
	return;
}

sb* sb_create_tree(int depth) {
	if (depth > 0) {
		sb* root = sb_create_node(1, 1);
		sb* prev_left = sb_create_node(0, 1);
		sb* prev_right = sb_create_node(1, 0);
		sb_add_node(root, prev_left, prev_right, depth - 1);
		return root;
	}
	return NULL;
}

void sb_tree_print(sb* root) {
	if (root) {
		sb_tree_print(root->left);
		cout << root->num << '/' << root->dnum << ' ';
		sb_tree_print(root->right);
	}
	return;
}

char* approximate_value(double value, int depth) {
	char* res = (char*)malloc(depth);
	res[depth - 1] = '\0';
	sb* sb_node = sb_create_tree(depth);
	for (int i = 0; i < depth - 1; ++i) {
		double node_value = (double(sb_node->num) / sb_node->dnum);
		if (value == node_value) {
			res[i] = ' ';
			continue;
			//return res;
		}
		if (value < node_value) {
			sb_node = sb_node->left;
			res[i] = 'L';
		}
		else {
			sb_node = sb_node->right;
			res[i] = 'R';
		}
	}
	return res;
}

char* tree_approx_value(sb* sb_node, double value, int depth) {
	char* res = (char*)malloc(depth);
	res[depth] = '\0';
	for (int i = 0; i < depth - 1; ++i) {
		double node_value = (double(sb_node->num) / sb_node->dnum);
		if (value == node_value) {
			res[i] = 'M';
			return res;
		}
		if (value < node_value) {
			sb_node = sb_node->left;
			res[i] = 'L';
		}
		else {
			sb_node = sb_node->right;
			res[i] = 'R';
		}
	}
	return res;
}

// MAIN

    while (true) {
        double value;
        int depth;
        cout << "Enter the value: " << '\n';
        cin >> value;
        cout << "Enter the depth of the tree: " << '\n';
        cin >> depth;
        if (depth > 0) {
            char* way = approximate_value(value, depth);
            cout << way << '\n';
        }
        else cout << "Depth cannot be less than 1";
    }


    while (true) {
        int input = 1;
        int len = 0;
        int* mas = str_to_mas_int(&input, &len);
        if (input) {
            for (int i = 0; i < len; ++i) cout << mas[i] << ' ';
            cout << '\n';
            int* sorted_list = tree_sort(mas, len);
            for (int i = 0; i < len; ++i) cout << sorted_list[i] << ' ';
            cout << '\n';
            free(sorted_list);
        }
        else cout << "Incorrect input";
        cout << '\n';
    }
