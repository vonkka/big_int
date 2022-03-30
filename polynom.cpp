#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include "polynom.h"
using namespace std;

poly* poly_get_monomial(int coef, int exp) {
	poly* temp = (poly*)malloc(sizeof(poly));
	if (temp) {
		temp->coef = coef;
		temp->exp = exp;
		temp->next = NULL;
	}
	return temp;
}

void poly_push(poly** head, int coef, int exp) {
	poly* temp = poly_get_monomial(coef, exp);
	temp->next = *head;
	*head = temp;
}

void poly_monom_print(int coeff, int exp) {
	if (exp) {
		if (coeff == 1) cout << 'x';
		else if (coeff == (-1)) cout << '-' << 'x';
		else cout << coeff << 'x';
		if (exp != 1) cout << '^' << exp;
	}
	else cout << coeff;
}

void poly_print(poly* head) {
	if (!(head)) {
		cout << '0';
		return;
	}

	if (!(head->coef)) {
		if (!(head->next)) {
			cout << '0' << '\n';
			return;
		}
		head = head->next;
	}
	poly_monom_print(head->coef, head->exp);
	head = head->next;

	while (head) {
		if (head->coef > 0) cout << '+';
		poly_monom_print(head->coef, head->exp);
		head = head->next;
	}
	cout << '\n';
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
		}
	}
	return;
}

poly* poly_get(const char* str) {
	poly* result = poly_get_monomial(0, 0);
	char prev = NULL;
	int ind = 0;
	int temp_num = 1;
	bool x_check = 0;
	int sgn = 0;
	int coef_temp = 0;
	int exp_temp = 0;

	while (*str == '-' || *str == '+') {
		if (!(prev) || prev == '+' || prev == '-') {
			if (*str == '-') ++sgn;
			prev = *str;
			++ind;
			++str;
		}
		else throw ind;
	}

	while (*str != '\n') {
		if (*str == 'x') {
			if (!(prev) || (isdigit(prev)) || prev == '+' || prev == '-') {
				if (sgn & 1) coef_temp = -temp_num;
				else coef_temp = temp_num;
				x_check = 1;
				sgn = 0;
				temp_num = 1;
				prev = *str;
				++str;
				++ind;
			} 
			else throw ind;
		}
		else if (*str == '+' || *str == '-') {
			if (prev == '+' || prev == '-' || (isdigit(prev)) || prev == 'x') {
				if (x_check) exp_temp = temp_num;
				else {
					if (sgn & 1) coef_temp = -temp_num;
					else coef_temp = temp_num;
					exp_temp = 0;
					sgn = 0;
				}
				poly_add(&result, coef_temp, exp_temp);
				x_check = 0;
				temp_num = 1;
				while (*str == '-' || *str == '+') {
					if (*str == '-') ++sgn;
					prev = *str;
					++str;
					++ind;
				}
			}
			else throw ind;
		}
		else if (*str == '^') {
			if (prev == 'x') {
				prev = *str;
				++str;
				++ind;
			}
			else throw ind;
		}
		else if (isdigit(*str)) {
			if (prev == '+' || prev == '-' || prev == '^' || !(prev)) {
				char* endp = NULL;
				temp_num = strtol(str, &endp, 10);
				ind += endp - str;
				str = endp;
				prev = *(str - 1);
			}
			else throw ind;
		}
		else throw ind;
	}
	if (!ind) {
		result = NULL;
		return result;
	}
	else {
		if (x_check) exp_temp = temp_num;
		else {
			if (sgn & 1) coef_temp = -temp_num;
			else coef_temp = temp_num;
			exp_temp = 0;
		}
		poly_add(&result, coef_temp, exp_temp);
	}
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
