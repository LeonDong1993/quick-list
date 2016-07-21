#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "qList.h"

int main()
{
    Qlist *ql=init_qlist(300000);
    //char *file="/home/dbuser/dong/dataset/uni-data/D30";
    //char *file="C:/Users/Dong/uni-data/A5";
    //run_assert_file(ql,file);
    //user_command(ql);
    long build_length=30e+6;
    double latency;
    struct timeval main_start,main_end;

    char *data_file="/home/dbuser/dong/dataset/setb-data";
    gettimeofday( &main_start, NULL );
    build_index(ql,data_file,build_length);
    gettimeofday( &main_end, NULL );
    latency = 1000000 * ( main_end.tv_sec - main_start.tv_sec ) + main_end.tv_usec - main_start.tv_usec;
    printf("\t main building time :%lf\n",latency/1000000);
    show_stat_info();

    return 0;
}
