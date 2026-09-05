#pragma once

#include "types.h"
#include "ustl/ustring.h"
#include "ustl/uvector.h"

#include "UserThread.h"
#include "Mutex.h"
#include "ScopeLock.h"

class Thread;
class UserThread;
class Loader;
class FileSystemInfo;
class Terminal;

class UserProcess
{
  public:
    /**
     * Constructor
     * @param minixfs_filename filename of the file in minixfs to execute
     * @param fs_info filesysteminfo-object to be used
     * @param terminal_number the terminal to run in (default 0)
     *
     */
    UserProcess(ustl::string minixfs_filename, FileSystemInfo *fs_info, uint32 terminal_number = 0);

    virtual ~UserProcess();

    FileSystemInfo* getWorkingDirInfo();
    void setWorkingDirInfo(FileSystemInfo* working_dir);

    Terminal* getTerminal();

    void setTerminal(Terminal *my_term);

    void* allocateUserStack();

    bool addThread(UserThread* thread);
    bool removeThread(UserThread* thread);
    void removeRemainingThreads();

    void markAsTerminating();

    size_t pid_;

    Loader* loader_;


    Mutex mutex_tid_;

    size_t getTid() {return max_tid_;}
    void incrementTid() {max_tid_++;}
    Mutex mutex_thread_list_{"UserProcess::mutex_thread_list_"};

    UserThread* getUserThreadByTID(size_t tid);

  private:
    int32 fd_;

    Terminal* my_terminal_;

    FileSystemInfo* working_dir_;

    bool isTerminating{false};

    ustl::vector<UserThread*> thread_list_;

    size_t max_tid_;
    size_t threads_created{0}; //Temporary till SLR is implemented
};

