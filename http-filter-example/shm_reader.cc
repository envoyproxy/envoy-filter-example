#include "shared_data.h"

using namespace std;

int main(int argc, char *argv[]) {

   cout << "Starting reader...attaching to shared memory";
   if(shm_allocate(SHM_ATTACH) == 0) {
      cout << "Could not attach to shared memory, terminating" << endl;
      return EXIT_FAILURE; //fatal, exit.
   }
   cout << "Attached to shared memory!" << endl;
   print_shm_info();
   //cin.ignore(numeric_limits<streamsize>::max(),'\n'); //flush input buffer
   while(1){
      if(shared_data->signal == 1) {
         cout << endl << shared_data->str << endl << endl;
         shared_data->signal = 0;
      } 
      else {
         cout << "Waiting for input.." << endl;
         sleep(1);
      }
   }   
      if (shm_release() == 0)
      cout << "Error de-allocating shared memory, terminating" << endl;
   
   return EXIT_SUCCESS;
}