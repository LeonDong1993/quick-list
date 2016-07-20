#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "qList.h"


static inline Qlist *init_qlist(len_type len)
{
    assert(len>0);
    Qlist *ql=(Qlist *)malloc(sizeof(Qlist));
    ql->used=0;
    ql->length=len;
    ql->ikey=(key_type *)malloc(sizeof(key_type)*len);
    ql->nodes=(Node **)malloc(sizeof(Node *)*len);
    return ql;
}

static len_type bin_search_node(Qlist *ql,key_type key)
{
	/*return 0:ql->length and the first key pos st all node elem < key*/
	return 0;
}

/*
	question : full sort or select median method is faster ?  ENTRIES = ? make them same ???
*/


static void insertion_sort(key_type *arr,int len)
{
    int i;
    for (i = 1; i < len; i++) {
        if (arr[i-1] > arr[i]) {
            key_type temp = arr[i];
            int j = i;
            while (j > 0 && arr[j - 1] > temp) {
                arr[j] = arr[j - 1];
                j--;
            }
            arr[j] = temp;
        }
    }
}

key_type get_median(Node *n)
{
    const len_type arr_size=17;
    key_type arr[arr_size];
    len_type i,j=0,step=ENTRIES/arr_size;
    for(i=0; i<arr_size; i++) {
        arr[i]=n->key[j];
        j+=step;
    }
    insertion_sort(arr,arr_size);
    return arr[arr_size/2];
}

static int qlist_split(Qlist *ql,len_type node_pos)
{
	Node *ori=ql->nodes[node_pos];
	Node *new=(Node *)malloc(sizeof(Node));
	new->next=ori->next;
	ori->next=new;
	key_type median = get_median(ori);
	len_type idx=0,count=0,;
	for(idx=0;idx<ENTRIES;i++){
		if(ori->key[idx] >= median){
			new->key[]
		}
	}



}


static int qlist_insert(Qlist *ql,key_type key,void *data_ptr)
{
    len_type pos=bin_search_node(ql,key);
    len_type idx=0
    if(pos >= 0){
        Node *n=ql->nodes[pos];
        assert(n->cnt < ENTRIES);
        idx = ++n->cnt;
        n->key[idx]=key;
        n->data[idx]=data_ptr;
        if(idx == ENTRIES){
        	qlist_split(ql,pos);
        }
    }else{

    }

}
