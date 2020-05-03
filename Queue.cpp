#include <iostream>

template<typename T>
class Queue{
    struct Node{
        T data{};
        Node * next{};

        Node(const T & data, Node * next = nullptr ) : data{data}, next { next }{

        }
    };

    Node * head {};
    Node* queueEnd{};
    size_t _size{};

public:
    Queue() = default;
    Queue(const Queue & rhs) = delete ;
    void enqueue(const T & data ){
        if(!head){
            head = new Node { data };
            queueEnd = head;
        }
        else
            queueEnd = queueEnd -> next = new Node { data };
        ++_size;
    }

    void dequeue(){
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

    size_t size(){
        return _size;
    }

    ~Queue(){
        while( _size ) dequeue();
    }

};

int main() {
    Queue<int> q;
    q.enqueue(8);
    q.enqueue(3);
    q.enqueue(2);
    q.top() = 9;
    std::cout << q.top() << "\n";
    q.dequeue();
    std::cout << q.top() << "\n";
    q.dequeue();
    std::cout << q.top() << "\n";
    q.dequeue();
    std::cout << q.size() << "\n";
}
