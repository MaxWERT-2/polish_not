#include "parse.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void set_str(char *exp);
void rm_space(char *exp);
void replace_func(char *exp);
lexems *str_sep(char *exp, int *mode_err, int *count);
lexems *arr_lex_push_back(lexems *arr_lex, int *count, int a, char *str);
char *num_from_str(char *str, int *ctr);
char *str_in_brackets(char *str, int *mode, int *ctr);

void set_str(char *exp) {
    rm_space(exp);
    replace_func(exp);
}

void rm_space(char *exp) {
    char *tmp = malloc(strlen(exp) * sizeof(char));
    int count = 0;
    for (int i = 0; i < (int)strlen(exp); i++) {
        if (exp[i] != ' ') {
            tmp[count] = exp[i];
            count++;
        }
    }
    exp = realloc(exp, count * sizeof(char));
    for (int i = 0; i < (int)strlen(exp); i++) exp[i] = tmp[i];
    free(tmp);
}

void replace_func(char *exp) {
    char *tmp = malloc((strlen(exp) + 1) * sizeof(char));
    int count = 0;
    for (int i = 0; i < (int)strlen(exp); i++) {
        int flag_count = 0;
        if (exp[i] == 's' && exp[i + 1] == 'i' && exp[i + 2] == 'n') {
            tmp[count] = 's';
            i = i + 2;
            flag_count = 1;
        }
        if (exp[i] == 'c' && exp[i + 1] == 'o' && exp[i + 2] == 's') {
            tmp[count] = 'c';
            i = i + 2;
            flag_count = 1;
        }
        if (exp[i] == 't' && exp[i + 1] == 'a' && exp[i + 2] == 'n') {
            tmp[count] = 't';
            i = i + 2;
            flag_count = 1;
        }
        if (exp[i] == 'c' && exp[i + 1] == 't' && exp[i + 2] == 'g') {
            tmp[count] = 'g';
            i = i + 2;
            flag_count = 1;
        }
        if (exp[i] == 's' && exp[i + 1] == 'q' && exp[i + 2] == 'r' && exp[i + 3] == 't') {
            tmp[count] = 'q';
            i = i + 3;
            flag_count = 1;
        }
        if (exp[i] == 'l' && exp[i + 1] == 'n') {
            tmp[count] = 'l';
            i = i + 1;
            flag_count = 1;
        }
        if (flag_count == 0) {
            tmp[count] = exp[i];
            count++;
        } else {
            count++;
        }
    }
    for (int i = 0; i < count; i++) exp[i] = tmp[i];
    exp[count] = '\0';
    free(tmp);
}

lexems *str_sep(char *exp, int *mode_err, int *count) {
    set_str(exp);
    lexems *arr_lex = malloc(sizeof(lexems));
    for (int i = 0; i < (int)strlen(exp); i++) {
        switch (exp[i]) {
            case 'x':  // x
                arr_lex = arr_lex_push_back(arr_lex, count, 1, NULL);
                break;
            case '+':  // +
                arr_lex = arr_lex_push_back(arr_lex, count, 2, NULL);
                break;
            case '-':  // -
                arr_lex = arr_lex_push_back(arr_lex, count, 3, NULL);
                break;
            case '*':  // *
                arr_lex = arr_lex_push_back(arr_lex, count, 4, NULL);
                break;
            case '/':  // /
                arr_lex = arr_lex_push_back(arr_lex, count, 5, NULL);
                break;
            case '^':  // ^
                arr_lex = arr_lex_push_back(arr_lex, count, 6, NULL);
                break;
            case '(':  // ( && )
            {
                char *inb = str_in_brackets(exp, mode_err, &i);
                arr_lex = arr_lex_push_back(arr_lex, count, 13, inb);
            } break;
            case 'l':  // ln(x)
            {
                i++;
                char *inb = str_in_brackets(exp, mode_err, &i);
                arr_lex = arr_lex_push_back(arr_lex, count, 12, inb);
            } break;
            case 't':  // tan(x)
            {
                i++;
                char *inb = str_in_brackets(exp, mode_err, &i);
                arr_lex = arr_lex_push_back(arr_lex, count, 9, inb);
            } break;
            case 's':  // sin(x)
            {
                i++;
                char *inb = str_in_brackets(exp, mode_err, &i);
                arr_lex = arr_lex_push_back(arr_lex, count, 7, inb);
            } break;
            case 'q':  // sqrt(x)
            {
                i++;
                char *inb = str_in_brackets(exp, mode_err, &i);
                arr_lex = arr_lex_push_back(arr_lex, count, 11, inb);
            } break;
            case 'c':  // cos(x)
            {
                i++;
                char *inb = str_in_brackets(exp, mode_err, &i);
                arr_lex = arr_lex_push_back(arr_lex, count, 8, inb);
            } break;
            case 'g':  // ctg(x)
            {
                i++;
                char *inb = str_in_brackets(exp, mode_err, &i);
                arr_lex = arr_lex_push_back(arr_lex, count, 10, inb);
            } break;
            default:  // num || error
                if (exp[i] >= '0' && exp[i] <= '9') {
                    char *num = num_from_str(exp, &i);
                    arr_lex = arr_lex_push_back(arr_lex, count, 0, num);
                } else {
                    *mode_err = -1;
                }
                break;
        }
    }
    return arr_lex;
}

lexems *arr_lex_push_back(lexems *arr_lex, int *count, int a, char *str) {
    lexems *tmp = malloc((*count + 1) * sizeof(lexems));
    for (int i = 0; i < *count; i++) {
        tmp[i].type = arr_lex[i].type;
        tmp[i].str = arr_lex[i].str;
    }
    tmp[*count].type = a;
    tmp[*count].str = str;
    (*count)++;
    free(arr_lex);
    return tmp;
}

char *num_from_str(char *str, int *ctr) {
    int len = (int)strlen(str);
    char *num = (char *)malloc((len + 1) * sizeof(char));
    if (num != NULL) {
        int flag = 1;
        int p_count = 0;
        int num_counter = 0;
        int i = (int)*ctr;
        for (; i < len + 1 && flag; ++i) {
            if (str[i] >= '0' && str[i] <= '9') {
                num[num_counter] = str[i];
                num_counter++;
            } else if (str[i] == '.' && p_count == 0) {
                p_count++;
                num[num_counter] = str[i];
                num_counter++;
            } else {
                flag = 0;
            }
        }
        *ctr = i - 2;
        num[num_counter] = '\0';
    }
    return num;
}

char *str_in_brackets(char *str, int *mode, int *ctr) {
    int len = (int)strlen(str);
    char *inb = (char *)malloc((len + 1) * sizeof(char));

    if (inb != NULL) {
        int bbalance = 1;
        int inb_counter = 0;
        int i = (int)*ctr + 1;
        for (; i < len && bbalance > 0; ++i) {
            if (str[i] == ')' && bbalance == 1) {
                bbalance = 0;
            } else {
                inb[inb_counter] = str[i];
                inb_counter++;
                if (str[i] == '(') {
                    bbalance++;
                }
                if (str[i] == ')') {
                    bbalance--;
                }
            }
        }
        if (bbalance > 0) {
            *mode = -1;
        }
        inb[inb_counter] = '\0';
        *ctr = i;
    }
    return inb;
}
