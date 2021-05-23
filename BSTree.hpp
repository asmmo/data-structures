#include <memory>
#include <iostream>

#ifndef BTREE_BTREE_H
#define BTREE_BTREE_H
namespace details{
    template<class T>
    class BSTree{
        class BSTreeNode {
            friend BSTree;
            T data;
            std::unique_ptr<BSTreeNode> left;
            std::unique_ptr<BSTreeNode> right;
        public:
            explicit BSTreeNode(T const &);
            explicit BSTreeNode(T&&);
        };
        std::unique_ptr<BSTreeNode> root;
        size_t size_{};
        void privateInsert(T &&, std::unique_ptr<BSTreeNode> &);

        void privateInsert(T const &, std::unique_ptr<BSTreeNode>&);
        size_t privateHeight(const std::unique_ptr<BSTreeNode> &) const;
        void privateInorderTraversal(const std::unique_ptr<BSTreeNode> &)const;
        void privatePreorderTraversal(const std::unique_ptr<BSTreeNode> &)const;
        void privatePostorderTraversal(const std::unique_ptr<BSTreeNode> &)const;
        void cpyCtorHelper(std::unique_ptr<BSTreeNode> &,const std::unique_ptr<BSTreeNode> &);
    public:
        BSTree() = default;
        explicit BSTree(T&&);
        explicit BSTree(const T&);
        BSTree(BSTree&&) noexcept;
        BSTree(const BSTree & );
        size_t size();
        void insert(T &&);
        void insert(T const &);
        size_t height() const;
        void inorderTraversal()const;
        void preorderTraversal()const;
        void postorderTraversal()const;
    };
}


template<class T>
details::BSTree<T>::BSTreeNode::BSTreeNode(T const & data):data{data}{}

template<class T>
details::BSTree<T>::BSTreeNode::BSTreeNode(T&& data):data{std::move(data)}{}

template<class T>
void details::BSTree<T>::insert(T && data) {
    privateInsert(std::move(data), this -> root);
}

template<class T>
void details::BSTree<T>::privateInsert(T && data, std::unique_ptr<BSTreeNode> &current ) {
    if(!current) {
        current = std::make_unique<BSTreeNode>(std::move(data));
        ++size_;
    }
    else if (data < current -> data){
        privateInsert(std::move(data), current -> left);
    }
    else{
        privateInsert(std::move(data), current -> right);
    }
}

template<class T>
void details::BSTree<T>::privateInsert(const T & data, std::unique_ptr<BSTreeNode> &current ) {
    if(!current) {
        current = std::make_unique<BSTreeNode>(std::move(data));
        ++size_;
    }
    else if (data < current -> data){
        privateInsert(data, current -> left);
    }
    else{
        privateInsert(data, current -> right);
    }
}

template<class T>
void details::BSTree<T>::insert(T const & data) {
    privateInsert(data, this -> root);
}

template<class T>
details::BSTree<T>::BSTree(const BSTree & rhs):BSTree{} {
        this->cpyCtorHelper(this->root, rhs.root);
        this -> size_ = rhs.size_;
}

template<class T>
void details::BSTree<T>::cpyCtorHelper(std::unique_ptr<BSTreeNode> & lhs, const std::unique_ptr<BSTreeNode> & rhs) {
    if (rhs) {
        lhs = std::make_unique<BSTreeNode>(rhs->data);
        if (rhs->left) {
            this->cpyCtorHelper(lhs->left, rhs->left);
        }
        if (rhs->right) {
            this->cpyCtorHelper(lhs->right, rhs->right);
        }
    }
}

template<class T>
size_t details::BSTree<T>::height() const {
    return privateHeight(root);
}

template<class T>
size_t details::BSTree<T>::privateHeight(const std::unique_ptr<BSTreeNode> &current) const {
    if(current){
        return 1 + std::max(privateHeight(current -> left), privateHeight(current -> right));
    }
    return 0;
}

template<class T>
void details::BSTree<T>::inorderTraversal() const {
    privateInorderTraversal(root);
    std::cout << "\n";
}

template<class T>
void details::BSTree<T>::preorderTraversal() const {
    privatePreorderTraversal(root);
    std::cout << "\n";

}

template<class T>
void details::BSTree<T>::postorderTraversal() const {
    privatePostorderTraversal(root);
    std::cout << "\n";
}

template<class T>
void details::BSTree<T>::privateInorderTraversal(const std::unique_ptr<BSTreeNode> &current) const {
    if (current -> left){
        privateInorderTraversal(current -> left);
    }
    std::cout << current -> data << ", ";
    if (current -> right){
        privateInorderTraversal(current -> right);
    }
}

template<class T>
void details::BSTree<T>::privatePreorderTraversal(const std::unique_ptr<BSTreeNode> &current) const {
    std::cout << current -> data << ", ";
    if (current -> left){
        privatePreorderTraversal(current -> left);
    }
    if (current -> right){
        privatePreorderTraversal(current -> right);
    }
}

template<class T>
void details::BSTree<T>::privatePostorderTraversal(const std::unique_ptr<BSTreeNode> &current) const {
    if (current -> left){
        privatePostorderTraversal(current -> left);
    }
    if (current -> right){
        privatePostorderTraversal(current -> right);
    }
    std::cout << current -> data << ", ";
}

template<class T>
details::BSTree<T>::BSTree(T && data) {
    this ->insert(std::move(data));
}

template<class T>
details::BSTree<T>::BSTree(const T & data) {
    this ->insert(data);
}

template<class T>
details::BSTree<T>::BSTree(BSTree && rhs) noexcept {
    this -> root = std::move(rhs.root);
    this->size_=rhs.size_;
    rhs.size_ = 0;
}

template<class T>
size_t details::BSTree<T>::size() {
    return size_;
}
#endif //BTREE_BTREE_H
