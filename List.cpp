#include <iostream>
#include <vector>


template <typename T>
class List{
    size_t _size{};
    struct Node {
        T data { };
        Node* prev{};
        Node* next{};

        Node ( ) = default;

        Node ( const T& data, Node* prev = nullptr, Node* next = nullptr):
        prev{ prev }, next{next}, data { data }{}

        Node ( T&& data, Node* prev = nullptr, Node* next = nullptr):
                prev{ prev }, next{next}, data { std::move( data ) }{}
    };
    Node* head{};
    Node* tail{};

public:

    class const_iterator{
    protected:
        Node* nodePtr{};
        const_iterator(Node* nodePtr): nodePtr{nodePtr}{}
    public:
        typedef T value_type;
        const_iterator() = default;

        const T& operator* () const {
            return nodePtr -> data;
        }

        const_iterator& operator++(){
            nodePtr = nodePtr -> next;
            return *this;
        }

        const_iterator& operator--(){
            nodePtr = nodePtr -> prev;
            return *this;
        }

        const_iterator operator++(int){

            const_iterator old { *this };
            ++ *this;
            return old;
        }

        const_iterator operator--(int){

            const_iterator old { *this };
            -- *this;
            return old;
        }


        bool operator== ( const const_iterator & rhs ) const {
            return nodePtr == rhs.nodePtr;
        }

        bool operator!= ( const const_iterator & rhs ) const {
            return !( *this == rhs );
        }

        friend List<T>;

    };


    class iterator : public const_iterator{
    public:
        using const_iterator::const_iterator;

        iterator() = default;
        T& operator* () {
            return this -> nodePtr -> data;
        }

        iterator& operator++() {
            this -> nodePtr = this -> nodePtr -> next;
            return *this;
        }

        iterator& operator--(){
            this -> nodePtr = this -> nodePtr -> prev;
            return *this;
        }

        iterator operator++(int){

            iterator old { *this };
            ++ *this;
            return old;
        }

        iterator operator--(int){

            iterator old { *this };
            -- *this;
            return old;
        }

    };

    //constructors
    List() {
        head = new Node{};
        tail = new Node{};
        head -> next = tail;
        tail -> prev = head;
    };

    List(size_t n, const T & value = T{}): head{ new Node }{

        Node* current = head;

        for(size_t i {}; i < n; ++i){
            Node* temp = current;
            current = current -> next = new Node{ value, current};
        }
        current -> next = tail = new Node{T{}, current};
    }

    List(const List & rhs) : List() {

        for (auto itr{rhs.cbegin()}; itr != rhs.cend(); ++itr)
            push_back(*itr);
    }

    List& operator= (const List & rhs){

        if(this == &rhs) return *this;

        List copy { rhs };
        std::swap(head, copy.head);
        std::swap(tail, copy.tail);
        std::swap(_size, copy._size);
        return *this;

    }

    List(List && rhs):List(){

        std::swap(head, rhs.head);
        std::swap(tail, rhs.tail);
        std::swap(_size, rhs._size);
    }

    List& operator= (List && rhs){

        List copy{ std::move( rhs ) };
        std::swap(head, copy.head);
        std::swap(tail, copy.tail);
        std::swap(_size, copy._size);
        return *this;
    }


    List( std::initializer_list< T > & list): head{new Node}{
        Node* current = head;

        for(auto it = std::begin(list); it != list.end(); ++it){
            T val;
            current = current -> next = new Node{ std::move(val = *it), current};
        }
        current -> next = tail = new Node{T{}, current};
    }


    List( std::initializer_list< T > && list): head{new Node}{
        Node* current = head;

        for(auto it = std::begin(list); it != list.end(); ++it){
            T val;
            current = current -> next = new Node{ std::move(val = *it), current};
        }
        current -> next = tail = new Node{T{}, current};
    }


    //destructor
    ~List( ){

        Node* current = head;

        while(current -> next){
            current = current -> next;
            delete current -> prev;
        }
        delete tail;
    }


    //mutators
    // Insert x before itr.
    iterator insert( iterator itr, const T & data ){

        Node *newNodePtr = itr.nodePtr;
        _size++;
        return { newNodePtr -> prev = newNodePtr -> prev -> next =
            new Node{ data, newNodePtr -> prev, newNodePtr } };
    }

    // Insert x before itr.
    iterator insert( iterator itr, T && data )
    {
        Node *newNodePtr = itr.nodePtr;
        _size++;
        return { newNodePtr -> prev = newNodePtr -> prev -> next
                = new Node{ std::move( data ), newNodePtr -> prev, newNodePtr } };
    }


    void erase(const iterator& itr){

        Node *toBeReturnedNodePtr = itr.nodePtr -> next;
        Node *toBeDeletedNodePtr = itr.nodePtr;
        toBeDeletedNodePtr -> prev -> next = toBeDeletedNodePtr -> next;
        toBeDeletedNodePtr -> next -> prev = toBeDeletedNodePtr -> prev;
        delete toBeReturnedNodePtr;
        --_size;

    }

    iterator erase( const iterator & from, const iterator & to )
    {
        for( iterator itr = from; itr != to; )
            itr = erase( itr );
        return to;
    }

    void push_front( const T& val ){
        insert(begin(), val);
    }
    void push_front( T&& val ){
        insert(begin(), std::move( val ));
    }


    void push_back( const T& val ){
        insert(end(), val);
    }
    void push_back( T && val ){
        insert(end(), std::move( val ));
    }


    void pop_front( ){
        erase( begin());
    }
    void pop_back( ){
        erase(end());
    }


    T& front( ){
        return *(this -> begin());
    }
    T& back( ){
        return *--(this -> end());
    }


    iterator begin(){
        return head -> next;
    }

    iterator end(){
        return tail;
    }


    const T& front( ) const {
        return *(this -> begin());
    }
    const T& back( ) const {
        return *--(this -> end());
    }



    const_iterator begin() const {
        return head -> next;
    }

    const_iterator cbegin() const {
        return {head -> next};
    }

    const_iterator end() const {
        return tail;
    }

    const_iterator cend() const {
        return tail;
    }



    //utilities
    size_t size() const {
        return size;
    }

    bool empty() const {
        return size;
    }

};

int main(){
    int j = 9;
    List<int> ls = {1, 7 , 8};
    ls.push_back(7);
    ls.push_front(j);
    ls.front() = 90;
    ls.back() = 70;
    List ls1 =ls;

    List<int> ls2= {4,5};
    ls2={std::move(ls1)};
    for(auto itr = ls.cbegin(); itr!= ls.cend(); ++itr)
        std::cout << *itr << ", ";
    std::cout << "\n\n";
    for(auto itr = ls1.cbegin(); itr!= ls1.cend(); ++itr)
        std::cout << *itr << ", ";
    std::cout << "\n\n";
    for(auto itr = ls2.cbegin(); itr!= ls2.cend(); ++itr)
        std::cout << *itr << ", ";


}
