#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "big_int.h"
using namespace std;

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

big_int* big_int_zero() {
    big_int* res = (big_int*)malloc(sizeof(big_int));
    res->length = 1;
    res->number = (unsigned char*)malloc(res->length);
    res->number[0] = 0;
    res->sign = '+';
    return res;
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

int big_int_abs_compare(const big_int* first, const big_int* sec) {
    if (first->length > sec->length)
        return 1;
    else if (first->length < sec->length)
        return -1;
    else if (first->length == sec->length) {
        for (int i = first->length - 1; i >= 0; --i) {
            if (first->number[i] > sec->number[i])
                return 1;
            else if (first->number[i] < sec->number[i])
                return -1;
        }
    }
    return 0;
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
    big_int* long_num = big_int_zero();
    big_int* short_num = big_int_zero();
    if (big_int_abs_compare(first, sec) >= 0) {
        big_int_assign(long_num, first);
        big_int_assign(short_num, sec);
    }
    else {
        big_int_assign(long_num, sec);
        big_int_assign(short_num, first);
    }

    big_int* res = (big_int*)malloc(sizeof(big_int));
    res->sign = long_num->sign;
    res->length = long_num->length;
    res->number = (unsigned char*)malloc(res->length);

    unsigned char carry = 0;
    unsigned int temp = 0;
    for (unsigned char i = 0; i < short_num->length; ++i) {
        temp = long_num->number[i] + short_num->number[i] + carry;
        res->number[i] = temp;
        carry = (temp >> 8);
    }
    for (unsigned char i = short_num->length; i < res->length; ++i) {
        temp = long_num->number[i] + carry;
        res->number[i] = temp;
        carry = (temp >> 8);
    }
    if (carry) {
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

    big_int* long_num = big_int_zero();
    big_int* short_num = big_int_zero();
    if (big_int_abs_compare(min, sub) >= 0) {
        big_int_assign(long_num, min);
        big_int_assign(short_num, sub);
    }
    else {
        big_int_assign(long_num, sub);
        big_int_assign(short_num, min);
    }

    big_int* res = (big_int*)malloc(sizeof(big_int));
    res->sign = long_num->sign;
    res->length = long_num->length;
    res->number = (unsigned char*)malloc(res->length);

    int temp = 0;
    unsigned char carry = 0;
    unsigned char count_zero = 0;
    for (unsigned char i = 0; i < short_num->length; ++i) {
        temp = long_num->number[i] - short_num->number[i] - carry;
        res->number[i] = temp;
        carry = (temp < 0);
        if (res->number[i] == 0) ++count_zero;
        else count_zero = 0;
    }
    for (unsigned char i = short_num->length; i < long_num->length; ++i) {
        temp = long_num->number[i] - carry;
        res->number[i] = temp;
        carry = (temp < 0);
        if (res->number[i] == 0) ++count_zero;
        else count_zero = 0;
    }
    res->length -= count_zero;
    unsigned char* res_num = (unsigned char*)malloc(res->length);
    memcpy(res_num, res->number, res->length);
    free(res->number);
    res->number = res_num;
    return res;
}

big_int* big_int_move_bit(big_int* num, char dest) {
    big_int* res = (big_int*)malloc(sizeof(big_int));
    res->length = num->length;
    res->sign = num->sign;
    bool prev = 0;
    unsigned char comp = 1;
    if (dest == 'l') {
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
            res->number[i] = 0;
            for (comp = 1; comp; comp <<= 1 ) {
                if (prev) res->number[i] |= comp;
                prev = (num->number[i] & comp);
            }
        }
        if (ind == 7 && prev) res->number[res->length - 1] = 1;
        return res;
    }
    else {
        unsigned char ind = 0;
        for (unsigned char k = 0; k < 8; ++k) {
            if (num->number[num->length - 1] & comp) ind = k;
            comp <<= 1;
        }
        if (ind == 0) --res->length;
        res->number = (unsigned char*)malloc(res->length);

        unsigned char prev = 0;
        for (unsigned char i = 0; i < res->length; ++i) {
            res->number[i] = 0;
            for (unsigned char comp = 1; comp; comp <<= 1) {
                if (comp == 1 && res->number[i] & comp) res->number[i - 1] |= 128;
                else if (prev == 1) res->number[i] |= comp;
                prev = (num->number[i] & comp);
            }
        }
        return res;
    }
}

big_int* big_int_move_byte(big_int* num, char dest) {
    big_int* res = (big_int*)malloc(sizeof(big_int));
    res->sign = num->sign;
    unsigned char prev = 0;
    if (dest == 'r') {
        res->length = num->length - 1;
        res->number = (unsigned char*)malloc(res->length);
        if (res->length == 0) {
            res->length = 1;
            res->number[0] = 0;
            return res;
        }
        unsigned char ind = 0;
        unsigned char comp = 1;
        for (unsigned char k = 0; k < 8; ++k) {
            if (num->number[num->length - 1] & comp) ind = k;
            comp <<= 1;
        }
        res->number[res->length - 1] = 0;
        comp = 1;
        for (unsigned char k = 0; k <= ind; ++k) {
            if (num->number[res->length - 1] & comp) res->number[res->length - 1] |= comp;
            comp <<= 1;
        }
        for (unsigned char i = 0; i < res->length - 1; ++i) res->number[i] = num->number[i];
        return res;
    }
    else {
        res->length = num->length + 1;
        res->number = (unsigned char*)malloc(res->length);
        res->number[0] = 0;
        for (unsigned char i = 0; i < num->length; ++i) res->number[i + 1] = num->number[i];
        return res;
    }
}

big_int* big_int_shift(big_int* num, unsigned char dist, char dest) {
    big_int* res = (big_int*)malloc(sizeof(big_int));
    res->sign = num->sign;
    res->length = 1;
    res->number = (unsigned char*)malloc(res->length);
    while (dist >= 8) {
        big_int_assign(num, big_int_move_byte(num, dest));
        dist -= 8;
    }
    for (unsigned char k = 0; k < dist; ++k) {
        big_int_assign(num, big_int_move_bit(num, dest));
    }
    big_int_assign(res, num);
    return res;
}

big_int* big_int_multiply(big_int* first, big_int* sec) {
    big_int* res = (big_int*)malloc(sizeof(big_int));
    big_int* long_num_mult = big_int_zero();
    big_int* short_num_mult = big_int_zero();
    if (big_int_abs_compare(first, sec) >= 0) {
        big_int_assign(long_num_mult, first);
        big_int_assign(short_num_mult, sec);
    }
    else {
        big_int_assign(long_num_mult, sec);
        big_int_assign(short_num_mult, first);
    }
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
            long_num_mult = big_int_move_bit(long_num_mult, 'l');
            comp <<= 1;
        }
    }
    while (res->number[res->length - 1] == 0) --res->length;
    return res;
}

//big_int* big_int_divide(big_int* divdnd, big_int* divdr) {
//    big_int* res = (big_int*)malloc(sizeof(big_int));
//    big_int* rem = (big_int*)malloc(sizeof(big_int));
//    big_int* long_num_mult = (big_int*)malloc(sizeof(big_int));
//    big_int* short_num_mult = (big_int*)malloc(sizeof(big_int));
//
//    long_short_def(divdnd, long_num_mult, divdr, short_num_mult, 0);
//    if (long_num_mult == divdr) {
//        res->length = 1;
//        res->number[0] = 0;
//        big_int_assign(rem, divdnd);
//    }
//
//    unsigned char ind = 0;
//    unsigned char comp = 1;
//    for (unsigned char k = 0; k < 8; ++k) {
//        if (divdr->number[divdr->length - 1] & comp) ind = k;
//        comp <<= 1;
//    }
//    unsigned char divdr_len = ((divdr->length - 1) << 3) + ind + 1;
//
//    comp = 1;
//    for (unsigned char k = 0; k < 8; ++k) {
//        if (divdnd->number[divdnd->length - 1] & comp) ind = k;
//        comp <<= 1;
//    }
//    unsigned char divdnd_len = ((divdnd->length - 1) << 3) + ind + 1;
//
//    big_int* temp_divdnd = (big_int*)malloc(sizeof(big_int*));
//    res->number[res->length - 1] = 0;
//    unsigned char count = 1;
//    big_int_assign(temp_divdnd, big_int_shift(divdnd, divdnd_len - divdr_len * count, 'r'));
//    for (unsigned char i = 0; i < (divdnd_len / divdr_len); ++i) {
//        
//    }
//    return res;
//}
