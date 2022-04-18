#include "graph.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include "help_functions.h"
using namespace std;

typedef struct grph_node {
	int num;
	grph_node* next;
}grph_node;

typedef struct list {
	grph_node* head;
}list;

typedef struct graph {
	int count;
	list* adj_list;
}graph;

void graph_free(graph* grph) {
	if (grph) {
		for (int i = 0; i < grph->count; ++i) {
			list* list_ptr = grph[i].adj_list;
			while (list_ptr->head) {
				grph_node* node_ptr = list_ptr->head;
				list_ptr->head = list_ptr->head->next;
				free(node_ptr);
			}
			free(list_ptr);
		}
		free(grph);
	}
	return;
}

grph_node* get_list_elem(grph_node* next, int num) {
	grph_node* temp = (grph_node*)malloc(sizeof(grph_node));
	temp->num = num;
	temp->next = next;;
	return temp;
}

void add_adj(list** head, int num) {
	if ((*head)->head->num == -1) {
		(*head)->head->num = num;
		(*head)->head->next = NULL;
		return;
	}

	grph_node* cur = (*head)->head;
	grph_node* prev = NULL;
	while (cur && cur->num < num) {
		prev = cur;
		cur= cur->next;
	}
	if (!cur) {
		grph_node* temp = get_list_elem(NULL, num);
		prev->next = temp;
		return;
	}
	if (cur->num > num) {
		if (cur == (*head)->head) {
			grph_node* temp = get_list_elem(cur, num);
			(*head)->head = temp;
			return;
		}
		grph_node* temp = get_list_elem(cur, num);
		prev->next = temp;
	}
	return;
}

list* adj_list_create(int cur, int size) {
	int input = 1;
	int len = 0;
	int* adj = str_to_mas_int(&input, &len);
	if (input) {
		list* node_adj = (list*)malloc(sizeof(grph_node) * size);
		node_adj->head = get_list_elem(NULL, -1);
		for (int i = 0; i < len; ++i) {
			if (0 <= adj[i] && adj[i] < size && adj[i] != cur) add_adj(&node_adj, adj[i]);
		}
		return node_adj;
	}
	return NULL;
}

graph* graph_create(int size) {
	graph* new_graph = (graph*)malloc(sizeof(list) * size);
	if (size > 0) {
		new_graph->count = size;
		for (int num = 0; num < size; ++num) new_graph[num].adj_list = adj_list_create(num, size);
		return new_graph;
	}
	return NULL;
}

void add_arc(graph* grph, int a, int b) {
	if (a < grph->count) {
		list* temp = grph[a].adj_list;
		add_adj(&temp, b);
	}
	return;
}

void add_edge(graph* grph, int a, int b) {
	add_arc(grph, a, b);
	add_arc(grph, b, a);
	return;
}

void del_arc(graph* grph, int a, int b) {
	if (a < grph->count && b < grph->count) {
		grph_node* prev = NULL;
		list* adj_list_ptr = grph[a].adj_list;
		grph_node* node_ptr = adj_list_ptr->head;
		while (node_ptr->num != b) {
			prev = node_ptr;
			node_ptr = node_ptr->next;
		}
		prev->next = node_ptr->next;
	}
	return;
}

void del_edge(graph* grph, int a, int b) {
	del_arc(grph, a, b);
	del_arc(grph, b, a);
	return;
}

void graph_print(graph* grph) {
	if (grph) {
		cout << '\n';
		for (int i = 0; i < grph->count; ++i) {
			cout << i << ": ";
			list* adj_list_ptr = grph[i].adj_list;
			grph_node* grph_node_ptr = adj_list_ptr->head;
			while (grph_node_ptr->next) {
				cout << grph_node_ptr->num << ", ";
				grph_node_ptr = grph_node_ptr->next;
			}
			cout << grph_node_ptr->num << '\n';
		}
	}
	else cout << "Graph is empty" << '\n';
	return;
}

//MAIN

while (true) {
        cout << "Enter the graph size: ";
        int size;
        cin >> size;
        graph* check_gr = graph_create(size);
        graph_print(check_gr);
        del_arc(check_gr, 1, 3);
        graph_print(check_gr);
        del_arc(check_gr, 3, 1);
        graph_print(check_gr);
        add_edge(check_gr, 1, 3);
        graph_print(check_gr);
        graph_free(check_gr);
    }
