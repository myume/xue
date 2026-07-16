#include <cstddef>
#include <stdexcept>

template <typename T, size_t N>
class RingBuffer {
  private:
    T arr[N];
    size_t start = 0;
    size_t end = 0;

  public:
    T read() {
        if (start == end) {
            throw std::runtime_error("reading from empty buffer");
        }
        return arr[start++ % N];
    }

    void write(T val) {
        if (end - start == N) {
            // could overwrite data instead of throwing
            throw std::runtime_error("buffer is full");
        }
        arr[end++ % N] = val;
    };
};
