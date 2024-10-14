#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void  ChildProcess(int []);
void ParentProcess(int []);

int  main(int  argc, char *argv[])
{

     srand(time(NULL));
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;



     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of two integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = 0; // BackAccount 
     ShmPTR[1] = 0; // Turn

     printf("Orig Bank Account = %d\n",
            ShmPTR[0]);
  
     printf("Server is about to fork a child process...\n");

     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          ChildProcess(ShmPTR);
          exit(0);
     }
     else{
      ParentProcess(ShmPTR);
     }

     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  ParentProcess(int  SharedMem[])
{
    int account;
    int balance;
    int random_time;
    int loopct;
    for(loopct = 0; loopct < 20; loopct++){
          random_time = rand() % (5-0+1);
          sleep(random_time);
          account = SharedMem[0];
          while(SharedMem[1] != 0);
            if(account <= 100){
              balance = rand() % (100-0+1);
              if (balance%2 == 0){
                account = account + balance;
                printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
              }
              else{
                printf("Dear old Dad: Doesn't have any money to give\n");
              }
            }
            else{
              printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
            }
            SharedMem[0] = account;
            SharedMem[1] = 1;
          
    }
}


void  ChildProcess(int  SharedMem[])
{
    int account;
    int balance;
    int random_time;
    int loopct;
    for(loopct = 0; loopct < 20; loopct++){
          random_time = rand() % (5-0+1);
          sleep(random_time);
          account = SharedMem[0];
          while(SharedMem[1] != 1);
            balance = rand() % (50-0+1);
            printf("Poor Student needs $%d\n", balance);
            if(balance <= account){
              account = account - balance;
              printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
            }
            else{
              printf("Poor Student: Not Enough Cash ($%d)\n", account );
            }
            SharedMem[0] = account;
            SharedMem[1] = 0;
          }
    
}