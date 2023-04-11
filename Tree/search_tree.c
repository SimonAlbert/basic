//
// Created by Simon on 2023/4/11.
// search tree implemented by LinkedList
//
#include <stdio.h>
#include <stdlib.h>

typedef int T;

typedef struct node {
    T val;
    int count;
    struct node *left;
    struct node *right;
} *node_ptr, node;

node_ptr lc(node_ptr t) {
    return t->left;
}

node_ptr rc(node_ptr t) {
    return t->right;
}
int end(node_ptr t){
    return !t||!t->count;
}
void print_node(node_ptr t) {
    printf("%d", t->val);
}

// 根左右
void pre_order_print(node_ptr t) {
    if (end(t)) return;
    print_node(t);
    pre_order_print(lc(t));
    pre_order_print(rc(t));
}

// 左根右
void in_order_print(node_ptr t) {
    if (end(t)) return;
    in_order_print(lc(t));
    print_node(t);
    in_order_print(rc(t));
}

// 左右根
void post_order_print(node_ptr t) {
    if (end(t)) return;
    post_order_print(lc(t));
    post_order_print(rc(t));
    print_node(t);
}

// 插入
void insert(T *v, node_ptr t, node_ptr parent) {
    if (end(t)) {
        t = (node_ptr) malloc(sizeof(node));
        t->val = *v;
        t->count = 1;
        t->parent = parent;
        t->left = NULL;
        t->right = NULL;
        return;
    }
    if (*v == t->val) {
        t->count++;
    } else if (*v < t->val) {
        insert(v, lc(t), t);
    } else if (*v > t->val) {
        insert(v, rc(t), t);
    }
}

node_ptr find_min(node_ptr t) {
    if (!lc(t)) return t;
    return find_min(lc(t));
}

node_ptr find_max(node_ptr t) {
    if (!rc(t)) return t;
    return find_max(rc(t));
}

node_ptr find_val(T *v, node_ptr t) {
    if (end(t)) return NULL;
    if (*v == t->val) {
        return t;
    } else if (*v < t->val) {
        return find_val(v, lc(t));
    } else {// *v > t->val
        return find_val(v, rc(t));
    }
}

void delete_val(T *v, node_ptr t) {
    if (end(t)) return;
    if (*v == t->val) {
        if (t->count > 1) {
            t->count--;
            return;
        } else {
            node_ptr l = lc(t);
            node_ptr r = rc(t);
            node_ptr c = l?l:r;
            if(l && !r){

            }
        }
    } else if (*v < t->val) {
        delete_val(v, lc(t));
    } else {
        delete_val(v, rc(t));
    }
}