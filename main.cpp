#define _DEBUG

#include "my_deque.h"

struct C {
    int a;
    C(int a) : a(a) {};
    friend std::ostream& operator<<(std::ostream& out, const C& obj) {
        out << obj.a;
        return out;
    }
};

int main() {
    // Deque<int> deq1;
    // deq1.print_deque();
    // deq1.push_back(5);
    // deq1.print_deque();

    // Deque<int> deq2(15, 5);
    // deq2.push_back(7);
    // for (int i = 0; i < 6; ++i) {
    //     deq2.push_front(3);
    // }
    // for (int i = 0; i < 11; ++i) {
    //     deq2.pop_front();
    // }
    // for (int i = 0; i < 11; ++i) {
    //     deq2.pop_back();
    // }
    // deq2.push_front(9);
    // deq2.print_deque();
    // deq2.print_arr();

    // Deque<int> deq3(deq2);
    // deq3.print_deque();
    // deq3.print_arr();

    // Deque<int> deq4;
    // deq4 = deq2;
    // deq4.print_deque();
    // deq4.print_arr();

    Deque<C> deq5(15, C(5));
    deq5.push_back(C(3));
    // for (int i = 0; i < 6; ++i) {
    //     deq5.push_front(C(7));
    // }
    // deq5.push_front(C(9));
    // deq5.print_deque();
    // deq5.print_arr();
    // std::cout << "first elem = " << *deq5.begin() << ", last elem = " << *(deq5.end() - 1) << ", len = " << deq5.end() - deq5.begin() << std::endl; 
    std::cout << "last elem = " << *(deq5.rend() - 1) << ", first elem = " << *deq5.rbegin() << ", len = " << deq5.rend() - deq5.rbegin() << std::endl; 
    // for (auto it = deq5.rbegin(); it != deq5.rend(); ++it) {
    //     std::cout << *it << " ";
    // }
    // deq5.print_deque();
    // Deque<C>::iterator it = deq5.begin() + 5;
    // deq5.insert(it, C(11));
    // deq5.print_deque();
    // deq5.print_arr();
    // deq5.erase(deq5.begin());
    // deq5.print_deque();
    // deq5.print_arr();    
}