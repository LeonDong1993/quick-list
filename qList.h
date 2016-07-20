/*
    ideas :
    multi-braids qlist perfromance test --> multi-braids bpTree
    borrow elements
    add merge technique
*/

#ifndef _QLIST_H
#define _QLIST_H
#define ENTRIES 4

typedef long key_type;
typedef long len_type;
typedef long size_type;

typedef struct node_struct
{
    struct node_struct *next;
    size_type cnt;
    key_type key[ENTRIES];
    void *data[ENTRIES];
}Node;

typedef struct qlist_struct
{
    len_type used;
    len_type length;
    key_type *ikey;
    Node **nodes;
}Qlist;


///-------------------------------------///
inline Qlist *init_qlist(len_type len);
int qlist_insert(Qlist *ql,key_type key,void *data_ptr);
int qlist_delete(Qlist *ql,key_type key);
int qlist_search(Qlist *ql,key_type key);
inline void exec_command(Qlist *ql,char op,key_type k1,key_type k2);
void run_assert_file(Qlist *ql,char *file);
void user_command(Qlist *ql);
void print_qlist(Qlist *ql);
///-------------------------------------///
long valid_ins,valid_del,valid_query;

#endif



