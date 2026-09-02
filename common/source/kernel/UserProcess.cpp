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
#include "ustl/ualgo.h"

UserProcess::UserProcess(ustl::string filename, FileSystemInfo *fs_info, uint32 terminal_number) : 
  pid_(0), loader_(0), fd_(VfsSyscall::open(filename, O_RDONLY)), my_terminal_(0), working_dir_(fs_info)
    // Thread(fs_info, filename, Thread::USER_THREAD), fd_(VfsSyscall::open(filename, O_RDONLY))
{
  ProcessRegistry::instance()->processStart(); //should also be called if you fork a process

  if (main_console->getTerminal(terminal_number))
    setTerminal(main_console->getTerminal(terminal_number));

  if (fd_ >= 0)
    loader_ = new Loader(fd_);

  if (!loader_ || !loader_->loadExecutableAndInitProcess())
  {
    debug(USERPROCESS, "Error: loading %s failed!\n", filename.c_str());
    ProcessRegistry::instance()->processExit();
    return;
  }

  UserThread* main_thread = new UserThread(this, filename, loader_);
  addThread(main_thread);

  Thread* thread = main_thread;
  Scheduler::instance()->addNewThread(thread);

  debug(USERPROCESS, "ctor: Done loading %s\n", filename.c_str());
}

FileSystemInfo* UserProcess::getWorkingDirInfo()
{
  return working_dir_;
}

void UserProcess::setWorkingDirInfo(FileSystemInfo* working_dir)
{
  working_dir_ = working_dir;
}

Terminal *UserProcess::getTerminal()
{
  return my_terminal_ ? my_terminal_ : main_console->getActiveTerminal();
}

void UserProcess::setTerminal(Terminal *my_term)
{
  my_terminal_ = my_term;
}

UserProcess::~UserProcess()
{
  assert(Scheduler::instance()->isCurrentlyCleaningUp());

  delete loader_;
  loader_ = 0;

  if (fd_ >= 0)
    VfsSyscall::close(fd_);

  delete working_dir_;
  working_dir_ = 0;

  thread_list_.clear();

  ProcessRegistry::instance()->processExit();
}

bool UserProcess::addThread(UserThread* thread){
  ScopeLock lock(mutex_thread_list_);
  if(isTerminating) return false;

  thread_list_.push_back(thread);
  return true;
}

bool UserProcess::removeThread(UserThread* thread)
{
  ScopeLock lock(mutex_thread_list_);
  for(auto it = thread_list_.begin(); it != thread_list_.end(); ++it){
    if(*it == thread){
      thread_list_.erase(it);
      break;
    }
  }
  return thread_list_.empty();
}

void UserProcess::removeRemainingThreads(){
  ScopeLock lock(mutex_thread_list_);
  for(auto thread : thread_list_){
    if(thread != currentThread){
      thread->kill();
    }
  }
}

// allocates 1 page atm + 1 guard page
void* UserProcess::allocateUserStack(){
  ScopeLock lock(mutex_thread_list_);
  size_t stack_vpn = ((USER_BREAK / PAGE_SIZE) - 1) - (++threads_created * 2); // allocate 1 page + 1 guard page
  size_t page_for_stack = PageManager::instance()->allocPPN();
  bool vpn_mapped = loader_->arch_memory_.mapPage(stack_vpn, page_for_stack, 1);
  assert(vpn_mapped && "Virtual page for stack was already mapped - this should never happen");

  return (void*)((stack_vpn + 1) * PAGE_SIZE - sizeof(pointer));
}

ustl::vector<UserThread*> UserProcess::getThreadList() {
  ScopeLock lock(mutex_thread_list_);
  return thread_list_;
}

void UserProcess::markAsTerminating(){
  ScopeLock lock(mutex_thread_list_);
  isTerminating = true;
}

