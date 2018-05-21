#include <stdlib.h>
#include <iostream>

#include "include/heaptest.h"

using namespace std;

int MINIBASE_RESTART_FLAG = 0;

int main(int argc, char **argv)
{
   HeapDriver hd;
   Status dbstatus;

   dbstatus = hd.RunTests();

   // Check if the create database has succeeded
   if (dbstatus != OK) {       
      cout << "Error encountered during hfpage tests: " << endl;
      minibase_errors.show_errors();      
      return(1);
   }
   
   return(0);
}


