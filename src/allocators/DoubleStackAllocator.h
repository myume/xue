#pragma once

#include <cstdlib>

class DoubleStackAllocator {
  private:
    class Marker {
      private:
        friend DoubleStackAllocator;

        bool high;
        size_t value;

        Marker(bool high, size_t val) : high(high), value(val) {}
    };

    void *memory;
    size_t capacity;
    size_t high;
    size_t low = 0;

  public:
    DoubleStackAllocator(size_t capacity);
    ~DoubleStackAllocator();

    DoubleStackAllocator(DoubleStackAllocator &&) noexcept;
    DoubleStackAllocator &operator=(DoubleStackAllocator &&);

    DoubleStackAllocator(const DoubleStackAllocator &) = delete;
    DoubleStackAllocator &operator=(const DoubleStackAllocator &) = delete;

    void *allocHigh(size_t size);
    void *allocLow(size_t size);

    Marker lowMarker();
    Marker highMarker();

    void freeToMarker(Marker mark);
    void freeToLowMarker(Marker mark);
    void freeToHighMarker(Marker mark);

    void clearLower() noexcept;
    void clearUpper() noexcept;
    void clear() noexcept;
};
