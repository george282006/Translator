
#ifndef STACK_H
#define STACK_H
#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T, template<typename...> class TContainer = std::vector>
class Stack {
    TContainer<T> data;
public:
    void push(const T& el) {
        data.push_back(el);
    }
    T pop() {
        if (data.empty())
            throw std::out_of_range("empty stack");
        T lastEl = data.back();
        data.pop_back();
        return lastEl;
    }
    T& top() {
        if (data.empty())
            throw std::out_of_range("empty stack");
        return data.back();
    }
    const T& top() const {
        if (data.empty())
            throw std::out_of_range("empty stack");
        return data.back();
    }
    size_t size() const {
        return data.size();
    }
    bool empty() const { return data.empty(); }
};



#endif //STACK_H
