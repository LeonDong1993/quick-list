/**
    Author :    leondong1993@qq.com
    Date:       2016-07-18
    version:    0.1
*/
/*
    ideas : 
    multi-braids qlist perfromance test --> multi-braids bpTree
    borrow elements
*/

#ifndef _QLIST_H
#define _QLIST_H
#define ENTRIES 128

typedef long key_type;
typedef unsigned long len_type;
typedef unsigned long size_type;

typedef struct node_struct{
    struct node_struct *next;
    size_type cnt;
    key_type key[ENTRIES];
    void *data[ENTRIES];
}Node;

typedef struct qlist_struct{
    len_type used;
    len_type length;
    key_type *ikey;
    Node **nodes;
}Qlist;

#endif



