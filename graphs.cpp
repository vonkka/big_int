#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include "graph.h"
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

void add_adj(grph_node** head, int num) {
	if (*head) {
		grph_node* cur = *head;
		grph_node* prev = NULL;
		while (cur && cur->num < num) {
			prev = cur;
			cur = cur->next;
		}
		if (!cur) {
			grph_node* temp = get_list_elem(NULL, num);
			prev->next = temp;
			return;
		}
		if (cur->num > num) {
			if (cur == *head) {
				grph_node* temp = get_list_elem(cur, num);
				*head = temp;
				return;
			}
			grph_node* temp = get_list_elem(cur, num);
			prev->next = temp;
		}
	}
	else *head = get_list_elem(NULL, num);
	return;
}

graph* graph_create(int size) {
	graph* new_graph = (graph*)malloc(sizeof(list) * size);
	if (size > 0) {
		new_graph->count = size;
		new_graph->adj_list = (list*) malloc(size * sizeof(grph_node));
		for (int num = 0; num < size; ++num) new_graph->adj_list[num].head = NULL;
		return new_graph;
	}
	return NULL;
}

void add_arc(graph* grph, int a, int b) {
	if (a < grph->count && b < grph->count) {
		add_adj(&grph->adj_list[a].head, b);
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
		grph_node* node_ptr = grph->adj_list[a].head;
		while (node_ptr && node_ptr->num != b) {
			prev = node_ptr;
			node_ptr = node_ptr->next;
		}
		if (prev) {
			if (node_ptr) prev->next = node_ptr->next;
			else prev->next = NULL;
		}
		else (grph->adj_list[a].head = NULL);
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
			grph_node* grph_node_ptr = grph->adj_list[i].head;
			if (grph_node_ptr) {
				while (grph_node_ptr->next) {
					cout << grph_node_ptr->num << ", ";
					grph_node_ptr = grph_node_ptr->next;
				}
				cout << grph_node_ptr->num;
			}
			cout << '\n';
		}
	}
	else cout << "Graph is empty" << '\n';
	return;
}

graph* edged_graph(graph* grph) {
	if (grph) {
		graph* res = (graph*)malloc(sizeof(grph));
		res->adj_list = (list*)malloc(sizeof(list) * grph->count);
		res->count = grph->count;
		for (int i = 0; i < grph->count; ++i) {
			list* temp = (list*)malloc(sizeof(grph_node) * grph->count);
			grph_node* head = grph->adj_list[i].head;
			temp->head = NULL;
			while (head) {
				add_adj(&temp->head, head->num);
				head = head->next;
			}
			if (temp->head) res->adj_list[i].head = temp->head;
			else res->adj_list[i].head = NULL;
		}
		for (int i = 0; i < grph->count; ++i) {
			grph_node* head = res->adj_list[i].head;
			while (head) {
				add_edge(res, i, head->num);
				head = head->next;
			}
		}
		return res;
	}
	return NULL;
}

que* q_elem_add(int num, que* ptr) {
	que* temp = (que*)malloc(sizeof(que*));
	temp->num = num;
	temp->next = ptr;
	return temp;
}

void q_pop(que** q) {
	(*q) = (*q)->next;
	return;
}

void q_push(que** q, int num) {
	if (!(*q)) {
		*q = q_elem_add(num, NULL);
		return;
	}
	que* cur = *q;
	while (cur->next) cur = cur->next;
	cur->next = q_elem_add(num, NULL);
	return;
}

int bfs_bpt_check(graph* grph) {
	int res = 1;
	if (grph) {
		graph* edged = edged_graph(grph);
		int* node_color = (int*)malloc(sizeof(int) * grph->count);
		for (int i = 0; i < edged->count; ++i) node_color[i] = -1;
		que* q = NULL;
		q_push(&q, 0);
		node_color[0] = 1;
		while (q) {
			grph_node* node_ptr = edged->adj_list[q->num].head;
			while (node_ptr) {
				if (node_color[node_ptr->num] == -1) {
					q_push(&q, node_ptr->num);
					node_color[node_ptr->num] = 1 - node_color[q->num];
				}
				if (node_color[q->num] == node_color[node_ptr->num]) {
					res = 0;
					break;
				}
				node_ptr = node_ptr->next;
			}
			q_pop(&q);
		}
	}
	return res;
}
