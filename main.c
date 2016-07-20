#include <stdio.h>
#include <stdlib.h>
#include "qList.h"
int main()
{
    Qlist *ql=init_qlist(3);
    char *file="C:/Users/Dong/uni-data/A5";
    //run_assert_file(ql,file);
    user_command(ql);
    return 0;
}
