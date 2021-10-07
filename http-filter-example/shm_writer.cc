#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "shared_data.h" //shared memory variables and functions

using namespace std;

//Control-C handler to release shared memory
void ctrlc_handler(int signum){
   cout << endl << "Caught Ctrl-C, de-allocating shared memory and terminating....";
   if (shm_release() == 0)
      cout << endl << "Error de-allocating shared memory, terminating";
   exit(signum);
}  

int main(int argc, char *argv[]) {
   
   //Setup cntrl-c handler to remove shared memory
   signal(SIGINT, ctrlc_handler);

   cout << "Starting writer...allocating shared memory" << endl; 
   if(shm_allocate(SHM_CREATE) == 0) {
      cout << "Could not create and attach to shared memory, terminating" << endl;
      return EXIT_FAILURE; //fatal, exit.
   }
   
   cout << "Attached to shared memory!" << endl <<  endl;
   print_shm_info();

   // Read lines from terminal and store in shared memory
   //cin.ignore(numeric_limits<streamsize>::max(),'\n'); //flush input buffer
   shared_data->signal = 0;
   while(1){
      if(shared_data->signal == 0){   
         cout << endl << "Enter a line of text, Ctrl-C to quit" << endl;
         cin.getline(shared_data->str, MAX_LENGTH);
         shared_data->signal  = 1;
      }
      else {
         //cout << endl << "sleep" << shared_data->signal;
         sleep(1);
      }
   }  
   
   if (shm_release() == 0)
      cerr << endl << "Error de-allocating shared memory, terminating";
   return EXIT_SUCCESS;
}