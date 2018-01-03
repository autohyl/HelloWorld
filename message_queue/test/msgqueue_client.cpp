#include "msgqueue.h"  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
  
void client(int, int) ;  
  
int   
main(int argc, char** argv)  
{  
    int  msgqid ;  
  
    //打开消息队列  
    msgqid = msgget(MQ_KEY1, 0) ;  
    if (msgqid < 0)  
    {  
        puts("Open msg queue error!\n") ;  
        exit(0) ;  
    }  
  
    client(msgqid, msgqid) ;  
    exit(0) ;  
}  
  
void  
client(int readfd, int writefd)  
{  
    mymsg_t msgToServer ;  
    mymsg_t msgFromServer ;  
    char*   writePtr ;  
    ssize_t pidLen ;  
    ssize_t dataLen ;  
    ssize_t recvBytes ;  
    int     pid ;  

    memset(&msgToServer, 0, sizeof(mymsg_t));
  
    //-------构造一条消息-----  
    //消息类型为1  
    msgToServer.mtype = 1 ;  
  
    //在消息头部放本进程ID和空格  
    pid = getpid() ;  
    snprintf(msgToServer.mdata, DATA_SIZE, "%ld ", pid) ;  
    pidLen = strlen(msgToServer.mdata) ;  
    writePtr = msgToServer.mdata + pidLen ;  
  
    //从标准输入读入文件路径  
    fgets(writePtr, DATA_SIZE - pidLen, stdin) ;  
    dataLen = strlen(msgToServer.mdata) ;    
    if (msgToServer.mdata[dataLen-1] == '\n') //删除换行符  
    {  
        msgToServer.mdata[dataLen-1] = '\0' ;  
    }  
  
    //发送消息  
    if (msgsnd(writefd, &msgToServer, strlen(msgToServer.mdata), 0) == -1)  
    {  
        puts("Send Error!");  
        exit(0) ;  
    }  
  
    //-----接收来自服务器的消息  
    while ((recvBytes = msgrcv(readfd, &msgFromServer, DATA_SIZE, pid, 0)) > 0)  
    {  
        write(STDOUT_FILENO, msgFromServer.mdata, recvBytes) ;  
    }  
  
}  