#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <iostream>

#include <ace/OS.h>
#include <ace/Auto_Ptr.h>
#include <ace/Singleton.h>
#include <ace/Synch.h>
#include <ace/Task.h>

class IOManager : public ACE_Task<ACE_MT_SYNCH> {

   public:
      enum { MAX_THREADS = 1 };
      
      virtual int open(void* = 0);
      
      virtual int put (ACE_Message_Block* mblk, ACE_Time_Value* timeout = 0);
      
      virtual int svc();
};

#endif
