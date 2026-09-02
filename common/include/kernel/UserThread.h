#pragma once

#include "types.h"
#include "fs/FileSystemInfo.h"

#include "Thread.h"
#include "UserProcess.h"

class Thread;
class UserThread;
class UserProcess;
class ArchThreadRegisters;
class Mutex;
class Lock;

extern Thread* currentThread;

class UserThread : public Thread
{
  public:
    UserThread(UserProcess* process, ustl::string name, Loader* loader);

    UserThread(UserProcess* process, ustl::string name, Loader* loader, void* entry_function, void *start_routine, void *arg);

    virtual ~UserThread();

     /**
     * runs whatever the user wants it to run;
     */
    virtual void Run();

    UserProcess* getUserProcess() const {
      return my_process_;
    }

  private:
    UserThread(UserThread const &src);
    UserThread &operator=(UserThread const &src);

    UserProcess* const my_process_;
};

