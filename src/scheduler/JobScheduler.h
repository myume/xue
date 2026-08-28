#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class JobScheduler {
  private:
    std::queue<std::function<void()>> jobs;
    std::mutex jobLock;
    std::condition_variable cv;

    std::atomic_bool finished;

    std::vector<std::thread> threadPool;

    void processJobs();

  public:
    JobScheduler();
    ~JobScheduler();

    void queueJob(std::function<void()> job);
};
