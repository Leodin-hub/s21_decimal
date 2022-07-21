#include "s21_decimal.h"

const s21_decimal onesz = {{1, 0, 0, 0}};
const long_decimal oneslz = {{1, 0, 0, 0, 0, 0, 0}};

// Перевод строки в биты децимала
void char_from_decim(char *c, s21_decimal *a) {
    for (int i = 0; i < 128 && *c; i++, c++)
        setbit(&a->bits[i / 32], i % 32, *c - 48);
}

// Печать децимала в нормальном виде
void decim_from_char(s21_decimal a, int flag) {
    char c[255];
    for (int i = 0; i < 255; c[i] = 0, i++) {}
    int exp = 0, neg = 0, ch = 0;
    if (flag) {
        exp = i_exp(a);
        neg = checkbit(a.bits[3], 31);
        a.bits[3] = 0;
    }
    s21_decimal on = onesz, t = a;
    div_ten(&t, 127, 1);
    while (greater(t, on, 127) || s21_is_equal(t, on)) {
        if (greater(a, on, 127) || s21_is_equal(t, on)) {
            mul_ten(&on, 4, 1);
            ch++;
        }
    }
    for (; ch >= 0; ch--) {
        int r = 0;
        !ch ? craft_decim(onesz, &on) : 0;
        while ((greater(a, on, 127) || s21_is_equal(a, on)) && !zeros_eq(a, 3)) {
            sub_r(a, on, &a, 127);
            r++;
        }
        r >= 10 ? r = 1 : 0;
        c[ch] = r + 48;
        div_ten(&on, 127, 1);
    }
    if (exp) {
        int check = 0;
        for (int i = 254; i > 0 && !check; check = i * !!c[i], i--) {}
        if (check < exp) {
            check++;
            for (; check <= exp; check++)
                c[check] = '0';
            c[check - 1] = '.';
            c[check] = '0';
        } else {
            for (int i = check; i >= exp; i--) {
                c[i + 1] = c[i];
            }
            c[exp] = '.';
        }
    }
    int check = 0;
    for (int i = 254; i > 0 && !check; check = i * !!c[i], i--) {}
    char c2[255];
    for (int i = 0; i < 255; c2[i] = 0, i++) {}
    ch = check;
    for (int i = 0; i <= ch; c2[i] = c[check], i++, check--) {}
    neg ? printf("-") : 0;
    printf("%s\n", c2);
}

// Печать long decimal в нормальном виде
void longdecim_from_char(long_decimal a) {
    char c[255];
    for (int i = 0; i < 255; c[i] = 0, i++) {}
    int ch = 0;
    long_decimal on = oneslz, t = a;
    div_ten_long(&t, 1);
    while (greater_long(t, on) || is_equal_long(t, on)) {
        if (greater_long(a, on) || is_equal_long(t, on)) {
            mul_ten_long(&on, 1);
            ch++;
        }
    }
    for (; ch >= 0; ch--) {
        int r = 0;
        !ch ? craft_long(oneslz, &on) : 0;
        while ((greater_long(a, on) || is_equal_long(a, on)) && !zeros_eq_long(a)) {
            sub_long(a, on, &a);
            r++;
        }
        r >= 10 ? r = 1 : 0;
        c[ch] = r + 48;
        div_ten_long(&on, 1);
    }
    int check = 0;
    for (int i = 254; i > 0 && !check; check = i * !!c[i], i--) {}
    char c2[255];
    for (int i = 0; i < 255; c2[i] = 0, i++) {}
    ch = check;
    for (int i = 0; i <= ch; c2[i] = c[check], i++, check--) {}
    printf("%s\n", c2);
}

// Печать в консоль всего числа побитово
void print_bits(const unsigned int value, const int check) {
    for (int i = 0; i < 32; i++)
        printf("%d", value & (1 << i) ? 1 : 0);
    check ? printf("\n") : 0;
}

// Печать всего числа децимал побитово
void print_full(const s21_decimal value) {
    for (int i = 0; i < 4; i++) {
        print_bits(value.bits[i], 0);
        printf(" ");
    }
    printf("\n");
    char *c = "matisa", *c2 = "expons";
    for (int i = 0; i <= 131; i++) {
        !i || i == 97 || i == 116 || i == 123 ? printf("|") : 0;
        (i > 0 && i < 46) || (i > 51 && i < 97) || (i > 99 && i < 116) || \
        (i > 123 && i < 132) ? printf("-") : 0;
        i > 45 && i < 52 ? printf("%c", *c), c++ : 0;
        i > 116 && i < 123 ? printf("%c", *c2), c2++ : 0;
        i == 99 ? printf(" ") : 0;
        i == 131 ? printf("\n") : 0;
    }
}


// Печать long decimal
void print_long(const long_decimal value) {
    for (int i = 0; i < MAX_LONG_S; print_bits(value.bits[i], 0), printf(" "), i++) {}
    printf("\n");
}

// Изменить бит на позиции на противоположный
int switchbit(const int value, const int posicion) {
    return value ^ (1 << posicion);
}
