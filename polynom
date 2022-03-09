#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "polynom.h"
using namespace std;

poly* poly_get_monomial(int coef, int exp) {
	poly* temp = (poly*)malloc(sizeof(poly));
	temp->coef = coef;
	temp->exp = exp;
	temp->next = NULL;
	return temp;
}

void poly_push(poly** head, int coef, int exp) {
	poly* temp = poly_get_monomial(coef, exp);
	temp->next = *head;
	*head = temp;
}

void poly_print(poly* head) {
	if (!(head->coef) && !(head->next)) {
		cout << '0';
		return;
	}
	if (head->exp) {
		if (head->coef == 1) cout << 'x';
		else if (head->coef == (-1)) cout << '-' << 'x';
		else cout << head->coef << 'x';
		if (head->exp != 1) cout << '^' << head->exp;
	}
	else cout << head->coef;
	head = head->next;
	if (!head) return;

	while (head->next) {
		if (head->coef > 0) cout << '+';
		if (head->exp) {
			if (head->coef == 1) cout << 'x';
			else if (head->coef == (-1)) cout << '-' << 'x';
			else cout << head->coef << 'x';
			if (head->exp != 1) cout << '^' << head->exp;
		}
		else cout << head->coef;
		head = head->next;
	}

	if (head->coef > 0) cout << '+';
	if (head->exp) {
		if (head->coef == 1) cout << 'x';
		else if (head->coef == (-1)) cout << '-' << 'x';
		else cout << head->coef << 'x';
		if (head->exp != 1) cout << '^' << head->exp;
	}
	else cout << head->coef;
	return;
}

void poly_add(poly** head, int coef, int exp) {
	if (coef == 0) return;
	poly* curr = *head;
	poly* prev = (poly*)malloc(sizeof(poly));
	prev->coef = 0;
	while (curr->next) {
		if (curr->exp > exp) {
			prev = curr;
			curr = curr->next;
		}
		else if (curr->exp == exp) {
			curr->coef += coef;
			if (!(curr->coef)) prev->next = curr->next;
			return;
		}
		else {
			poly* temp = poly_get_monomial(coef, exp);
			temp->next = curr;
			if (prev->coef) prev->next = temp;
			else (*head) = temp;
			return;
		}
	}
	if (curr->exp > exp) {
		poly* temp = poly_get_monomial(coef, exp);
		curr->next = temp;
	}
	else if (curr->exp == exp) {
		curr->coef += coef;
		if (!(curr->coef)) prev->next = curr->next;
	}
	else {
		if (curr->coef == 0) {
			(*head)->coef = coef;
			(*head)->exp = exp;
		}
		else {
			poly* temp = poly_get_monomial(coef, exp);
			if (prev->coef) {
				temp->next = curr;
				prev->next = temp;
			}
			else poly_push(head, coef, exp);
			int a = 5;
		}
	}
	return;
}

poly* poly_get(const char* str) {
	poly* result = poly_get_monomial(0,0);
	char temp_num[10];
	for (int i = 0; i < 10; ++i) temp_num[i] = '*';
	int k = 0;
	bool x_check = 0;
	bool sgn = 0;
	int coef_temp = 0;
	int exp_temp = 0;
	while(*str != '\0') {
		if (*str == 'x') {
			if (temp_num[0] == '*') coef_temp = 1;
			else coef_temp = atoi(temp_num);
			if (sgn) coef_temp = -coef_temp;
			for (int l = 0; l < k; ++l) temp_num[l] = '*';
			k = 0;
			x_check = 1;
		}
		else if (*str == '+' || *str == '-') {
			if (x_check) {
				if (temp_num[0] == '*') exp_temp = 1;
				else exp_temp = atoi(temp_num);
			}
			else {
				coef_temp = atoi(temp_num);
				if (sgn) coef_temp = -coef_temp;
				exp_temp = 0;
			}
			poly_add(&result, coef_temp, exp_temp);
			for (int l = 0; l < k; ++l) temp_num[l] = '*';
			k = 0;
			x_check = 0;
			if (*str == '-') sgn = 1;
			else sgn = 0;
		}
		else if (*str == '^');
		else {
			temp_num[k] = *str;
			++k;
		}
		str++;
	}

	if (x_check) {
		if (temp_num[0] == '*') exp_temp = 1;
		else exp_temp = atoi(temp_num);
	}
	else {
		coef_temp = atoi(temp_num);
		if (sgn) coef_temp = -coef_temp;
		exp_temp = 0;
	}
	poly_add(&result, coef_temp, exp_temp);
	for (int l = 0; l < k; ++l) temp_num[l] = 0;
	k = 0;
	if (*str == '-') sgn = 1;
	
	return result;
}

poly* poly_sum(poly* first, poly* sec) {
	while (sec) {
		poly_add(&first, sec->coef, sec->exp);
		sec = sec->next;
	}
	return first;
}

poly* poly_mult(poly* first, poly* sec) {
	poly* res = poly_get_monomial(0, 0);
	poly* cur;
	while (first) {
		poly* cur = sec;
		while (cur) {
			poly_add(&res, first->coef * cur->coef, first->exp + cur->exp);
			cur = cur->next;
		}
		first = first->next;
	}
	return res;
}
