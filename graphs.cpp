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

#include "graph.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include "help_functions.h"
using namespace std;

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

void add_adj(list** adj_list, int num) {
	if ((*adj_list)->head->num == -1) {
		(*adj_list)->head->num = num;
		(*adj_list)->head->next = NULL;
		return;
	}

	grph_node* cur = (*adj_list)->head;
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
		if (cur == (*adj_list)->head) {
			grph_node* temp = get_list_elem(cur, num);
			(*adj_list)->head = temp;
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
		grph_node* node_ptr = grph[a].adj_list->head;
		while (node_ptr && node_ptr->num != b) {
			prev = node_ptr;
			node_ptr = node_ptr->next;
		}
		if (prev) {
			if (node_ptr) prev->next = node_ptr->next;
			else prev->next = NULL;
		}
		else (adj_list_ptr->head = adj_list_ptr->head->next);
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

graph* edged_graph(graph* grph) {
	if (grph) {
		graph* res = (graph*)malloc(sizeof(grph));
		res->count = grph->count;
		for (int i = 0; i < grph->count; ++i) {
			list* temp = (list*)malloc(sizeof(grph_node) * grph->count);
			list* list_ptr = grph[i].adj_list;
			grph_node* node = list_ptr->head;
			temp->head = get_list_elem(NULL, -1);
			while (node) {
				add_adj(&temp, node->num);
				node = node->next;
			}
			res[i].adj_list = temp;
		}
		for (int i = 0; i < grph->count; ++i) {
			list* list_ptr = res[i].adj_list;
			grph_node* node = list_ptr->head;
			while (node) {
				add_edge(res, i, node->num);
				node = node->next;
			}
		}
		return res;
	}
	return NULL;
}
//
//list* nodes_queue(graph* grph) {
//	list* res = (list*)malloc(sizeof(grph_node) * grph->count);
//	for (int i = 0; i < grph->count; ++i) {
//
//	}
//}

grph_node* q_pop(list** q) {
	grph_node* res = (*q)->head;
	(*q)->head = (*q)->head->next;
	return res;
}

void q_push(list** queue, grph_node* node) {
	list* cur = *queue;
	while (cur->head->next) cur->head = cur->head->next;
	cur->head = node;
}

int bfs_bpt_check(graph* grph) {
	int res = 1;
	if (grph) {
		graph* edged = edged_graph(grph);
		int* node_color = (int*)malloc(sizeof(int) * grph->count);
		for (int i = 0; i < edged->count; ++i) node_color[i] = -1;
		list* q = (list*)malloc(sizeof(grph_node) * edged->count);
		q->head = get_list_elem(NULL, 0);
		node_color[0] = 1;
		while (q->head) {
			list* adj_list_ptr = edged[q->head->num].adj_list;
			grph_node* node_ptr = adj_list_ptr->head;
			if (node_ptr->num != -1) {
				while (node_ptr) {
					if (node_color[node_ptr->num] == -1) node_color[node_ptr->num] = 1 - node_color[q->head->num];
					if (node_color[node_ptr->num] == -1) q_push(&q, node_ptr);
					node_ptr = node_ptr->next;
				}
				if (node_color[q->head->num] == node_color[node_ptr->num]) {
					res = 0;
					break;
				}
			}
		}
	}
	return res;
}
