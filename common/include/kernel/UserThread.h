#pragma once

#include "types.h"
#include "fs/FileSystemInfo.h"

#include "Thread.h"
#include "UserProcess.h"
#include "Mutex.h"

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

    virtual UserProcess* getUserProcess() const override {
      return my_process_;
    }

    virtual bool isUserThread() const override { 
      return true; 
    }

    virtual CancelType getCancelType() const override { 
      return cancel_type_; 
    };
    virtual CancelType setCancelType(CancelType type);
    
    virtual CancelState getCancelState() const override { 
      return cancel_state_; 
    };
    virtual CancelState setCancelState(CancelState state);

    virtual bool isToBeCanceled() const override { 
      return toBeCanceled_; 
    };
    virtual void setToBeCanceled(bool state) { 
      toBeCanceled_ = state; 
    };

  private:
    UserThread(UserThread const &src);
    UserThread &operator=(UserThread const &src);

    UserProcess* my_process_;

    Mutex mutex_toBeCanceled_{"UserThhread::mutex_toBeCanceled_"};
    Mutex mutex_cancel_type_{"UserThhread::mutex_cancel_type_"};
    Mutex mutex_cancel_state_{"UserThhread::mutex_cancel_state_"};

    bool toBeCanceled_{false};
    CancelType cancel_type_{PTHREAD_CANCEL_DEFERRED};
    CancelState cancel_state_{PTHREAD_CANCEL_ENABLE};
};

