add_subdirectory(allocators)
add_subdirectory(scheduler)

target_sources(xue PRIVATE RingBuffer.h)
