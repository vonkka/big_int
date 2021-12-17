#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "big_int.h"
using namespace std;

unsigned char count_cell(unsigned char first, unsigned char sec, unsigned char* temp) {
    unsigned char res = 0;
    unsigned char sum = 0;
    unsigned char comp = 1;
    unsigned char f_cell = 0;
    unsigned char s_cell = 0;
    for (unsigned int k = 0; k < 8; ++k) {
        f_cell = ((first & comp) ? 1 : 0);
        s_cell = ((sec & comp) ? 1 : 0);
        sum = f_cell + s_cell + *temp;
        if (sum > 1) *temp = 1;
        else *temp = 0;
        if (sum & 1) res |= (comp);
        comp <<= 1;
    }
    return res;
}

char long_short_def(big_int* first, big_int* long_num, big_int* sec, big_int* short_num, bool minus) {
    unsigned char comp = 0x80;
    long_num->length = 1;
    long_num->sign = '+';
    long_num->number = (unsigned char*)malloc(long_num->length);
    short_num->length = 1;
    short_num->sign = '+';
    short_num->number = (unsigned char*)malloc(short_num->length);
    if (first->length > sec->length) {
        big_int_assign(long_num, first);
        big_int_assign(short_num, sec);
        if (minus) return '+';
    }
    else if (first->length == sec->length) {
        char k1 = 0; char k2 = 0;
        for (; comp; comp >>= 1) {
            if (comp == 1) {
                big_int_assign(long_num, first);
                big_int_assign(short_num, sec);
                if (minus) return '+';
                break;
            }
            k1 = ((first->number[first->length - 1] & comp) ? 1 : 0);
            k2 = ((sec->number[sec->length - 1] & comp) ? 1 : 0);
            if (k1 > k2) {
                big_int_assign(long_num, first);
                big_int_assign(short_num, sec);
                if (minus) return '+';
                break;
            }
            else if (k1 < k2) {
                big_int_assign(long_num, sec);
                big_int_assign(short_num, first);
                if (minus) return '-';
                break;
            }
        }
    }
    else {
        big_int_assign(long_num, sec);
        big_int_assign(short_num, first);
        if (minus) return '-';
    }
}

big_int* big_int_sum(big_int* first, big_int* sec) {
    if (first->sign == '+' && sec->sign == '-') {
        sec->sign = '+';
        return big_int_sub(first, sec);
    }
    else if (first->sign == '-' && sec->sign == '+') {
        first->sign = '+';
        return big_int_sub(sec, first);
    }

    big_int* res = (big_int*)malloc(sizeof(big_int));
    big_int* long_num = (big_int*)malloc(sizeof(big_int));
    big_int* short_num = (big_int*)malloc(sizeof(big_int));
    long_short_def(first, long_num, sec, short_num, 0);
    res->sign = long_num->sign;
    res->length = long_num->length;
    res->number = (unsigned char*)malloc(res->length);

    unsigned char temp = 0;
    unsigned char comp = 1;
    for (unsigned char i = 0; i < short_num->length; ++i) {
        res->number[i] = count_cell(long_num->number[i], short_num->number[i], &temp);
    }
    for (unsigned char i = short_num->length; i < res->length; ++i) {
        res->number[i] = count_cell(long_num->number[i], 0, &temp);
    }
    if (temp) {
        unsigned char ind = 0;
        comp = 1;
        for (unsigned char k = 0; k < 8; ++k) {
            if (long_num->number[res->length - 1] & comp) ind = k;
            comp <<= 1;
        }
        if (ind == 7) {
            ++res->length;
            res->number = (unsigned char*)realloc(res->number, res->length);
            res->number[res->length - 1] = 1;
        }
        else res->number[res->length - 1] |= (1 << (ind + 1));
    }
    return res;
}
