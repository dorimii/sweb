#include "UserThread.h"

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

UserThread::~UserThread(){
  // FIX RACECONDITION
  my_process_->removeThread(this);
  if(my_process_ && my_process_->getThreadList().size() == 0){
    delete my_process_;
  }
}

void UserThread::Run()
{
  debug(USERPROCESS, "Run: Fail-safe kernel panic - you probably have forgotten to set switch_to_userspace_ = 1\n");
  assert(false);
}
