#ifndef DEQUE_H
#define DEQUE_H

#include <vector>

template <typename T>
class Deque {
private:
    static constexpr bucket_size = 10;
    std::vector<T*> arr;
public:
    Deque() = default;
    Deque(const T&) {
        
    }
}; 


#endif