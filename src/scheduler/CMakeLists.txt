add_library(xue_scheduler STATIC
    JobScheduler.h
    JobScheduler.cpp
)

target_include_directories(xue_scheduler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(job_example example/main.cpp)

target_link_libraries(job_example PRIVATE xue_scheduler)
