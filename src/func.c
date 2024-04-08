#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "structs.h"
#define EPS 1e-10

double func(char *exp, int *mode, double value);

int is_op(int value) {
    int ret_value = 0;
    if ((value >= 2 && value <= 6) || value == 14) {
        ret_value = 1;
    }
    return ret_value;
}

double lex_value(lexems lex, int *mode, double value) {
    double ret_value;
    if (lex.type == 0) {
        sscanf(lex.str, "%lf", &ret_value);
    } else if (lex.type == 1) {
        ret_value = value;
    } else if (lex.type == 7) {
        ret_value = sin(func(lex.str, mode, value));
    } else if (lex.type == 8) {
        ret_value = cos(func(lex.str, mode, value));
    } else if (lex.type == 9) {
        ret_value = func(lex.str, mode, value);
        if (fabs(cos(ret_value)) < EPS) {
            *mode = 0;
        } else {
            ret_value = tan(ret_value);
        }
    } else if (lex.type == 10) {
        ret_value = func(lex.str, mode, value);
        if (fabs(sin(ret_value)) < EPS) {
            *mode = 0;
        } else {
            ret_value = cos(ret_value) / sin(ret_value);
        }
    } else if (lex.type == 11) {
        ret_value = func(lex.str, mode, value);
        if (ret_value < -EPS) {
            *mode = 0;
        } else {
            ret_value = sqrt(ret_value);
        }
    } else if (lex.type == 12) {
        ret_value = func(lex.str, mode, value);
        if (ret_value < EPS) {
            *mode = 0;
        } else {
            ret_value = log(ret_value);
        }
    } else if (lex.type == 13) {
        ret_value = func(lex.str, mode, value);
    }
    return ret_value;
}

int priority(int type) {
    int ret_value = 0;
    if (type == 2) {
        ret_value = 1;
    }
    if (type == 3) {
        ret_value = 1;
    }
    if (type == 4) {
        ret_value = 2;
    }
    if (type == 5) {
        ret_value = 2;
    }
    if (type == 6) {
        ret_value = 3;
    }
    return ret_value;
}

double polnot_to_d(list *root, int *mode) {
    double ret_value = 0.0;
    list *sup = root;
    if (sup != NULL) {
        while (sup->next != NULL) {
            sup = sup->next;
        }
        double val = sup->value;
        int op_c = sup->op;
        root = remove_n(sup, root);
        if (is_op(op_c)) {
            if (op_c == 2) {
                ret_value = polnot_to_d(root, mode) + polnot_to_d(root, mode);
            } else if (op_c == 3) {
                ret_value = -polnot_to_d(root, mode) + polnot_to_d(root, mode);
            } else if (op_c == 4) {
                ret_value = polnot_to_d(root, mode) * polnot_to_d(root, mode);
            } else if (op_c == 5) {
                ret_value = polnot_to_d(root, mode);
                if (fabs(ret_value) < EPS) {
                    *mode = 0;
                    ret_value = polnot_to_d(root, mode);
                } else {
                    ret_value = polnot_to_d(root, mode) / ret_value;
                }
            } else if (op_c == 6) {
                double poc = polnot_to_d(root, mode);
                ret_value = polnot_to_d(root, mode);
                if (fabs(ret_value) < EPS) {
                    if (poc < EPS) {
                        *mode = 0;
                    } else {
                        ret_value = 0.0;
                    }
                } else if (ret_value < -EPS) {
                    if (poc == (int)poc) {
                        ret_value = pow(ret_value, poc);
                    } else {
                        *mode = 0;
                    }
                } else {
                    ret_value = pow(ret_value, poc);
                }
            } else if (op_c == 14) {
                ret_value = -polnot_to_d(root, mode);
            }
        } else {
            ret_value = val;
        }
    }
    return ret_value;
}

double func(char *exp, int *mode, double value) {
    double ans = 0.0;
    if (*mode == 1) {
        int count = 0;

        lexems *str_arr = str_sep(exp, mode, &count);
        if (*mode == 1) {
            list *line = NULL;
            list *line_root = NULL;
            int pr_arr[1000];
            int op_c = 0;
            int pr = 0;
            int switch_flag = 0;
            for (int i = 0; i < count; ++i) {
                if (is_op(str_arr[i].type)) {
                    int cur_pr = priority(str_arr[i].type);
                    if (line_root != NULL) {
                        if (switch_flag == 1) {
                            if (cur_pr <= pr) {
                                for (int j = op_c - 1; j >= 0; --j) {
                                    line = add(line, 0);
                                    line->op = pr_arr[j];
                                }
                                op_c = 0;
                            }
                            pr_arr[op_c] = str_arr[i].type;
                            op_c++;
                            pr = cur_pr;
                            switch_flag = 0;
                        } else {
                            *mode = -1;
                        }
                    } else {
                        if (str_arr[i].type == 3) {
                            pr_arr[op_c] = 14;
                            op_c++;
                            pr = cur_pr;
                            switch_flag = 0;
                        } else {
                            *mode = -1;
                        }
                    }
                } else {
                    if (switch_flag == 0) {
                        double value_c = lex_value(str_arr[i], mode, value);
                        if (line != NULL) {
                            line = add(line, value_c);
                        } else {
                            line = init(value_c);
                            line_root = line;
                        }
                        line->op = 0;
                        switch_flag = 1;
                    } else {
                        *mode = -1;
                    }
                }
            }
            for (int j = op_c - 1; j >= 0; --j) {
                line = add(line, 0);
                line->op = pr_arr[j];
            }
            ans = polnot_to_d(line_root, mode);
        }
        for (int i = 0; i < count; ++i) {
            if (str_arr[i].str != NULL) {
                free(str_arr[i].str);
            }
        }
        free(str_arr);
    }
    return ans;
}
