#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "qList.h"


Qlist *init_qlist(len_type len)
{
    assert(len>0);
    Qlist *ql=(Qlist *)malloc(sizeof(Qlist));
    ql->used=0;
    ql->length=len;
    ql->ikey=(key_type *)malloc(sizeof(key_type)*len);
    ql->nodes=(Node **)malloc(sizeof(Node *)*len);
    return ql;
}





int qlist_insert(Qlist *ql,key_type key,void *data_ptr){
    len_t pos=bin_search_node(ql,key);
    if(pos>=0){
        Node *n=ql->nodes[0];

    }else{

    }

}
