#include <iostream>

#include <iostream>

template<typename T>
class Stack{
    struct Node{
        T data{};
        Node * next{};
        Node(const T & data, Node * next = nullptr ) : data{data}, next { next }{

        }
    };

    Node * head {};
    size_t _size{};

public:
    Stack() = default;
    Stack(const Stack & rhs) = delete ;
    void push(T data ){
        head = new Node { data, head};
        ++_size;
    }

    void pop(){
        if(_size) {
            Node * ptr = head;
            head = head -> next;
            delete ptr;
            --_size;
        }
    }

    T& top(){
        return head -> data;
    }
        
    const T& top() const {
        return head -> data;
    }
    
    size_t size() const {
        return _size;
    }
    
    bool empty( ) const {
        return _size == 0;
    }

    ~Stack(){
        while( _size ) pop();
    }

};

int main() {
    Stack<int> s;
    s.push(8);
    s.push(3);
    s.push(2);
    s.top() = 9;
    std::cout << s.top() << "\n";
    s.pop();
    std::cout << s.top() << "\n";
    s.pop();
    std::cout << s.top() << "\n";
    s.pop();
    std::cout << s.size() << "\n";
}
