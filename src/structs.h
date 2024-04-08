#ifndef SRC_STRUCTS_H_
#define SRC_STRUCTS_H_

typedef struct node {
    double value;
    int op;
    struct node *next;
} list;

typedef struct lex {
    int type;
    /*
    0 - number
    1 - x
    2 - +
    3 - -
    4 - *
    5 - /
    6 - ^
    7 - sin()
    8 - cos()
    9 - tan()
    10 - ctg()
    11 - sqrt()
    12 - ln()
    13 - ()
    */
    char *str;
} lexems;

struct node* init(const double value);
struct node* add(struct node* elem, double value);
struct node* remove_n(struct node* elem, struct node* root);
void destroy(struct node* root);

#endif  // SRC_STRUCTS_H_
