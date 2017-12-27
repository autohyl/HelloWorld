#include <iostream>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
using namespace std;

union semun {   
   short val;          /*SETVAL用的�?*/  
  
   struct semid_ds* buf; /*IPC_STAT、IPC_SET用的semid_ds结构*/  
  
   unsigned short* array; /*SETALL、GETALL用的数组�?*/  
  
   struct seminfo *__buf;   /*为控制IPC_INFO提供的缓�?*/    
};

sembuf op_lock_[2] =
{
  {0, 0, 0},                    // Wait for [0] (lock) to equal 0
  {0, 1, SEM_UNDO},             // then increment [0] to 1 - this locks it.
                                // UNDO to release the lock if processes exit
                                // before explicitly unlocking.
};

int p(int semid) {
	struct sembuf temp_sembuf;
	temp_sembuf.sem_num = 0;
	temp_sembuf.sem_op = -1;
	temp_sembuf.sem_flg = SEM_UNDO;
	return semop(semid, &temp_sembuf, 1);
}

int v(int semid) {
	struct sembuf temp_sembuf;
	temp_sembuf.sem_num = 0;
	temp_sembuf.sem_op = 1;
	temp_sembuf.sem_flg = SEM_UNDO;
	return semop(semid, &temp_sembuf, 1);
}

int main() {
	int sem_id = semget(123, 1, IPC_CREAT | 0666);
	if (sem_id == -1) 
		cout << "semget error" << endl;
		
	/*
	if (semop (sem_id, &op_lock_[0], 2) == -1)
		cout << "semop error" << endl;

	semun semctl_arg;
	semctl_arg.val = 0;
	int semval = semctl(sem_id, 1, GETVAL, semctl_arg);
	cout << "semval: " << semval << endl;*/

	union semun num;
	num.val = 1;
	if (semctl(sem_id, 0, SETVAL, num) == -1) 
		cout << "semctl error: " << strerror(errno) << endl;	
	
	switch (fork()) {
		case -1:
			cout << "fork error" << endl;
			break;
		case 0:
			while (true) 
			{
				int child_sem = semget(123, 0, IPC_CREAT);
				if (child_sem == -1)
					cout << "semctl 1 error: " << strerror(errno) << endl;
				//cout << "child_sem: " << child_sem << endl;
				p(child_sem);
				cout << "A" << endl;
				usleep(1234);
				cout << "B" << endl;
				usleep(3456);
				v(child_sem);
			}			
			exit(1);
		default:
			while (true) {
				//cout << "sem_id: " << sem_id << endl;
				p(sem_id);
				cout << "C" << endl;
				usleep(4567);
				cout << "D" << endl;
				usleep(3241);
				v(sem_id);
			}
			wait(NULL);
			if (semctl(sem_id, 0, IPC_RMID, NULL)) 
				cout << "2 semctl error" << endl;
			break;
	}		

	return 0;
}
