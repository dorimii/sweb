#include "ProcessRegistry.h"
#include "UserProcess.h"
#include "kprintf.h"
#include "Console.h"
#include "Loader.h"
#include "VfsSyscall.h"
#include "File.h"
#include "PageManager.h"
#include "ArchThreads.h"
#include "offsets.h"
#include "Scheduler.h"
#include "Thread.h"

UserProcess::UserProcess(ustl::string filename, FileSystemInfo *fs_info, uint32 terminal_number):
  fd_(VfsSyscall::open(filename.c_str(), O_RDONLY)), working_dir_(fs_info)
{
  ProcessRegistry::instance()->processStart(); //should also be called if you fork a process

  if (fd_ >= 0)
    // open executable file and create an ELF/binary loader
    loader_ = new Loader(fd_);

  // allocate memory for program code, data, BSS segment
  if (!loader_ || !loader_->loadExecutableAndInitProcess())
  {
    debug(USERPROCESS, "Error: loading %s failed!\n", filename.c_str());
    return;
  }

  // allocate physical page frame and map it to virtual memory below USER BREAK
  size_t page_for_stack = PageManager::instance()->allocPPN();
  bool vpn_mapped = loader_->arch_memory_.mapPage(USER_BREAK / PAGE_SIZE - 1, page_for_stack, 1);
  assert(vpn_mapped && "Virtual page for stack was already mapped - this should never happen");

  Thread* thread = new Thread(filename,Thread::USER_THREAD, this);
  thread->setUserStackPointer((void*)(USER_BREAK - sizeof(pointer)));
  addThread(thread);

  // set up user registers
  ArchThreads::createUserRegisters(thread->user_registers_, loader_->getEntryFunction(),
                                    thread->getUserStackPointer(),
                                    thread->getKernelStackStartPointer());

  // bind process page directory to this specific thread
  ArchThreads::setAddressSpace(thread, loader_->arch_memory_);

  debug(USERPROCESS, "ctor: Done loading %s\n", filename.c_str());

  if (main_console->getTerminal(terminal_number))
    thread->setTerminal(main_console->getTerminal(terminal_number));

  thread->switch_to_userspace_ = 1;

  Scheduler::instance()->addNewThread(thread);
}

void UserProcess::removeThread(Thread *thread)
{
  for (auto i = threads_.begin(); i != threads_.end(); ++i) {
    if (*i == thread) {
      threads_.erase(i);
      break;
    }
  }
}

UserProcess::~UserProcess()
{
  for (size_t i = 0; i < threads_.size(); i++) {
    Thread* thread = threads_[i];
    if (thread) {
      thread->setUserStackPointer(nullptr);
      delete thread;
    }
  }
  threads_.clear();

  assert(Scheduler::instance()->isCurrentlyCleaningUp());
  delete loader_;
  loader_ = 0;

  if (fd_ > 0)
    VfsSyscall::close(fd_);

  delete working_dir_;
  working_dir_ = 0;

  ProcessRegistry::instance()->processExit();
}

void UserProcess::Run()
{
  debug(USERPROCESS, "Run: Fail-safe kernel panic - you probably have forgotten to set switch_to_userspace_ = 1\n");
  assert(false);
}

FileSystemInfo* UserProcess::getWorkingDirInfo()
{
  return working_dir_;
}


void UserProcess::setWorkingDirInfo(FileSystemInfo* working_dir)
{
  working_dir_ = working_dir;
}
