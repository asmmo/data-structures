use std::{
    borrow::{Borrow, BorrowMut},
    cell::RefCell,
    cmp::Ordering,
    default::Default,
    fmt::Debug,
    rc::Rc,
};
#[derive(Default)]
enum BST<K: Ord + Debug + Default, T: Debug + Default> {
    #[default]
    Empty,
    Full(BSTNode<K, T>),
}

#[derive(Default)]
struct BSTNodeInner<K: Ord + Debug + Default, T: Debug + Default> {
    key: K,
    value: T,
    l_child: BST<K, T>,
    r_child: BST<K, T>,
}
type BSTNode<K, T> = Rc<RefCell<BSTNodeInner<K, T>>>;
impl<K: Ord + Debug + Default, T: Debug + Default> Debug for BST<K, T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Empty => write!(f, "Empty"),
            Self::Full(node) => f.debug_tuple("Full").field(node).finish(),
        }
    }
}

impl<K: Ord + Debug + Default, T: Debug + Default> Debug for BSTNodeInner<K, T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("BSTNode")
            .field("key", &self.key)
            .field("value", &self.value)
            .field("l_child", &self.l_child)
            .field("r_child", &self.r_child)
            .finish()
    }
}

impl<K: Ord + Debug + Default, T: Debug + Default> BSTNodeInner<K, T> {
    fn is_leaf(&self) -> bool {
        self.borrow().l_child.is_empty() && self.borrow().r_child.is_empty()
    }
}
impl<K: Ord + Debug + Default, T: Debug + Default> BST<K, T> {
    fn new() -> Self {
        Self::Empty
    }

    fn insert(&mut self, key: K, value: T) {
        match self {
            BST::Empty => {
                *self = BST::Full(BSTNode::new(RefCell::new(BSTNodeInner {
                    key,
                    value,
                    l_child: BST::Empty,
                    r_child: BST::Empty,
                })))
            }

            BST::Full(node) => {
                let ord = key.cmp(&node.as_ref().borrow().key);
                match ord {
                    Ordering::Less => node.as_ref().borrow_mut().l_child.insert(key, value),
                    Ordering::Equal => node.as_ref().borrow_mut().r_child.insert(key, value),
                    Ordering::Greater => node.as_ref().borrow_mut().value = value,
                }
            }
        }
    }

    fn search(&self, key: K) -> Option<BSTNode<K, T>> {
        match self {
            BST::Empty => None,
            BST::Full(node) => match key.cmp(&node.as_ref().borrow().key) {
                Ordering::Greater => node.as_ref().borrow().l_child.search(key),
                Ordering::Less => node.as_ref().borrow().r_child.search(key),
                Ordering::Equal => Some(node.clone()),
            },
        }
    }

    fn min(&self) -> Option<BSTNode<K, T>> {
        match self {
            BST::Empty => None,
            BST::Full(node) => match node.as_ref().borrow().l_child {
                BST::Empty => Some(node.clone()),
                BST::Full(_) => node.as_ref().borrow().l_child.min(),
            },
        }
    }

    fn max(&self) -> Option<BSTNode<K, T>> {
        match self {
            BST::Empty => None,
            BST::Full(node) => match node.as_ref().borrow().r_child {
                BST::Empty => Some(node.clone()),
                BST::Full(_) => node.as_ref().borrow().r_child.max(),
            },
        }
    }

    fn inorder_traverse(&self) {
        match self {
            BST::Empty => (),
            BST::Full(node) => {
                node.as_ref().borrow().l_child.borrow().inorder_traverse();
                print!(
                    "{{{:?}, {:?}}}, ",
                    node.as_ref().borrow().key,
                    node.as_ref().borrow().value
                );
                node.as_ref().borrow().r_child.borrow().inorder_traverse();
            }
        }
    }

    fn is_empty(&self) -> bool {
        match self {
            BST::Empty => true,
            BST::Full(_) => false,
        }
    }

    fn remove(&mut self, key: K) -> Option<(K, T)> {
        match self {
            BST::Empty => None,
            BST::Full(node) => {
                if key < node.as_ref().borrow().key && !node.as_ref().borrow().l_child.is_empty() {
                    node.as_ref().borrow_mut().l_child.remove(key)
                } else if key > node.as_ref().borrow().key
                    && !node.as_ref().borrow().r_child.is_empty()
                {
                    node.as_ref().borrow_mut().r_child.remove(key)
                } else {
                    let res_bst;
                    if node.as_ref().borrow().is_leaf() {
                        res_bst = std::mem::replace(self, BST::Empty);
                    } else if node.as_ref().borrow().l_child.is_empty() {
                        let mut r_child = node.as_ref().borrow().r_child.clone();
                        std::mem::swap(self, &mut r_child);
                        res_bst = r_child;
                    } else if node.as_ref().borrow().r_child.is_empty() {
                        let mut l_child = node.as_ref().borrow().l_child.clone();
                        std::mem::swap(self, &mut l_child);
                        res_bst = l_child;
                    } else {
                        let right_tree_min_child = node.as_ref().borrow().r_child.min().unwrap();
                        std::mem::swap(
                            &mut right_tree_min_child.as_ref().borrow_mut().key,
                            &mut node.as_ref().borrow_mut().key,
                        );
                        std::mem::swap(
                            &mut right_tree_min_child.as_ref().borrow_mut().value,
                            &mut node.as_ref().borrow_mut().value,
                        );

                        return node.as_ref().borrow_mut().r_child.remove(key);
                    }
                    match res_bst {
                        BST::Empty => None,
                        BST::Full(mut res_node) => {
                            let BSTNodeInner { key, value, .. } = res_node.borrow_mut().take();
                            Some((key, value))
                        }
                    }
                }
            }
        }
    }
}

impl<K: Ord + Debug + Default, T: Debug + Default> Clone for BST<K, T> {
    fn clone(&self) -> Self {
        match self {
            Self::Empty => Self::Empty,
            Self::Full(node) => Self::Full(node.clone()),
        }
    }
}
#[test]
fn test() {
    let mut bst = BST::<i32, String>::new();
    println!("bst.min(): {:?}", bst.min());
    println!("bst.max(): {:?}", bst.max());
    bst.insert(1, "one".to_string());
    bst.insert(3, "three".to_string());
    bst.insert(2, "two".to_string());
    bst.insert(-200, "negative two hundreds".to_string());
    bst.insert(-300, "negative three hundreds".to_string());
    bst.insert(-400, "negative four hundreds".to_string());
    bst.inorder_traverse();
    println!();
    println!("bst.search(3): {:?}", bst.search(3));
    println!("bst.search(30): {:?}", bst.search(30));
    println!("bst.search(-400): {:?}", bst.search(-400));
    println!("bst.min(): {:?}", bst.min());
    println!("bst.max(): {:?}", bst.max());
    let bst2 = bst.clone();
    assert_eq!(format!("{:?}", bst), format!("{:?}", bst2));
    println!("BST Before remove 3: {:?}", bst);
    bst.remove(3);
    println!("BST After remove 3: {:?}", bst);
    println!("BST Before remove 1: {:?}", bst);
    bst.remove(1);
    println!("BST After remove 1: {:?}", bst);
}
