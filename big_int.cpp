#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "big_int.h"
using namespace std;

unsigned char count_cell(unsigned char first, unsigned char sec, unsigned char* temp, char sign) {
    unsigned char res = 0;
    char res_cell = 0;
    unsigned char comp = 1;
    unsigned char f_cell = 0;
    unsigned char s_cell = 0;
    if (sign == '+') {
        for (unsigned int k = 0; k < 8; ++k) {
            f_cell = ((first & comp) ? 1 : 0);
            s_cell = ((sec & comp) ? 1 : 0);
            res_cell = f_cell + s_cell + *temp;
            if (res_cell > 1) *temp = 1;
            else *temp = 0;
            if (res_cell & 1) res |= (comp);
            comp <<= 1;
        }
    }
    else {
        for (unsigned char k = 0; k < 8; ++k) {
            f_cell = ((first & comp) ? 1 : 0);
            s_cell = ((sec & comp) ? 1 : 0);
            res_cell = f_cell - s_cell - *temp;
            if (res_cell == 1) {
                res |= comp;
                *temp = 0;
            }
            else if (res_cell == 0) {
                *temp = 0;
            }
            else if (res_cell == -1) {
                res |= comp;
                *temp = 1;
            }
            else {
                *temp = 1;
            }
            comp <<= 1;
        }
    }
    return res;
}

char long_short_def(big_int* first, big_int* long_num, big_int* sec, big_int* short_num, bool minus) {
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
        for (char i = first->length - 1; i >= 0; --i) {
            for (unsigned char comp = 0x80; comp; comp >>= 1) {
                k1 = ((first->number[i] & comp) ? 1 : 0);
                k2 = ((sec->number[i] & comp) ? 1 : 0);
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
                if ((i == 0) && (comp == 1)) {
                    big_int_assign(long_num, first);
                    big_int_assign(short_num, sec);
                    if (minus) return '+';
                    break;
                }
            }
        }
    }
    else {
        big_int_assign(long_num, sec);
        big_int_assign(short_num, first);
        if (minus) return '-';
    }
}

void big_int_free(big_int* n) {
    free(n->number);
    free(n);
}

big_int* big_int_get(const char* binary) {
    big_int* result = (big_int*)malloc(sizeof(big_int));
    if (binary[0] == '-') {
        result->sign = '-';
        ++binary;
    }
    else result->sign = '+';
    unsigned int len = strlen(binary);
    unsigned char rem = len & 7;
    result->length = (len >> 3) + (rem > 0);
    result->number = (unsigned char*)malloc(result->length);  // big razryadi - last

    unsigned int end = result->length;                               // small razryadi - pervie
    bool zero_last = 0;
    unsigned char zero_count = 0;
    if (rem) {
        result->number[--end] = 0;
        for (char k = rem - 1; k >= 0; --k) {
            if (binary[k] == '1') {
                result->number[end] |= (1 << (rem - k - 1));
            }
        }
        if (result->number[end] == 0) {
            zero_last = 1;
        }
    }
    char ind = 0;
    for (unsigned int i = 0; i < end; ++i) {
        result->number[i] = 0;
        for (char k = 0; k < 8; ++k) {
            ind = len - 1 - k - (i << 3);
            if (binary[ind] == '1') {
                result->number[i] |= (1 << k);
            }
        }

        if (result->number[i] == 0) {
            ++zero_count;
        }
        else {
            zero_count = 0;
        }
    }
    if (zero_last && (result->length > 1)) {
        result->length -= (zero_count + 1);
        result->number = (unsigned char*)realloc(result->number, result->length);
    }
    return result;
}


void big_int_assign(big_int* wh, big_int* fr) {
    wh->length = fr->length;
    wh->sign = fr->sign;
    free(wh->number);
    wh->number = (unsigned char*)malloc(wh->length);
    memcpy(wh->number, fr->number, wh->length);
}

void big_int_print(big_int* num) {
    unsigned char comp = 0x80;
    if (num->length == 0) {
        cout << '0' << '\n';
        return;
    }
    if (num->sign == '-') {
        cout << '-';
    }
    for (; comp; comp >>= 1) {
        if ((num->number[num->length - 1] & comp) ? 1 : 0) break;
    }
    for (; comp; comp >>= 1) {
        cout << ((num->number[num->length - 1] & comp) ? 1 : 0);
    }
    for (int i = num->length - 2; i >= 0; --i) {
        comp = 0x80;
        for (; comp; comp >>= 1) {
            cout << ((num->number[i] & comp) ? 1 : 0);
        }
    }
    cout << '\n';
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
    for (unsigned char i = 0; i < short_num->length; ++i) {
        res->number[i] = count_cell(long_num->number[i], short_num->number[i], &temp, '+');
    }
    for (unsigned char i = short_num->length; i < res->length; ++i) {
        res->number[i] = count_cell(long_num->number[i], 0, &temp, '+');
    }
    if (temp) {
        unsigned char ind = 0;
        unsigned char comp = 1;
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

big_int* big_int_sub(big_int* min, big_int* sub) {
    if (min->sign == '-' and sub->sign == '+') {
        sub->sign = '-';
        return big_int_sum(min, sub);
    }
    else if (min->sign == '+' and sub->sign == '-') {
        sub->sign = '+';
        return big_int_sum(min, sub);
    }

    big_int* res = (big_int*)malloc(sizeof(big_int));
    big_int* long_num = (big_int*)malloc(sizeof(big_int));
    big_int* short_num = (big_int*)malloc(sizeof(big_int));
    res->sign = long_short_def(min, long_num, sub, short_num, 1);
    res->length = long_num->length;
    res->number = (unsigned char*)malloc(res->length);

    unsigned char temp = 0;
    unsigned char count_zero = 0;
    for (unsigned char i = 0; i < short_num->length; ++i) {
        res->number[i] = count_cell(long_num->number[i], short_num->number[i], &temp, '-');
        if (res->number[i] == 0) ++count_zero;
        else count_zero = 0;
    }
    for (unsigned char i = short_num->length; i < long_num->length; ++i) {
        res->number[i] = count_cell(long_num->number[i], 0, &temp, '-');
        if (res->number[i] == 0) ++count_zero;
        else count_zero = 0;
    }
    res->length -= count_zero;
    return res;
}

big_int* big_int_move(big_int* num) {
    big_int* res = (big_int*)malloc(sizeof(big_int));
    res->length = num->length;
    res->sign = num->sign;
    bool prev = 0;
    unsigned char comp = 1;
    unsigned char ind = 0;
    for (unsigned char k = 0; k < 8; ++k) {
        if (num->number[num->length - 1] & comp) ind = k;
        comp <<= 1;
    }
    if (ind == 7) {
        ++res->length;
    }
    res->number = (unsigned char*)malloc(res->length);
    for (unsigned char i = 0; i < num->length; ++i) {
        comp = 1;
        res->number[i] = 0;
        for (unsigned char k = 0; k < 8; ++k) {
            if (prev) {
                res->number[i] |= comp;
            }
            prev = (num->number[i] & comp);
            comp <<= 1;
        }
    }
    if (ind == 7 && prev) res->number[res->length - 1] = 1;
    return res;
}

big_int* big_int_multiply(big_int* first, big_int* sec) {
    big_int* res = (big_int*)malloc(sizeof(big_int));
    big_int* long_num_mult = (big_int*)malloc(sizeof(big_int));
    big_int* short_num_mult = (big_int*)malloc(sizeof(big_int));
    long_short_def(first, long_num_mult, sec, short_num_mult, 0);
    res->length = (long_num_mult->length << 1);
    res->number = (unsigned char*)malloc(res->length);

    if (first->sign != sec->sign) {
        res->sign = '-';
    }
    else res->sign = '+';
    long_num_mult->sign = '+';
    short_num_mult->sign = '+';

    unsigned char comp = 1;
    for (unsigned char i = 0; i < res->length; ++i) res->number[i] = 0;
    for (unsigned char i = 0; i < short_num_mult->length; ++i) {
        unsigned char comp = 1;
        for (unsigned char k = 0; k < 8; ++k) {
            if (short_num_mult->number[i] & comp) {
                res = big_int_sum(res, long_num_mult);
            }
            long_num_mult = big_int_move(long_num_mult);
            comp <<= 1;
        }
    }
    while (res->number[res->length - 1] == 0) --res->length;
    return res;
}
