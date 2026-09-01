#pragma once

#include "types.h"
#include "ustl/ustring.h"
#include "ustl/uvector.h"

#include "UserThread.h"

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

    void addThread(UserThread* thread);
    void removeThread(UserThread* thread);

    ustl::vector<UserThread*> getThreadList() const {
      return thread_list_;
    }

    size_t pid_;

    Loader* loader_;

  private:
    int32 fd_;

    Terminal* my_terminal_;

    FileSystemInfo* working_dir_;

    ustl::vector<UserThread*> thread_list_;
};

