#include "io-manager.h"

int IOManager::open(void*) { return activate(THR_NEW_LWP, MAX_THREADS); }
      
int IOManager::put (ACE_Message_Block* mblk, ACE_Time_Value* timeout) { std::cout<<"bouttoput"<<std::endl; return putq(mblk, timeout); }
      
int IOManager::svc()
{
   while(true) {
      std::cout<<"hello"<<std::endl;
      for (ACE_Message_Block* log_blk; getq(log_blk) != -1; ) {
         std::cout << "hi" << std::endl;
      }
   }
   return 0;
}
