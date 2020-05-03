#include <iostream>

template<typename T>
class Stack{
    struct Node{
        Node * next{};
        T data{};

        Node(T data, Node * next = nullptr ) : data{data}, next { next }{

        }
    };

    Node * head {};
    size_t _size{};

public:
    Stack() = default;
    Stack(const Stack & rhs) = delete ;
    void push_front(T data ){
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

    T top(){
        return head -> data;
    }

    ~Stack(){
        while( _size ) pop();
    }

};
