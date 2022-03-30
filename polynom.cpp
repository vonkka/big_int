
#include <stdlib.h>
#include <iostream>
#include "RPN.h"
#include <ctype.h>
#include "euclid.h"
using namespace std;

int ftn_exist(const char* input, char prev) {
	if (op_priority(prev) == 2 || op_priority(prev) == 3 || prev == '(' || prev == ',') {
		++input;
		if (*input == 'c') {
			++input;
			if (*input == 'd') return 1;
			else if (*input == 'm') return 2;
		}
	}
	return 0;
}

int op_priority(char op) {
	switch (op) {
	case '(': return 1;
	case '+': return 2;
	case '-': return 2;
	case '*': return 3;
	case '/': return 3;
	case 'g': return 4;
	case 'l': return 4;
	case ')': return 5;
	case ',': return 6;
	default:
		break;
	}
}

void rpn_add(rpn** head, int num, char symb) {
	if (num || symb) {
		if ((*head)->next == NULL && (*head)->num == 0 && (*head)->symb == 0) {
			(*head)->num = num;
			(*head)->symb = symb;
			return;
		}
		rpn* new_elem = (rpn*)malloc(sizeof(rpn));
		new_elem->num = num;
		new_elem->symb = symb;
		new_elem->next = NULL;
		rpn* cur = *head;
		while (cur->next) cur = cur->next;
		cur->next = new_elem;
		return;
	}
	return;
}

void ops_add(ops** head, rpn** rhead, char op) {
	if (!(*head)) {
		(*head) = (ops*)malloc(sizeof(ops));
		(*head)->op = op;
		(*head)->next = NULL;
		return;
	}
	if ((*head)->next == NULL && (*head)->op == 0) {
		(*head)->op = op;
		(*head)->next = NULL;
		return;
	}

	ops* new_elem = (ops*)malloc(sizeof(ops));
	new_elem->op = op;
	if (op_priority(op) > op_priority((*head)->op) || op == '(') {
		if (op_priority(op) > 4) {
			while ((*head) && (*head)->op != '(') {
				rpn_add(rhead, 0, (*head)->op);
				ops_pop(head);
			}
			if (op == ')') {
				ops_pop(head);
				if (*head && (((*head)->op == 'g') || ((*head)->op == 'l'))) {
					rpn_add(rhead, 0, (*head)->op);
					ops_pop(head);
				}
			}
		}
		else {
			new_elem->next = (*head);
			(*head) = new_elem;
		}
		return;
	}
	while ((*head) && op_priority(op) <= op_priority((*head)->op)) {
		rpn_add(rhead, 0, (*head)->op);
		ops_pop(head);
	}
	if (!(*head)) {
		(*head) = (ops*)realloc(*head, sizeof(ops));
		(*head)->op = op;
		(*head)->next = NULL;
		return;
	}
	if (new_elem->op != ')') {
		new_elem->next = (*head);
		(*head) = new_elem;
	}
	return;
}

char rpn_pop(rpn** head) {
	char symb = (*head)->symb;
	(*head) = (*head)->next;
	return symb;
}

char ops_pop(ops** head) {
	if (*head) {
		char symb = (*head)->op;
		(*head) = (*head)->next;
		return symb;
	}
}

rpn* rpn_get(const char* input, int* err_ind) {
	rpn* res = (rpn*)malloc(sizeof(rpn));
	ops* head = (ops*)malloc(sizeof(ops));
	res->num = 0;
	res->symb = 0;
	res->next = NULL;
	head->op = 0;
	head->next = NULL;

	char prev = 0;
	int sign = 0;
	int ind = 0;
	int opn_bt = 0;
	int cls_bt = 0;

	while (*input != '\n' && !(*err_ind)) {
		if (isdigit(*input)) {
			if (op_priority(prev) != 4 || prev != ')') {
				char* endp = NULL;
				int temp_num = strtol(input, &endp, 10);
				rpn_add(&res, temp_num, 0);
				ind += endp - input;
				input = endp;
				prev = *(input - 1);
			}
			else *err_ind = ind;
		}
		else if (op_priority(*input) == 2) {
			if ((*input) == '-') ++sign;
			++input;
			++ind;
			while (op_priority(*input) == 2) {
				if ((*input) == '-') ++sign;
				++input;
				++ind;
			}
			if (sign & 1) ops_add(&head, &res, '-');
			else ops_add(&head, &res, '+');
			prev = *(input - 1);
			sign = 0;
		}
		else if (*input == '*') {
			if (isdigit(prev) || prev == ')') {
				ops_add(&head, &res, *input);
				prev = *input;
				++input;
				++ind;
			}
			else *err_ind = ind;
		}
		else if (*input == '/') {
			if (isdigit(prev) || prev == ')') ops_add(&head, &res, *input);
			else *err_ind = ind;
			prev = *input;
			++input;
			++ind;
		}
		else if (*input == 'g' || *input == 'l') {
			if (ftn_exist(input, prev) == 1) ops_add(&head, &res, 'g');
			else ops_add(&head, &res, 'l');
			prev = *input;
			input += 3;
			ind += 3;
		}
		else if (*input == '(') {
			ops_add(&head, &res, '(');
			++opn_bt;
			prev = *input;
			++input;
			++ind;
		}
		else if (*input == ')') {
			if ((1 < op_priority(prev) < 6) || isdigit(*input)) {
				ops_add(&head, &res, *input);
				++cls_bt;
				if (cls_bt > opn_bt) *err_ind = ind;
				prev = *input;
				++input;
				++ind;
			}
			else *err_ind = ind;
		}
		else if (*input == ',') {
			if (isdigit(prev)) ops_add(&head, &res, *input);
			else *err_ind = ind;
			prev = *input;
			++input;
			++ind;
		}
		else *err_ind = ind;
	}
	ops_add(&head, &res, '+');
	return res;
}

void rpn_print(rpn* head) {
	while (head) {
		if (head->symb) cout << head->symb << ' ';
		else if (head->num) cout << head->num << ' ';
		head = head->next;
	}
	cout << '\n';
	return;
}

int rpn_count(rpn* head) {
	int res = 0;

	ct* chead = (ct*)malloc(sizeof(ct));
	chead->num = head->num;
	chead->next = NULL;
	head = head->next;

	ct* new_elem = (ct*)malloc(sizeof(ct));
	while (head) {
		if (head->num) {
			new_elem->num = head->num;
			new_elem->next = chead;
			*chead = new_elem;
		}
		else {
			int num1; int num2;
			num1 = chead->num;
			chead = chead->next;
			num2 = chead->num;
			chead = chead->next;
			if (head->symb == '+') {
				res += num1 + num2;
				new_elem->num = num1 + num2;
				new_elem->next = chead;
				chead = new_elem;
			}
			else if (head->symb == '-') {
				res += num2 - num1;
				new_elem->num = num2 - num1;
				new_elem->next = chead;
				chead = new_elem;
			}
			else if (head->symb == '*') {
				res += num1 * num2;
				new_elem->num = num1 * num2;
				new_elem->next = chead;
				chead = new_elem;
			}
			else if (head->symb == '/') {
				res += num2 / num1;
				new_elem->num = num2 / num1;
				new_elem->next = chead;
				chead = new_elem;
			}
			else if (head->symb == 'g') {
				res += gcd(num1, num2);
				new_elem->num = gcd(num1, num2);
				new_elem->next = chead;
				chead = new_elem;
			}
			else if (head->symb == 'l') {
				res += lcm(num1, num2);
				new_elem->num = lcm(num1, num2);
				new_elem->next = chead;
				chead = new_elem;
			}
		}
		head = head->next;
	}
	return res;
}
