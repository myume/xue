#include "JobScheduler.h"
#include <print>

int main() {
    JobScheduler scheduler;
    for (auto i = 0; i < 100; i++) {
        scheduler.queueJob([i] { std::println("{}", i); });
    }
}
