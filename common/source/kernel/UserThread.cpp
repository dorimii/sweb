#include "UserThread.h"
#include "../../include/kernel/UserThread.h"

#include "UserProcess.h"
#include "kprintf.h"
#include "ArchThreads.h"
#include "ArchInterrupts.h"
#include "Scheduler.h"
#include "Loader.h"
#include "Console.h"
#include "Terminal.h"
#include "backtrace.h"
#include "KernelMemoryManager.h"
#include "Stabs2DebugInfo.h"

UserThread::UserThread(UserProcess* process, ustl::string name, Loader* loader) : Thread(nullptr, name, Thread::USER_THREAD),
    my_process_(process)
{
  assert(my_process_ != nullptr && "[Error]: Thread needs to have a process.");
  assert(my_process_->loader_ && "[Error]: Process must have a valid loader.");

  loader_ = loader;

  void* stack_top = my_process_->allocateUserStack();

  ArchThreads::createUserRegisters(
    user_registers_, 
    my_process_->loader_->getEntryFunction(),
    stack_top,
    getKernelStackStartPointer()
  );

  ArchThreads::setAddressSpace(this, my_process_->loader_->arch_memory_);

  switch_to_userspace_ = 1;
}

UserThread::UserThread(UserProcess* process, ustl::string name, Loader* loader, void* pthread_wrapper, void *start_routine, void *arg):
    Thread(nullptr, name, Thread::USER_THREAD), my_process_(process)
{
  assert(my_process_ != nullptr && "[Error]: Thread needs to have a process.");
  assert(my_process_->loader_ && "[Error]: Process must have a valid loader.");

  loader_ = loader;

  process->mutex_tid_.acquire();
  setTID(process->getTid());
  process->incrementTid();
  process->mutex_tid_.release();

  void* stack_top = my_process_->allocateUserStack();

  ArchThreads::createUserRegisters(
    user_registers_,
    pthread_wrapper,
    stack_top,
    getKernelStackStartPointer()
  );

  user_registers_->rdi = (size_t)start_routine;
  user_registers_->rsi = (size_t)arg;

  ArchThreads::setAddressSpace(this, my_process_->loader_->arch_memory_);

  switch_to_userspace_ = 1;
}

UserThread::~UserThread(){
  if(my_process_ && my_process_->removeThread(this)){
    delete my_process_;
    my_process_ = nullptr;
  }
}

void UserThread::Run()
{
  debug(USERPROCESS, "Run: Fail-safe kernel panic - you probably have forgotten to set switch_to_userspace_ = 1\n");
  assert(false);
}

CancelType UserThread::setCancelType(CancelType type) {
  ScopeLock lock(mutex_cancel_type_);

  CancelType old_type = cancel_type_; 
  cancel_type_ = type;
  return old_type;
};

CancelState UserThread::setCancelState(CancelState state) { 
  ScopeLock lock(mutex_cancel_state_);

  CancelState old_state = cancel_state_;
  cancel_state_ = state; 
  return old_state;
};
