#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include "print.hpp"

using namespace std;

void f1() {}

void f2() {}

template<class T, size_t N>
void t(const T (&arr)[N]) {
    unip::print(unip::details::is_support_iterator_v<decltype(arr)>);
    unip::print("============");
    for (auto i: arr) {
        unip::print(i);
    }
}


void test_base_type() {
    std::vector<int> v{1, 4, 5, 2, 23, 5, 24};
    std::vector<std::string> v1{"111", "324234", "abc"};
    std::vector<bool> v2{false, true};
    std::list<bool> l1{false, true};
    std::list<double> l2{1.34, 434.23};
    std::map<int, int> m{{1, 2},
                         {4, 5},
                         {2, 5}};
    std::unordered_map<int, int> m1{{1, 2},
                                    {4, 5},
                                    {2, 5}};
    unip::print(v1);
    unip::print(l2);
    unip::print(m);
    unip::print(m1);

    int arr[10]{1, 2, 35, 1111};
//    t(arr);
    unip::print(arr);

    auto &p1 = f1;
    auto &p2 = f2;
    vector<void (*)()> v3{p1, f2};
    unip::print(v3);
}

int main() {
//    test_base_type();
    return 0;
}
