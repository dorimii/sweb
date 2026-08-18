#pragma once

#include "Loader.h"
#include "Thread.h"
#include "fs/FileSystemInfo.h"
#include <uvector.h>

// tight now every process is one thread
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

    void setWorkingDirInfo(FileSystemInfo* working_dir);

    FileSystemInfo* getWorkingDirInfo();

    Loader* getLoader() {return loader_;};

    void addThread(Thread* thread) {threads_.push_back(thread);};
    void removeThread(Thread* thread);

    virtual void Run(); // not used

  private:
    int32 fd_;

    Loader* loader_;

    ustl::vector<Thread*> threads_;

  protected:
    FileSystemInfo* working_dir_;
};

