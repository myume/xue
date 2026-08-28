#include "JobScheduler.h"
#include <mutex>
#include <print>
#include <pthread.h>
#include <sched.h>

JobScheduler::JobScheduler() {
    unsigned num_cores = std::thread::hardware_concurrency();
    for (auto core = 0; core < num_cores - 1; core++) {
        threadPool.emplace_back(&JobScheduler::processJobs, this);

        cpu_set_t cpuSet;
        CPU_ZERO(&cpuSet);
        CPU_SET(core, &cpuSet);
        pthread_setaffinity_np(threadPool.back().native_handle(),
                               sizeof(cpuSet), &cpuSet);
    }
    std::println("scheduler spawned {} threads", num_cores);
};

void JobScheduler::queueJob(std::function<void()> job) {
    {
        std::scoped_lock<std::mutex> lock(jobLock);
        jobs.push(job);
    }
    cv.notify_one();
}

void JobScheduler::processJobs() {
    while (!finished) {
        std::unique_lock<std::mutex> lock(jobLock);
        cv.wait(lock, [&] { return !jobs.empty() || finished; });
        if (!jobs.empty()) {
            auto job = jobs.front();
            jobs.pop();
            lock.unlock();
            job();
        } else {
            return;
        }
    }
};

JobScheduler::~JobScheduler() {
    finished = true;
    cv.notify_all();
    for (auto &thread : threadPool) {
        thread.join();
    }
};
