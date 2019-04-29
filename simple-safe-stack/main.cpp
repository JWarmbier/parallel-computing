#include <iostream>
#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct emptyStack : std::exception{
    const char* what() const throw(){
        return "Stack is empty.\n";
    }
};

template <typename T> class threadsafeStack{
private:
    std::stack <T> data;
    mutable std::mutex m;
public:
    threadsafeStack(){}
    threadsafeStack(const threadsafeStack& other){
        std::lock_guard<std::mutex> lock(other.m);  // thanks to mutable specifer it's possible to do it
        data = other.data;                          // copy stack from "other" object
    }

    threadsafeStack& operator=(const threadsafeStack&) = delete; //delete assign operator

    void push(T newValue){
        std::lock_guard<std::mutex> lock(m);
        data.push(newValue);
    }

    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw emptyStack();
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }
    void pop(T& value){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw emptyStack();
        value=data.top();
        data.pop();
    }
    bool empty() const{
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};
int main() {
    threadsafeStack<int> tmp;
    try{
        tmp.pop();
    }
    catch (const std::exception& obj){
        std::cout << obj.what() << std::endl;
    }

    return 0;
}