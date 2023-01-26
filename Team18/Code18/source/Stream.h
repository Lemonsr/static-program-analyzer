#pragma once

#include <vector>

namespace spa {
    template <typename T>
    class Stream {
    private:
        std::vector<T> items;
        long long start = 0;
    public:
        void pushBack(T item) {
            items.push_back(item);
        }

        T& operator[](long long offset) {
            return items[start + offset];
        }

        long long remaining() {
            return items.size() - start;
        }

        void seek(long long offset) {
            start += offset;
        }

        bool match(long long offset, std::vector<T> compare) {
            if (remaining() - offset < compare.size()) {
                return false;
            }
            for (long long i = 0; i < compare.size(); ++i) {
                if (this->operator[](offset + i) != compare[i]) {
                    return false;
                }
            }
            return true;
        }

        bool match(std::vector<T> compare) {
            return match(0, compare);
        }
    };
}
