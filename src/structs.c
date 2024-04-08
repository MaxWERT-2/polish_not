#include "structs.h"

#include <stdio.h>
#include <stdlib.h>

struct node* init(const double value) {
    struct node* tmp = (struct node*)malloc(sizeof(struct node));
    tmp->value = value;
    tmp->next = NULL;
    return tmp;
}

struct node* add(struct node* elem, double value) {
    struct node* tmp = init(value);
    if (elem != NULL) {
        tmp->next = elem->next;
        elem->next = tmp;
    }
    return tmp;
}

struct node* remove_n(struct node* elem, struct node* root) {
    struct node* ret_value = root;
    if (elem == root) {
        if (root != NULL) {
            ret_value = root->next;
            free(root);
        }
    } else {
        struct node* sup = root;
        while (sup->next != NULL && sup->next != elem) {
            sup = sup->next;
        }
        if (sup->next == elem) {
            sup->next = elem->next;
            free(elem);
        }
    }
    return ret_value;
}

void destroy(struct node* root) {
    if (root != NULL) {
        destroy(root->next);
    }
    free(root);
}
