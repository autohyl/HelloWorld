//消息队列是双向通信的，故用单个队列就够用。  
//我们用每个消息的类型来标识该消息是从客户到服务器，还是从服务器到客户。  
//客户向队列发类型为1、PID和路径名。  
//服务器向队列发类型为客户进程ID的文件内容。  
//  
//小心死锁隐患：  
//客户们可以填满消息队列，妨碍服务器发送应答，于是客户被阻塞在发送中，服务器也被阻塞。  
//避免的方法是：约定服务器对消息队列总是使用非阻塞写。  
  
#include "msgqueue.h"  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>
  
void server(int, int) ;  
  
int   
main(int argc, char** argv)  
{  
    int  msgqid;  
  
    //创建消息队列  
    msgqid = msgget(MQ_KEY1, IPC_CREAT) ;  
    if (msgqid < 0)  
    {  
        puts("Create msg queue error!\n") ;  
        exit(0) ;  
    }  
  
    server(msgqid, msgqid) ;  
    exit(0) ;  
}  
  
void  
server(int readfd, int writefd)  
{  
    FILE*    fp ;  
    pid_t    clientPid ;  
    mymsg_t* msgPtr ;  
    ssize_t  recvBytes ;  
    char*    pathStr ;      
  
    for ( ; ; )  
    {  
        //从消息队列中读取来自客户的请求文件路径  
        msgPtr = (mymsg_t*)malloc(DATA_SIZE + sizeof(long)) ;  
        recvBytes = msgrcv(readfd, msgPtr, DATA_SIZE, 1, 0) ; //阻塞读  
        if (recvBytes <= 0)  
        {  
            puts("pathname missing") ;  
            continue ;  
        }  
        msgPtr->mdata[recvBytes] = '\0' ;  

        fprintf(stdout, "recv msg %s\n", msgPtr->mdata);
  
        //分析消息，提取客户PID，文件路径  
        if ((pathStr = strchr(msgPtr->mdata, ' ')) == NULL)  
        {  
            puts("bogus request!") ;  
            continue ;  
        }  
        *pathStr++ = 0 ;  
        clientPid = atol(msgPtr->mdata) ;  
  
        //读取文件内容 返回给客户  
        msgPtr->mtype = clientPid ; //msgPtr既作为接收消息 又用作发送消息  
        if ((fp = fopen(pathStr, "r")) == NULL)  
        {  
            //读取文件失败，返回给客户失败信息（在原消息内容后 添加错误信息）  
            snprintf(msgPtr->mdata + recvBytes, sizeof(msgPtr->mdata) -recvBytes,   
                    ": can't open!") ;  
  
            if (msgsnd(writefd, msgPtr, strlen(msgPtr->mdata), IPC_NOWAIT) == -1)  
            {  
                puts("Send Error!");  
                exit(0);  
            }  
        }  
        else  
        {   //copy文件内容 发给客户  
            while (fgets(msgPtr->mdata, DATA_SIZE, fp) != NULL)  
            {  
                msgsnd(writefd, msgPtr, strlen(msgPtr->mdata), IPC_NOWAIT) ; //非阻塞写  
            }  
        }  
    }//for()  
}  