#ifndef _MAGQUEUE_H_  
#define _MAGQUEUE_H_  
#include <sys/ipc.h> //����ftok  
#include <sys/msg.h>  
#include <sys/types.h>  
  
//��Ϣ���еĶ� дģʽ����  
#define MSG_W 0200  
#define MSG_R 0400  
  
//����������֪����Ϣ���м�  
#define MQ_KEY1 128L  
  
#define DATA_SIZE 512  

typedef struct my_msgbuf  
{  
    long mtype ;  
    char mdata[DATA_SIZE] ;  
} mymsg_t;
  
#endif  