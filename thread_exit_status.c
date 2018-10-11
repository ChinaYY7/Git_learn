//gcc thread_exit_status.c  -o ../bin/thread_exit_status.out -lpthread
//"c": "cd $dir && gcc -g $fileName -o ../bin/$fileNameWithoutExt.out -lpthread && cd $dir/../bin && ./$fileNameWithoutExt.out",
#include "apue.h"
#include <pthread.h>
//出错处理
void Error_Exit(const char *str)
{
    perror(str);
    exit(EXIT_FAILURE);
}
void printids(const char *s)
{
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%1lx)\n",s,(unsigned long)pid,(unsigned long)tid,(unsigned long)tid);
}
void *thr_fn1(void *arg)
{
    printids("new thread 1:");
    printf("thread 1 returning\n");
    return ((void *)1);
}
void *thr_fn2(void *arg)
{
    printids("new thread 2:");
    printf("thread 2 exiting \n");
    return ((void *)2);
}
int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    printids("main thread:");
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        Error_Exit("can't create thread 1");

    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        Error_Exit("can't create thread 2");

    err = pthread_join(tid1, &tret);
    if (err != 0)
        Error_Exit("can't join with thread 1");
    printf("thread 1 exit code %ld\n", (long)tret);

    err = pthread_join(tid2, &tret);
    if (err != 0)
        Error_Exit("can't join with thread 2");
    printf("thread 2 exit code %ld\n", (long)tret);
    exit(0);
}
