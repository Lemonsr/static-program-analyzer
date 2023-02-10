#pragma once

#include <cstdint>
#include <vector>

namespace spa {
template <typename T>
class Stream {
 private:
    std::vector<T> items;
    int64_t start = 0;

 public:
    void resetStartPosition() {
      start = 0;
    }

    void pushBack(T item) {
      items.push_back(item);
    }

    T& operator[](int64_t offset) {
      return items[start + offset];
    }

    int64_t remaining() {
      return items.size() - start;
    }

    void seek(int64_t offset) {
      start += offset;
    }

    bool match(int64_t offset, std::vector<T> compare) {
      if (remaining() - offset < compare.size()) {
        return false;
      }
      for (int64_t i = 0; i < compare.size(); ++i) {
        if (this->operator[](offset + i) != compare[i]) {
            return false;
        }
      }
      return true;
    }

    bool match(std::vector<T> compare) {
      return match(0, compare);
    }

    int64_t find(T item) {
      for (int64_t i = 0; i < this->remaining(); ++i) {
        if (this->operator[](offset + i) == item) {
          return i;
        }
      }
      return -1;
    }
};
}  // namespace spa
