//递归降序遍历目录层次结构，并按文件类型计数
#include"apue.h"
#include<dirent.h>
#include<limits.h>
#include<fcntl.h>
typedef int Myfunc(const char *, const struct stat*, int);//声明了一个新的函数类型Myfunc，这种函数类型带3个参数，返回值是int类型的
static Myfunc myfunc;//声明了函数myfunc的原形，用于对文件类型计数和读文件出错处理
static int myftw(char *, Myfunc *);//为输入路径字符串分配内存（遍历过程中路径大小是变化的），调用dopath函数，
static int dopath( Myfunc *);//降序遍历目录
static long nreg, ndir, nchr, nblk,nfifo, nslink, nsock, ntot;//各种类型文件计数


int main(int argc, char *argv[])
{
    int ret;   //保存遍历函数的返回值，结果总为0
    if (argc != 2)
        perror("usage: ftw4 <starting-pathname>");
    ret = myftw(argv[1], myfunc);//根据输入的路径为遍历起始点
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;//所有文件数量的和，计算百分比
    if( 0 == ntot ) //因为计算百分比，要做除数，除数不能为0
        ntot = 1;
    //输出各种文件的数量及所占的比例
    printf("     files sum = %7ld\n\n",ntot);
    printf("  reglar files = %7ld, %5.2lf %% \n",nreg, nreg*100.0/ntot);
    printf("   directories = %7ld, %5.2lf %% \n",ndir, ndir*100.0/ntot);
    printf(" block special = %7ld, %5.2lf %% \n",nblk, nblk*100.0/ntot);
    printf("  char special = %7ld, %5.2lf %% \n",nchr, nchr*100.0/ntot);
    printf("         FIFOs = %7ld, %5.2lf %% \n",nfifo, nfifo*100.0/ntot);
    printf("symbolic links = %7ld, %5.2lf %% \n",nslink, nslink*100.0/ntot);
    printf("       sockets = %7ld, %5.2lf %% \n",nsock, nsock*100.0/ntot);

    exit(ret);
}


#define FTW_F 1     //表示不是目录的文件
#define FTW_D 2     //目录文件
#define FTW_DNR 3   //目录不能打开，没权限
#define FTW_NS 4    //不能读取文件信息
static char *fullpath;
#ifdef PATH_MAX
    int PATH_LEN = PATH_MAX;
#else
    int PATH_LEN = 1024;
#endif
static int myftw(char *pathname, Myfunc *func)
{
    //《APUE》书中，这个功能用了一个很复杂的函数path_alloc()来实现
    //这里直接为它分配了一段内存
    if(PATH_LEN < strlen(pathname))  //如果路径长度大小比设定的最大路径长度还长，就多分配些内存
        PATH_LEN *=2;
    fullpath = malloc(PATH_LEN);                       
    strcpy(fullpath, pathname);  //路径名保存到分配的内存中
    fullpath[PATH_LEN-1] = '\0'; //字符串结尾
    return(dopath(func));        
}


static int dopath(Myfunc* func)
{
    struct stat statbuf;  //保存文件信息
    struct dirent *dirp;  //目录内容
    DIR *dp;              //目录有关信息
    int ret,n;            //返回值，当前路径长度

    if (lstat(fullpath, &statbuf) < 0)                  //读取文件信息
        return(func(fullpath, &statbuf, FTW_NS));       //文件信息读取失败

    if (S_ISDIR(statbuf.st_mode) == 0)                  //是不是目录        
        return(func(fullpath, &statbuf, FTW_F));        //不是目录

    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)   //对目录文件计数
        return(ret);                                    //func程序出错
    n = strlen(fullpath);                               //路径长度
    if (n + _PC_NAME_MAX + 2 > PATH_LEN)                //路径长度动态变化，预估之后的路径长度有没有超过之前分配的内存大小的可能
    {
        PATH_LEN *=2;
        if((fullpath = realloc(fullpath ,PATH_LEN)) == NULL)
            perror("realloc failed\n");
    }
    fullpath[n++] = '/';                                //路径名后添加符号'/',划分路径分量的
    fullpath[n] = 0;                                    //路径以0为结尾
    if((dp = opendir(fullpath)) == NULL)                //打开目录
        return(func(fullpath, &statbuf, FTW_DNR));      //打开目录失败
    while ((dirp = readdir(dp)) != NULL)                //读取目录内容
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)//跳过.和..  不需要遍历
            continue;
        strcpy(&fullpath[n], dirp->d_name);             //将读到的目录内容，即该目录下文件的文件名，添加到路径中
        if ((ret = dopath(func)) != 0)                  //递归调用本身
            break;                                      //出错
    }
    fullpath[n-1] = 0;                                  //去掉'/'
    if (closedir(dp) < 0)                               //关闭目录
        fprintf(stderr, "can't close directory %s\n", fullpath);
    return(ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch (type){
        case FTW_F:
            switch (statptr->st_mode & S_IFMT){
            case S_IFREG: nreg++; break;
            case S_IFBLK: nblk++; break;
            case S_IFCHR: nchr++; break;
            case S_IFIFO: nfifo++; break;
            case S_IFLNK: nslink++; break;
            case S_IFSOCK: nsock++; break;
            case S_IFDIR: fprintf(stderr, "for S_IFDIR for %s\n", pathname);
            }
            break;
        case FTW_D:
            ndir++;
            break;
        case FTW_DNR:
            fprintf(stderr, "can't read directory %s\n", pathname);
            break;
        case FTW_NS:
            fprintf(stderr, "stat error for %s\n", pathname);
            break;
        default:
            fprintf(stderr, "unkown type %d for pathname %s\n", type, pathname);
    }
    return(0);
}

