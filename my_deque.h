#ifndef DEQUE_H
#define DEQUE_H

#ifdef _DEBUG
#include <iostream>
#endif /* _DEBUG */

#include <vector>
#include <utility>
#include <type_traits>
#include <iterator>
#include <stdexcept>
#include <stddef.h>
#include <stdint.h>


template <typename T>
class Deque {
private:
    static constexpr size_t bucket_size = 10;

    std::vector<T*> arr;
    size_t bucket_count;
    size_t sz;
    size_t cap;
    std::pair<int, int> begin_pos;
    std::pair<int, int> end_pos;

    std::pair<int, int> pos_calc(const std::pair<int, int>& pos, size_t offset);
    void pos_forward(std::pair<int, int>& pos);
    void pos_back(std::pair<int, int>& pos);

    void expand_back(size_t n = 2);
    void expand_front(size_t n = 2);

    bool is_index_in_range(int i, int j) const;
    bool is_index_in_range(const std::pair<int, int>& val) const;

    template <bool IsConst>
    class common_iterator {
    private:
        using ConditionalArr  = std::conditional_t<IsConst, const std::vector<T*>, std::vector<T*>>;
        using ConditionalPtr  = std::conditional_t<IsConst, const T*, T*>;
        using ConditionalRef  = std::conditional_t<IsConst, const T&, T&>;
        using ConditionalType = std::conditional_t<IsConst, const T, T>;

        std::pair<int, int> it_pos;
        ConditionalPtr      it_ptr;
        ConditionalArr&     arr_ref;

        void pos_calc(std::pair<int, int>& pos, size_t offset) const {
            size_t begin = pos.first * bucket_size + pos.second;
            size_t val   = begin + offset;
            pos.first    = val / bucket_size;
            pos.second   = val % bucket_size;
        }

    public:
        using iterator_category      = std::random_access_iterator_tag;
        using difference_type        = std::ptrdiff_t;
        using value_type             = ConditionalType;
        using pointer                = ConditionalPtr;
        using reference              = ConditionalRef;

        common_iterator(const common_iterator<IsConst>& common_iterator) = default;
        common_iterator(ConditionalArr& arr, const std::pair<int, int>& pos) 
        : it_pos(pos), it_ptr(arr[pos.first] + pos.second), arr_ref(arr) {};

        common_iterator& operator++() {
            if (it_pos.second < bucket_size - 1) {
                ++it_pos.second;
                ++it_ptr;
            } else {
                ++it_pos.first;
                it_pos.second = 0;
                it_ptr = arr_ref[it_pos.first] + it_pos.second;
            }
            return *this;
        }
        
        common_iterator& operator--() {
            if (it_pos.second > 0) {
                --it_pos.second;
                --it_ptr;
            } else {
                --it_pos.first;
                it_pos.second = bucket_size - 1;
                it_ptr = arr_ref[it_pos.first] + it_pos.second;
            }
            return *this;
        }

        common_iterator operator++(int) {
            common_iterator old = *this;
            ++(*this);
            return old;
        }
        
        common_iterator operator--(int) {
            common_iterator old = *this;
            --(*this);
            return old;
        }

        common_iterator& operator+=(difference_type n) {
            pos_calc(it_pos, n);
            it_ptr = arr_ref[it_pos.first] + it_pos.second;
            return *this;
        }

        common_iterator& operator-=(difference_type n) {
            pos_calc(it_pos, -n);
            it_ptr = arr_ref[it_pos.first] + it_pos.second;
            return *this;
        }

        common_iterator operator+(difference_type n) const {
            common_iterator tmp = *this;
            pos_calc(tmp.it_pos, n);
            tmp.it_ptr = arr_ref[tmp.it_pos.first] + tmp.it_pos.second;
            return tmp;
        }

        common_iterator operator-(difference_type n) const {
            common_iterator tmp = *this;
            pos_calc(tmp.it_pos, -n);
            tmp.it_ptr = arr_ref[tmp.it_pos.first] + tmp.it_pos.second;
            return tmp;
        }

        bool operator<(const common_iterator<IsConst>& other) const {
            return it_pos < other.it_pos;
        }

        bool operator<=(const common_iterator<IsConst>& other) const {
            return it_pos <= other.it_pos;
        }

        bool operator>=(const common_iterator<IsConst>& other) const {
            return it_pos >= other.it_pos;
        }

        bool operator>(const common_iterator<IsConst>& other) const {
            return it_pos > other.it_pos;
        }

        bool operator==(const common_iterator<IsConst>& other) const {
            return it_pos == other.it_pos;
        }

        bool operator!=(const common_iterator<IsConst>& other) const {
            return it_pos != other.it_pos;
        }

        difference_type operator-(const common_iterator<IsConst>& other) const {
            return (it_pos.first - other.it_pos.first) * bucket_size + (it_pos.second - other.it_pos.second);
        }

        ConditionalRef operator*() {
            return *it_ptr;
        }

        ConditionalPtr operator->() {
            return it_ptr;
        }
    };
public:
    using iterator               = common_iterator<false>;
    using const_iterator         = common_iterator<true>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Deque();
    Deque(const Deque<T>& other);
    Deque(int n, const T& value = T());
    ~Deque();

    Deque<T>& operator=(const Deque<T>& other);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    T& at(size_t index);
    const T& at(size_t index) const;

    size_t size() const;
    size_t capacity() const;

    void push_back(const T& value = T());
    void pop_back();
    void push_front(const T& value = T());
    void pop_front();

    void insert(iterator iter, const T& value);

    void erase(iterator iter);

    iterator begin();
    iterator end();
    
    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    #ifdef _DEBUG
    void print_arr() const;
    void print_deque() const;
    #endif /* _DEBUG */
};

// Private functions ---------------------------------------------------------------------------/
template <typename T>
std::pair<int, int> Deque<T>::pos_calc(const std::pair<int, int>& pos, size_t offset) {
    size_t begin = pos.first * bucket_size + pos.second;
    size_t val = begin + offset;
    return std::make_pair<int, int>(val / bucket_size, val % bucket_size);
}

template <typename T>
void Deque<T>::pos_forward(std::pair<int, int>& pos) {
    pos.second = pos.second + 1;
    if (pos.second == bucket_size) {
        ++pos.first;
        pos.second = 0;
    }
}

template <typename T>
void Deque<T>::pos_back(std::pair<int, int>& pos) {
    if (pos.second == 0) {
        --pos.first;
        pos.second = bucket_size - 1;
    }
    else pos.second = pos.second - 1;
}

template <typename T>
bool Deque<T>::is_index_in_range(int i, int j) const {
    return ((std::make_pair(i, j) >= begin_pos) && (std::make_pair(i, j) < end_pos));
}

template <typename T>
bool Deque<T>::is_index_in_range(const std::pair<int, int>& val) const {
    return ((val >= begin_pos) && (val < end_pos));
}

template <typename T>
void Deque<T>::expand_back(size_t n) {
    if (n < 2) return;
    std::vector<T*> new_arr(bucket_count * n);
    size_t new_bucket_count = bucket_count * n;
    size_t new_cap = bucket_size * new_bucket_count;
    for (size_t i = 0; i < new_bucket_count; ++i) {
        new_arr[i] = reinterpret_cast<T*>(new int8_t[bucket_size * sizeof(T)]);
        if (i < bucket_count) {
            new_arr[i] = arr[i];
        }
    }
    arr              = new_arr;
    cap              = new_cap;
    bucket_count     = new_bucket_count;
}

template <typename T>
void Deque<T>::expand_front(size_t n) {
    if (n < 2) return;
    std::vector<T*> new_arr(bucket_count * n);
    size_t new_bucket_count = bucket_count * n;
    size_t new_cap = bucket_size * new_bucket_count;
    for (size_t i = 0; i < new_bucket_count; ++i) {
        new_arr[i] = reinterpret_cast<T*>(new int8_t[bucket_size * sizeof(T)]);
        if (i >= bucket_count) {
            new_arr[i] = arr[i - bucket_count];
        }
    }
    arr              = new_arr;
    cap              = new_cap;
    begin_pos.first += bucket_count;
    end_pos.first   += bucket_count;
    bucket_count     = new_bucket_count;
}

// Public functions ----------------------------------------------------------------------------/
template <typename T>
Deque<T>::Deque() : arr(1), bucket_count(1), sz(0), cap(bucket_size), begin_pos{0, bucket_size / 2}, end_pos{0, bucket_size / 2} {
    // arr[0] = new T[bucket_size]; // не подходит, т.к. вызовется конструктор по-умолчанию для T
    arr[0] = reinterpret_cast<T*>(new int8_t[bucket_size * sizeof(T)]); // просто выделили память
}

template <typename T>
Deque<T>::Deque(const Deque<T>& other) : arr(other.bucket_count) {
    size_t i = 0;
    size_t j;
    try {
        for ( ; i < other.bucket_count; ++i) {
            arr[i] = reinterpret_cast<T*>(new int8_t[bucket_size * sizeof(T)]);
            for (j = 0; j < bucket_size; ++j) {
                if(other.is_index_in_range(i, j)) {
                    new(arr[i] + j) T(*(other.arr[i] + j)); // placement new
                }
            }
        }
        bucket_count = other.bucket_count;
        begin_pos    = other.begin_pos;
        end_pos      = other.end_pos;
        sz           = other.sz;
        cap          = other.cap;
    }
    catch (...) {
        for (size_t x = 0; x <= i; ++x) {
            size_t y_end = (x == i) ? j : bucket_size;
            for (size_t y = 0; y < y_end; ++y) {
                (arr[x] + y)->~T(); // явный вызов деструктора по адресу
            }
            delete[] reinterpret_cast<int8_t*>(arr[x]); //освобождаем память 
        }
        throw;
    }
}

template <typename T>
Deque<T>::Deque(int n, const T& value) : sz(n), begin_pos{0, bucket_size / 2} {
    if (n < 0) throw std::bad_alloc();

    size_t first_indx = (bucket_size / 2);
    size_t last_indx  = (n - 1 + first_indx) % bucket_size;
    size_t buckets    = ((n + first_indx) / bucket_size) + (((n + first_indx) % bucket_size) == 0 ? 0 : 1);

    size_t i = 0;
    size_t j; 
    try {
        arr.resize(buckets);
        cap          = buckets * bucket_size;
        sz           = n; 
        bucket_count = buckets;
        end_pos      = pos_calc(begin_pos, sz);

        for ( ; i < buckets; ++i) {
            arr[i] = reinterpret_cast<T*>(new int8_t[bucket_size * sizeof(T)]);
            for (j = 0; j < bucket_size; ++j) {
                if(is_index_in_range(i, j)) {
                    new(arr[i] + j) T(value);  // вызываем копирующий конструктор
                }
            }
        }
    }
    catch(...) {
        for (size_t x = 0; x <= i; ++x) {
            size_t y_end = (x == i) ? j : bucket_size;
            for (size_t y = 0; y < y_end; ++y) {
                (arr[x] + y)->~T(); // явный вызов деструктора по адресу
            }
            delete[] reinterpret_cast<int8_t*>(arr[x]); // освобождаем память
        }
        throw;
    }
}

template <typename T>
Deque<T>::~Deque() {
    for (int i = 0; i < bucket_count; ++i) {
        for (int j = 0; j < bucket_size; ++j) {
            if (is_index_in_range(i, j)) {
                (arr[i] + j)->~T(); // явный вызов деструктора по адресу
            }
        }

        delete[] reinterpret_cast<int8_t*>(arr[i]); // освобождаем память
    }
}

template <typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& other) {
    std::vector<T*> new_arr(other.bucket_count);

    size_t i = 0;
    size_t j;
    try {
        for ( ; i < other.bucket_count; ++i) {
            new_arr[i] = reinterpret_cast<T*>(new int8_t[bucket_size * sizeof(T)]);
            for (j = 0; j < bucket_size; ++j) {
                if(other.is_index_in_range(i, j)) {
                    new(new_arr[i] + j) T(*(other.arr[i] + j)); // placement new
                }
            }
        }
    }
    catch (...) {
        for (size_t x = 0; x <= i; ++x) {
            size_t y_end = (x == i) ? j : bucket_size;
            for (size_t y = 0; y < y_end; ++y) {
                (new_arr[x] + y)->~T(); // явный вызов деструктора по адресу
            }
            delete[] reinterpret_cast<int8_t*>(new_arr[x]); //освобождаем память 
        }
        throw;
    }    

    // если все прошло без исключений, освобождаем старый вектор
    for (size_t x = 0; x < bucket_count; ++x) {
        for (size_t y = 0; y < bucket_size; ++y) {
            if (is_index_in_range(x, y)) {
                (arr[x] + y)->~T(); // явный вызов деструктора по адресу
            }
        }
        delete[] reinterpret_cast<int8_t*>(arr[x]); //освобождаем память 
    }

    // копируем новый вектор
    arr = new_arr;

    // копируем состояние
    bucket_count = other.bucket_count;
    begin_pos    = other.begin_pos;
    end_pos      = other.end_pos;
    sz           = other.sz;
    cap          = other.cap;
}

template <typename T>
size_t Deque<T>::size() const {
    return sz;
}

template <typename T>
size_t Deque<T>::capacity() const {
    return cap;
}

template <typename T>
T& Deque<T>::operator[](size_t index) {
    std::pair<int, int> pos = pos_calc(begin_pos, index);
    return arr[pos.first][pos.second];
}

template <typename T>
const T& Deque<T>::operator[](size_t index) const {
    std::pair<int, int> pos = pos_calc(begin_pos, index);
    return arr[pos.first][pos.second];
}

template <typename T>
T& Deque<T>::at(size_t index) {
    std::pair<int, int> pos = pos_calc(begin_pos, index);
    if (is_index_in_range(pos)) return arr[pos.first][pos.second];
    else throw std::out_of_range("at(): out of range");
}

template <typename T>
const T& Deque<T>::at(size_t index) const {
    std::pair<int, int> pos = pos_calc(begin_pos, index);
    if (is_index_in_range(pos)) return arr[pos.first][pos.second];
    else throw std::out_of_range("at(): out of range");
}

template <typename T>
void Deque<T>::push_back(const T& value){
    if (((end_pos.first * bucket_size + end_pos.second) == cap) || sz == cap) {
        expand_back();
    }
    ++sz;
    new(arr[end_pos.first] + end_pos.second) T(value);
    pos_forward(end_pos);
}

template <typename T>
void Deque<T>::pop_back() {
    if (sz == 0) {
        return;
    } 
    --sz;
    pos_back(end_pos);
    (arr[end_pos.first] + end_pos.second)->~T();
}

template <typename T>
void Deque<T>::push_front(const T& value) {
    if (((begin_pos.first == begin_pos.second) && (begin_pos.first == 0)) || sz == cap) {
        expand_front();
    }
    ++sz;
    pos_back(begin_pos);
    new(arr[begin_pos.first] + begin_pos.second) T(value);
}

template <typename T>
void Deque<T>::pop_front() {
    if (sz == 0) {
        return;
    } 
    --sz;
    (arr[begin_pos.first] + begin_pos.second)->~T();
    pos_forward(begin_pos);
}

template <typename T>
void Deque<T>::insert(iterator iter, const T& value) {
    push_back(*(end() - 1));
    for (iterator it = iter + 1 ; it != this->end(); ++it) {
        *it = *(it - 1);
    }
    *iter = value;
}

template <typename T>
void Deque<T>::erase(iterator iter) {
    iterator it(iter);
    for ( ; it != this->end(); ++it) {
        *it = *(it + 1);
    }
    pop_back();
}

template <typename T>
typename Deque<T>::iterator Deque<T>::begin() {
    return iterator(arr, begin_pos);
}

template <typename T>
typename Deque<T>::iterator Deque<T>::end() {
    return iterator(arr, end_pos);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::begin() const {
    return const_iterator(arr, begin_pos);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::end() const {
    return const_iterator(arr, end_pos);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::cbegin() const {
    return const_iterator(arr, begin_pos);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::cend() const {
    return const_iterator(arr, end_pos);
}

template <typename T>
typename Deque<T>::reverse_iterator Deque<T>::rbegin() {
    return reverse_iterator(iterator(arr, end_pos));
}

template <typename T>
typename Deque<T>::reverse_iterator Deque<T>::rend() {
    return reverse_iterator(iterator(arr, begin_pos));
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rbegin() const {
    return const_reverse_iterator(const_iterator(arr, end_pos));    
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rend() const {
    return const_reverse_iterator(const_iterator(arr, begin_pos));
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crbegin() const {
    return const_reverse_iterator(const_iterator(arr, end_pos)); 
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crend() const {
    return const_reverse_iterator(const_iterator(arr, begin_pos));
}


#ifdef _DEBUG
template <typename T>
void Deque<T>::print_arr() const {
    for (int i = 0; i < bucket_count; ++i) {
        for (int j = 0; j < bucket_size; ++j) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <typename T>
void Deque<T>::print_deque() const {
    size_t count = 0;
    for (int i = 0; i < bucket_count; ++i) {
        for (int j = 0; j < bucket_size; ++j) {
            if (is_index_in_range(i, j)) {
                ++count;
                std::cout << arr[i][j] << " ";
            }
        }
    }
    std::cout << "(" << count << " elements)" << std::endl << std::endl;
}
#endif /* _DEBUG */


#endif /* DEQUE_H */