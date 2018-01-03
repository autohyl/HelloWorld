//��Ϣ������˫��ͨ�ŵģ����õ������о͹��á�  
//������ÿ����Ϣ����������ʶ����Ϣ�Ǵӿͻ��������������Ǵӷ��������ͻ���  
//�ͻ�����з�����Ϊ1��PID��·������  
//����������з�����Ϊ�ͻ�����ID���ļ����ݡ�  
//  
//С������������  
//�ͻ��ǿ���������Ϣ���У���������������Ӧ�����ǿͻ��������ڷ����У�������Ҳ��������  
//����ķ����ǣ�Լ������������Ϣ��������ʹ�÷�����д��  
  
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
  
    //������Ϣ����  
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
        //����Ϣ�����ж�ȡ���Կͻ��������ļ�·��  
        msgPtr = (mymsg_t*)malloc(DATA_SIZE + sizeof(long)) ;  
        recvBytes = msgrcv(readfd, msgPtr, DATA_SIZE, 1, 0) ; //������  
        if (recvBytes <= 0)  
        {  
            puts("pathname missing") ;  
            continue ;  
        }  
        msgPtr->mdata[recvBytes] = '\0' ;  

        fprintf(stdout, "recv msg %s\n", msgPtr->mdata);
  
        //������Ϣ����ȡ�ͻ�PID���ļ�·��  
        if ((pathStr = strchr(msgPtr->mdata, ' ')) == NULL)  
        {  
            puts("bogus request!") ;  
            continue ;  
        }  
        *pathStr++ = 0 ;  
        clientPid = atol(msgPtr->mdata) ;  
  
        //��ȡ�ļ����� ���ظ��ͻ�  
        msgPtr->mtype = clientPid ; //msgPtr����Ϊ������Ϣ ������������Ϣ  
        if ((fp = fopen(pathStr, "r")) == NULL)  
        {  
            //��ȡ�ļ�ʧ�ܣ����ظ��ͻ�ʧ����Ϣ����ԭ��Ϣ���ݺ� ��Ӵ�����Ϣ��  
            snprintf(msgPtr->mdata + recvBytes, sizeof(msgPtr->mdata) -recvBytes,   
                    ": can't open!") ;  
  
            if (msgsnd(writefd, msgPtr, strlen(msgPtr->mdata), IPC_NOWAIT) == -1)  
            {  
                puts("Send Error!");  
                exit(0);  
            }  
        }  
        else  
        {   //copy�ļ����� �����ͻ�  
            while (fgets(msgPtr->mdata, DATA_SIZE, fp) != NULL)  
            {  
                msgsnd(writefd, msgPtr, strlen(msgPtr->mdata), IPC_NOWAIT) ; //������д  
            }  
        }  
    }//for()  
}  