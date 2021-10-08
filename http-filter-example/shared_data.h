#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>


#define SHM_KEY 0x1234

const int SHM_ACCESS = 0666;
const int SHM_CREATE = SHM_ACCESS|IPC_CREAT;
const int SHM_ATTACH = SHM_ACCESS;
const int MAX_LENGTH = 1024;
const char QUIT[] = "quit";

using namespace std;

//simple data structure shared by both processes
struct shared_data {
   char str[MAX_LENGTH];
   int signal;
};

//variables common to writer and reader process
int shmid;
struct shared_data *shared_data;
struct shmid_ds shm_stats;


//Create and/or attach to shared memory
int shm_allocate(int shm_mode){
   shmid = shmget(SHM_KEY, sizeof(struct shared_data), shm_mode);
   if (shmid == -1){
      cout << "shmget error:" << errno << endl << endl;
      return 0;
   }    
   // shared_data = (struct shared_data *)shmat(shmid, (void*) 0, 0);
   void *ret = shmat(shmid, NULL, 0);
   // int *data = reinterpret_cast<int*>(ret);
   // if ( data == NULL || *data == -1 ) {
   //      cout << "shmat error:" << errno << endl << endl;
   //      return 0;
   // }
   shared_data = reinterpret_cast<struct shared_data*>(ret);
   if ( shared_data == NULL ) {
        cout << "shmat error:" << errno << endl << endl;
        return 0;
   }

   return 1; //success
}

//Detach from and deallocate shared memory
int shm_release(){
   if (shmdt(shared_data) == -1)
      return 0;

   if (shmctl(shmid, IPC_RMID, 0) == -1)
      return 0;
   
   return 1; //success
}

void print_shm_info(){
   if(shmctl(shmid, IPC_STAT, &shm_stats) == 0) {
      cout << endl << "Size of segment in bytes: " << shm_stats.shm_segsz << endl;
      cout << "Processes attached: " << shm_stats.shm_nattch << endl << endl; 
   }
   else
      cout << endl << "Error fetching shared memory info" << endl << endl;   
}
