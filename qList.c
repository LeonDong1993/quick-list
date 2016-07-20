#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <math.h>
#include "qList.h"

long valid_ins=0,valid_del=0,valid_query=0;

inline void show_stat_info()
{
    printf("Valid insert: %ld \n",valid_ins);
    printf("Valid query: %ld \n",valid_query);
    printf("Valid delete: %ld \n",valid_del);
}

inline void exec_command(Qlist *ql,char op,key_type k1,key_type k2)
{
    switch(op) {
    case 'i':
        if (1==qlist_insert(ql,k1,NULL)) {
            valid_ins++;
        }
        break;
    case 's':
        if (1==qlist_search(ql,k1)) {
            valid_query++;
        }
        break;
    case 'd':
        if (1==qlist_delete(ql,k1)) {
            valid_del++;
        }
        break;
    case 'o':
        show_stat_info();
        break;
    case 'r':
        break;
    case 'p':
        print_qlist(ql);
        break;
    default:
        fprintf(stderr,"warning: command not right %c \n",op);
        return;
    }
}

void user_command(Qlist *ql)
{
    char op;
    key_type k1=1,k2=0;
    while(1) {
        printf(">> ");
        scanf(" %c",&op);
        if(op!='o' && op !='q' && op !='p') {
            scanf("%ld",&k1);
        }
        if(op=='r') {
            scanf("%ld",&k2);
        } else if(op == 'q') {
            return;
        }
        exec_command(ql,op,k1,k2);
        //exec_command(ql,'o',0,0);
    }
}

void run_assert_file(Qlist *ql,char *file)
{
    FILE *fp=fopen(file,"r");
    if(NULL==fp) {
        printf("operation file not exist! \n");
        exit(-1);
    }
    char op;
    key_type k1=0,k2=0;
    int ret;
    while(1) {
        fscanf(fp,"%c",&op);
        if(op!='r'){
            ret=fscanf(fp," %ld\n",&k1);
        }else{
            ret=fscanf(fp," %ld %ld\n",&k1,&k2);
        }
        if(ret == EOF){
            break;
        }
        exec_command(ql,op,k1,k2);
    }
    show_stat_info();
    fclose(fp);
    return;
}

void print_qlist(Qlist *ql)
{
    len_type length=ql->used;
    len_type idx=0;
    for(idx=0;idx<length;idx++){
        printf("%ld ",ql->ikey[idx]);
    }
    printf("\n nodes \n");
    for(idx=0;idx<=length;idx++){
        printf("%p\n",ql->nodes[idx]);
        len_type count=ql->nodes[idx]->cnt,j=0;
        for(j=0;j<count;j++){
            printf("%ld ",ql->nodes[idx]->key[j]);
        }
        printf("\n");
    }
}

///=========CORE FUNCTION========///


inline void copy_entry(Node *dest,len_type dest_pos,Node *src,len_type src_pos)
{
    dest->key[dest_pos]=src->key[src_pos];
    dest->data[dest_pos]=src->data[src_pos];
}

inline Qlist *init_qlist(len_type len)
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
    if(ql->used == 0){
        return -1;
    }else if (ql->used == 1){
        if (key >= ql->ikey[0]){
            return 1;
        }else{
            return 0;
        }
    }
    len_type low=0,high=ql->used-1,mid=0;
    while(low <= high){
        mid=(low+high)>>1;
        if (ql->ikey[mid]>key){
            low = mid+1;
        }else if(ql->ikey[mid]< key ){
            high = mid-1;
        }else{
            return mid+1;
        }
    }
    /*if(ql->ikey[low] <= key && low < ql->used){
        low++;
    }*/
    return low;
}

/*
	question : full sort or select median method is faster ?
	ENTRIES = ? make them same ???
*/

static void insertion_sort(key_type *arr,int len)
{
    int i,j;
    for (i = 1; i < len; i++) {
        if (arr[i-1] > arr[i]) {
            key_type temp = arr[i];
            j = i;
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
	Node *n=(Node *)malloc(sizeof(Node));
	n->next=ori->next;
	ori->next=n;
	key_type median = get_median(ori);
	len_type cur=0,count=0,last=ENTRIES-1;
	for(cur=0;cur<=last;){
		if(ori->key[cur] >= median){
            copy_entry(n,count,ori,cur);
			count++;
            copy_entry(ori,cur,ori,last);
            last--;
		}else{
            cur++;
		}
	}
	ori->cnt=last+1;
	n->cnt=count;
	// insert this new node to qlist
	double rate= ( (double)ql->used / ql->length );
	if( (rate > 0.95 && node_pos < ( ql->used >>1 )) || (ql->used==ql->length) ){
        printf("INFO: realloc quick list space \n");
        len_type extend_len=ql->length+(len_type)(ql->length/log10(ql->length));
        key_type *ikey=(key_type *)malloc(sizeof(key_type)*extend_len);
        Node **nodes=(Node **)malloc(sizeof(Node *)*extend_len);
        //move index key
        memcpy(ikey,ql->ikey,(node_pos)*sizeof(key_type));
        ikey[node_pos]=median;
        memcpy(ikey+node_pos+1 ,ql->ikey+node_pos,(ql->used-node_pos)*sizeof(key_type));
        //move node pointer
        memcpy(nodes,ql->nodes,(node_pos+1)*sizeof(Node *));
        nodes[node_pos+1]=n;
        memcpy(nodes+node_pos+2,ql->nodes+node_pos+1,(ql->used-node_pos)*sizeof(Node *));
        ql->used++;
        ql->length=extend_len;
        free(ql->nodes);
        free(ql->ikey);
        ql->ikey =ikey;
        ql->nodes = nodes;
	}else{
        memmove(ql->ikey+node_pos+1,ql->ikey+node_pos,(ql->used-node_pos)*sizeof(key_type));
        ql->ikey[node_pos]=median;
        memmove(ql->nodes+node_pos+2,ql->ikey+node_pos+1,(ql->used-node_pos)*sizeof(key_type));
        ql->nodes[node_pos+1]=n;
        ql->used++;

	}
	return 0;
}


int qlist_insert(Qlist *ql,key_type key,void *data_ptr)
{
    len_type pos=bin_search_node(ql,key);
    len_type idx=0;
    if(pos >= 0){
        Node *n=ql->nodes[pos];
        assert(n->cnt < ENTRIES);
        for (idx=0;idx<n->cnt;idx++){
            if(n->key[idx]==key){
                return 0;
            }
        }
        idx = n->cnt++;
        n->key[idx]=key;
        n->data[idx]=data_ptr;
        if((idx+1) == ENTRIES){
        	qlist_split(ql,pos);
        }
    }else{
        // first insert initialize
        ql->used++;
        ql->ikey[0]=key;
        ql->nodes[0]=(Node *)malloc(sizeof(Node));
        ql->nodes[1]=(Node *)malloc(sizeof(Node));
        ql->nodes[0]->cnt=0;
        ql->nodes[0]->next=ql->nodes[1];
        ql->nodes[1]->cnt=1;
        ql->nodes[1]->next=NULL;
        ql->nodes[1]->key[0]=key;
        ql->nodes[1]->data[0]=data_ptr;
    }
    return 1;
}

int qlist_delete(Qlist *ql,key_type key)
{
    len_type pos=bin_search_node(ql,key);
    Node *n=ql->nodes[pos];
    len_type idx=0;
    for (idx=0;idx<n->cnt;idx++){
        if(n->key[idx]==key){
            copy_entry(n,idx,n,--n->cnt);
            return 1;
        }
    }
    return 0;
}

int qlist_search(Qlist *ql,key_type key)
{
    len_type pos=bin_search_node(ql,key);
    Node *n=ql->nodes[pos];
    len_type idx=0;
    for (idx=0;idx<n->cnt;idx++){
        if(n->key[idx]==key){
            return 1;
        }
    }
    return 0;
}
