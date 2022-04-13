#include "graph.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include "help_functions.h"
using namespace std;

typedef struct list {
	int num;
	list* next;
}list;

typedef struct gr_node {
	int num;
	list* adj_list;
	gr_node* next;
}gr_node;

typedef struct graph {
	int count;
	gr_node* node;
}graph;

void graph_free(graph* head) {
	gr_node* node_ptr = head->node;
	while (node_ptr) {
		list* list_ptr = node_ptr->adj_list;
		while (list_ptr) {
			list* temp = list_ptr;
			list_ptr = list_ptr->next;
			free(temp);
		}
		gr_node* temp = node_ptr;
		node_ptr = node_ptr->next;
		free(temp);
	}
	free(head);
	return;
}

void add_node(gr_node** head, int num, list* adj) {
	gr_node* new_node = (gr_node*)malloc(sizeof(gr_node));
	new_node->num = num;
	new_node->adj_list = adj;
	new_node->next = NULL;
	if (!(*head)) {
		*head = new_node;
		return;
	}
	gr_node* cur = *head;
	while (cur->next) cur = (cur)->next;
	cur->next = new_node;
	return;
}

void add_adj(list** head, int num) {
	list* temp = (list*)malloc(sizeof(list));
	temp->num = num;
	temp->next = *head;
	*head = temp;
	return;
}

list* adj_list_create(int cur, int size) {
	int input = 1;
	int len = 0;
	int* adj = str_to_mas_int(&input, &len);
	list* node_adj = NULL;
	for (int i = 0; i < len; ++i) {
		if (0 < adj[i] && adj[i] <= size && adj[i] != cur) add_adj(&node_adj, adj[i]);
	}
	if (node_adj) return node_adj;
	node_adj = (list*)malloc(sizeof(list));
	node_adj->num = -1;
	return node_adj;
}

graph* graph_create(int size) {
	graph* new_graph = (graph*)malloc(sizeof(graph));
	gr_node* gr_head = NULL;
	int num = 1;
	int emp = 1;
	while (num <= size) {
		list* node_adj = adj_list_create(num, size);
		add_node(&gr_head, num, node_adj);
		++num;
		if (node_adj->num != -1) emp = 0;
	}
	new_graph->node = gr_head;
	new_graph->count = size;
	if (!emp) return new_graph;
	else return NULL;
}

void add_arc(graph* head, int a, int b) {
	while (head->node->num != a) head->node = head->node->next;
	add_adj(&head->node->adj_list, b);
	return;
}

void add_edge(graph* head, int a, int b) {
	if (a <= head->count && b <= head->count) {
		add_arc(head, a, b);
		add_arc(head, b, a);
	}
	return;
}

void del_arc(graph* head, int a, int b) {
	while (head->node->num != a) head->node = head->node->next;
	list* prev = NULL;
	while (head->node->adj_list->num != b) {
		prev = head->node->adj_list;
		head->node->adj_list = head->node->adj_list->next;
	}
	prev->next = head->node->adj_list->next;
	return;
}

void del_edge(graph* head, int a, int b) {
	if (a <= head->count && b <= head->count) {
		del_arc(head, a, b);
		del_arc(head, b, a);
	}
	return;
}

void graph_print(graph* gr) {
	if (gr) {
		cout << '\n';
		for (int i = 0; i < gr->count; ++i) {
			cout << gr->node->num << ": ";
			if (gr->node->adj_list->num == -1) cout << "There is no adjacity" << '\n';
			else {
				while (gr->node->adj_list->next) {
					cout << gr->node->adj_list->num << ", ";
					gr->node->adj_list = gr->node->adj_list->next;
				}
				cout << gr->node->adj_list->num << '\n';
			}
			gr->node = gr->node->next;
		}
	}
	else cout << "Graph is empty" << '\n';
	return;
}
