#include <iostream>
#include <vector>

using namespace std;
template <typename T, typename U>
class Assoc {
    template <typename P>
    struct RBTree {
        enum {RED, BLACK};
        template <typename Q>
        struct RBNode {
            struct RBNode<Q> *parent;
            struct RBNode<Q> *left;
            struct RBNode<Q> *right;
            Q *key;
            unsigned int color;
            RBNode():parent(NULL), left(NULL), right(NULL), key(NULL) {
            };
        };
        struct RBNode<P> *root;  // root of rb tree
        size_t count; // number of nodes in the tree
        struct RBNode<P> *currNode; // current node

        RBTree() {
            root = NULL; count = 0;
        }
        void rotate_left(RBNode<P> *node) {
            RBNode<P> *right = node->right;  // get the node's right child
            node->right = right->left; // make the node's right child' left 
                                      // child as the node's right child
            // If the left child of node's right child is not NULL
            // make the node as its parent node
            if(right->left != NULL) {
                right->left->parent = node;
            }
            // make right child's parent as node's parent
            right->parent = node->parent;
            if(node->parent != NULL) {
                // if node is the left child, then node's right child
                // is now parent's left child
                if(node == node->parent->left) {
                    node->parent->left = right;
                }
                else {
                    // if node is the right child, then node's right child
                    // is now parent's right child
                    node->parent->right = right;
                }
            }
            else {
                root = right; // if node was the root node, make rt
                              // child the root node
            }
            // node is now left child of its right child
            right->left = node;
            node->parent = right;
        }
        void rotate_right(RBNode<P> *node) {
            RBNode<P> *left = node->left;  // get the node's left child
            // set right child of node's left child as its left child
            node->left = left->right;
            if (left->right != NULL)
                // node is the new parent of node's left child's right child
                left->right->parent = node;
                // parent of left child is node's parent
            left->parent = node->parent;

            if (node->parent != NULL) {
                // if node is the right child, then node's left child
                // is now its parent's right child
                if (node == node->parent->right) {
                    node->parent->right = left;
                } 
                else {
                    // if node is the left child, then node's left child
                    // is now its parent's left child
                    node->parent->left = left;
                }
            } 
            else {
                // if node was the root node, make its left
                // child the root node
                root = left;
            }
            // node is now right child of its left child
            left->right = node;
            node->parent = left;
        }
        void insert_fixup(RBNode<P> *node)
        {
            RBNode<P> *uncle;

            /* While not at the root and need fixing... */
            while (node != root && node->parent->color == RED) {
                /* If our parent is left child of our grandparent... */
                if (node->parent == node->parent->parent->left) {
                    uncle = node->parent->parent->right;

                    /* If our uncle is RED... */
                    if (uncle->color == RED) {
                        /* Paint the parent and the uncle BLACK... */
                        node->parent->color = BLACK;
                        uncle->color = BLACK;

                        /* And the grandparent RED... */
                        node->parent->parent->color = RED;

                        /* And continue fixing the grandparent */
                        node = node->parent->parent;
                    } 
                    else {    /* Our uncle is BLACK... */
                        /* Are we the right child? */
                        if (node == node->parent->right) {
                            node = node->parent;
                            rotate_left(node);
                        }
                        /* Now we're the left child, repaint and rotate... */
                        node->parent->color = BLACK;
                        node->parent->parent->color = RED;
                        rotate_right(node->parent->parent);
                    }
                } 
                else {
                    /* our parent is right child of our grandparent... */
                    uncle = node->parent->parent->left;

                    /* If our uncle is RED... */
                    if (uncle->color == RED) {
                        /* Paint the parent and the uncle BLACK... */
                        node->parent->color = BLACK;
                        uncle->color = BLACK;

                        /* And the grandparent RED... */
                        node->parent->parent->color = RED;

                        /* And continue fixing the grandparent */
                        node = node->parent->parent;
                    } 
                    else {                /* Our uncle is BLACK... */
                        /* Are we the left child? */
                        if (node == node->parent->left) {
                            node = node->parent;
                            rotate_right(node);
                        }
                        /* Now we're the right child, repaint and rotate... */
                        node->parent->color = BLACK;
                        node->parent->parent->color = RED;
                        rotate_left(node->parent->parent);
                    }
                }
            }
            root->color = BLACK;
        }
        RBNode<P> *
        insert (RBNode<P> *data)
        {
            /* XXX Not necessary, but keeps compiler quiet... */
            int r = 0;

            /* We start at the root of the tree */
            RBNode<P>    *node = root;
            RBNode<P>    *parent = NULL;

            /* Lets find the new parent... */
            while (node != NULL) {
                /* Compare two keys, do we have a duplicate? */
                if (data->key == node->key) {
                    return NULL;
                }
                parent = node;

                if (data->key < node->key) {
                    node = node->left;
                } else {
                    node = node->right;
                }
            }

            /* Initialize the new node */
            data->parent = parent;
            data->left = data->right = NULL;
            data->color = RED;
            count++;

            /* Insert it into the tree... */
            if (parent != NULL) {
                if (data->key < node->key) {
                    parent->left = data;
                } else {
                    parent->right = data;
                }
            } else {
                root = data;
            }

            /* Fix up the red-black properties... */
            insert_fixup(data);

            return data;
        }
        /*
         * Searches the red black tree, returns the data if key is found or NULL otherwise.
         *
         */
        RBNode<P> *
        search (const void *key)
        {
            RBNode<P> *node;

            if (find_less_equal(key, &node)) {
                return node;
            } else {
                return NULL;
            }
        }

        /* helpers for delete */
        void swap_int8(unsigned char* x, unsigned char* y)
        {
            unsigned char t = *x; *x = *y; *y = t;
        }

        void swap_np(RBNode<P>** x, RBNode<P>** y)
        {
            RBNode<P>* t = *x; *x = *y; *y = t;
        }

        void change_parent_ptr(RBNode<P>* parent, RBNode<P>* old, RBNode<P>* newNode)
        {
            if(parent == NULL)
            {
                assert(root == old);
                if(root == old) root = newNode;
                return;
            }
            assert(parent->left == old || parent->right == old
                || parent->left == newNode || parent->right == newNode);
            if(parent->left == old) parent->left = newNode;
            if(parent->right == old) parent->right = newNode;
        }
        void change_child_ptr(RBNode<P>* child, RBNode<P>* old, 
             RBNode<P>* newNode)
        {
            if(child == NULL) return;
            assert(child->parent == old || child->parent == newNode);
            if(child->parent == old) child->parent = newNode;
        }

        RBNode<P> * deleteNode(const void *key)
        {
            RBNode<P> *to_delete;
            RBNode<P> *child;
            if((to_delete = search(key)) == 0) return 0;
            count--;

            /* make sure we have at most one non-leaf child */
            if(to_delete->left != NULL && to_delete->right != NULL)
            {
                /* swap with smallest from right subtree (or largest from left) */
                RBNode<P> *smright = to_delete->right;
                while(smright->left != NULL)
                    smright = smright->left;
                /* swap the smright and to_delete elements in the tree,
                 * but the RBNode<P> is first part of user data struct
                 * so cannot just swap the keys and data pointers. Instead
                 * readjust the pointers left,right,parent */

                /* swap colors - colors are tied to the position in the tree */
                swap_int8(&to_delete->color, &smright->color);

                /* swap child pointers in parents of smright/to_delete */
                change_parent_ptr(to_delete->parent, to_delete, smright);
                if(to_delete->right != smright)
                    change_parent_ptr(smright->parent, smright, to_delete);

                /* swap parent pointers in children of smright/to_delete */
                change_child_ptr(smright->left, smright, to_delete);
                change_child_ptr(smright->left, smright, to_delete);
                change_child_ptr(smright->right, smright, to_delete);
                change_child_ptr(smright->right, smright, to_delete);
                change_child_ptr(to_delete->left, to_delete, smright);
                if(to_delete->right != smright)
                    change_child_ptr(to_delete->right, to_delete, smright);
                if(to_delete->right == smright)
                {
                    /* set up so after swap they work */
                    to_delete->right = to_delete;
                    smright->parent = smright;
                }

                /* swap pointers in to_delete/smright nodes */
                swap_np(&to_delete->parent, &smright->parent);
                swap_np(&to_delete->left, &smright->left);
                swap_np(&to_delete->right, &smright->right);
        
                /* now delete to_delete (which is at the location where the smright previously was) */
            }
            assert(to_delete->left == NULL || to_delete->right == NULL);

            if(to_delete->left != NULL) child = to_delete->left;
            else child = to_delete->right;

            /* unlink to_delete from the tree, replace to_delete with child */
            change_parent_ptr(to_delete->parent, to_delete, child);
            change_child_ptr(child, to_delete, to_delete->parent);

            if(to_delete->color == RED)
            {
                /* if node is red then the child (black) can be swapped in */
            }
            else if(child->color == RED)
            {
                /* change child to BLACK, removing a RED node is no problem */
                if(child!=NULL) child->color = BLACK;
            }
            else delete_fixup(child, to_delete->parent);
        
            /* unlink completely */
            to_delete->parent = NULL;
            to_delete->left = NULL;
            to_delete->right = NULL;
            to_delete->color = BLACK;
            return to_delete;
        }

        void delete_fixup(RBNode<P>* child, RBNode<P>* child_parent)
        {
            RBNode<P>* sibling;
            int go_up = 1;
        
            /* determine sibling to the node that is one-black short */
            if(child_parent->right == child) sibling = child_parent->left;
            else sibling = child_parent->right;

            while(go_up)
            {
                if(child_parent == NULL)
                {
                    /* removed parent==black from root, every path, so ok */
                    return;
                }
        
                if(sibling->color == RED)
                {   /* rotate to get a black sibling */
                    child_parent->color = RED;
                    sibling->color = BLACK;
                    if(child_parent->right == child)
                        rotate_right(child_parent);
                    else    rotate_left(child_parent);
                    /* new sibling after rotation */
                    if(child_parent->right == child) sibling = child_parent->left;
                    else sibling = child_parent->right;
                }

                if(child_parent->color == BLACK
                    && sibling->color == BLACK
                    && sibling->left->color == BLACK
                    && sibling->right->color == BLACK)
                {   /* fixup local with recolor of sibling */
                    if(sibling != NULL)
                        sibling->color = RED;
        
                    child = child_parent;
                    child_parent = child_parent->parent;
                    /* prepare to go up, new sibling */
                    if(child_parent->right == child) sibling = child_parent->left;
                    else sibling = child_parent->right;
                }
                else go_up = 0;
            }
        
            if(child_parent->color == RED
                && sibling->color == BLACK
                && sibling->left->color == BLACK
                && sibling->right->color == BLACK)
            {
                /* move red to sibling to rebalance */
                if(sibling != NULL)
                    sibling->color = RED;
                child_parent->color = BLACK;
                return;
            }
            assert(sibling != NULL);
        
            /* get a new sibling, by rotating at sibling. See which child
               of sibling is red */
            if(child_parent->right == child
                && sibling->color == BLACK
                && sibling->right->color == RED
                && sibling->left->color == BLACK)
            {
                sibling->color = RED;
                sibling->right->color = BLACK;
                rotate_left(sibling);
                /* new sibling after rotation */
                if(child_parent->right == child) sibling = child_parent->left;
                else sibling = child_parent->right;
            }
            else if(child_parent->left == child
                && sibling->color == BLACK
                && sibling->left->color == RED
                && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                sibling->left->color = BLACK;
                rotate_right(sibling);
                /* new sibling after rotation */
                if(child_parent->right == child) sibling = child_parent->left;
                else sibling = child_parent->right;
            }
        
            /* now we have a black sibling with a red child. rotate and exchange colors. */
            sibling->color = child_parent->color;
            child_parent->color = BLACK;
            if(child_parent->right == child)
            {
                assert(sibling->left->color == RED);
                sibling->left->color = BLACK;
                rotate_right(child_parent);
            }
            else
            {
                assert(sibling->right->color == RED);
                sibling->right->color = BLACK;
                rotate_left(child_parent);
            }
        }

        int
        find_less_equal(const void *key, RBNode<P> **result)
        {
            int r;
            RBNode<P> *node;
        
            assert(result);
        
            /* We start at root... */
            node = root;
        
            *result = NULL;
        
            /* While there are children... */
            while (node != NULL) {
                r = cmp(key, node->key);
                if (key == node->key) {
                    /* Exact match */
                    *result = node;
                    return 1;
                }
                if (key < node->key) {
                    node = node->left;
                } else {
                    /* Temporary match */
                    *result = node;
                    node = node->right;
                }
            }
            return 0;
        }
        
        /*
         * Finds the first element in the red black tree
         *
         */
        RBNode<P> *
        first ()
        {
            RBNode<P> *node;
        
            for (node = root; node->left != NULL; node = node->left);
            return node;
        }
        
        RBNode<P> *
        last ()
        {
            RBNode<P> *node;
        
            for (node = root; node->right != NULL; node = node->right);
            return node;
        }
        
        /*
         * Returns the next node...
         *
         */
        RBNode<P> *
        next (RBNode<P> *node)
        {
            RBNode<P> *parent;
        
            if (node->right != NULL) {
                /* One right, then keep on going left... */
                for (node = node->right; node->left != NULL; node = node->left);
            } else {
                parent = node->parent;
                while (parent != NULL && node == parent->right) {
                    node = parent;
                    parent = parent->parent;
                }
                node = parent;
            }
            return node;
        }
        
        RBNode<P> *
        previous(RBNode<P> *node)
        {
            RBNode<P> *parent;
        
            if (node->left != NULL) {
                /* One left, then keep on going right... */
                for (node = node->left; node->right != NULL; node = node->right);
            } else {
                parent = node->parent;
                while (parent != NULL && node == parent->left) {
                    node = parent;
                    parent = parent->parent;
                }
                node = parent;
            }
            return node;
        }


        /**
         * Given an rbtree "root" node, find the first node under that tree in
         * postorder.
         */
        RBNode<P> *
        postorder_first(RBNode<P> *root)
        {
            RBNode<P> *node = root;
            do {
                while (node->left != NULL) {
                    node = node->left;
                }
                while ((node->left == NULL) &&
                       (node->right != NULL)) {
                    node = node->right;
                }
            } while (node->left != node->right);
            return node;
        }


        /**
         * Given any node in an rbtree, find the next node in postorder.
         */
        RBNode<P> *
        postorder_next(RBNode<P> *node)
        {
            if ((node->parent->right == NULL) ||
                (node->parent->right == node))
                node = node->parent;
            else
                node = postorder_first(node->parent->right);
            return node;
        }
        
    };
    template<typename A, typename B>
    struct Pair {
        A key;
        B val;
        Pair(A &k, B &u): key(k), val(u) {}
        Pair(const A &k, const B &u): key(k), val(u) {}
    };
    vector<Pair<T, U> > vec;
    Assoc(const Assoc &) {} ;  // copy constructor
    Assoc & operator=(const Assoc &) {}; // assignment operator
    public:
        Assoc() {}
        const U& operator[] (const T &t)
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(t == p->key) return p->val;
                return vec.back().val;
            }
        }
        void add(const T &t, const U & val)
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(const_cast<T &>(t) == p->key) return;
            }
            vec.push_back(Pair<T, U> (t, val));
        }
        void erase(const T &t)
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(const_cast<T &>(t) == p->key) {
                    vec.erase(p);
                    return;
                }
            }
        }
        U & find(const T & key) 
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(const_cast<T &>(key) == p->key) {
                    return p->val;
                }
            }
            return vec.back().val;
        }
        void print_all() const
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                //std::cout << p->name << ": " << p->val << std::endl;
            }
        }
    };

template<typename X, typename Y>
class Map {
    Assoc<X, Y> vec;
public:
    Map() {};
    void insert(const X &t, const Y &u)
    {
        vec.add(t, u);
    }
    void erase(const X & key)
    {
        vec.erase(key);
    }
    Y & find(const X & t) 
    {
        return vec.find(t);
    }
};

class A {
public:
    string s;
    A() {};
    A(string &str): s(str) {};
    A(const char *str): s(string(str)) {};
    operator string() { return s; }
    bool operator== (A &a) { if(a.s == s)  return true; return false;}
};

class B {
public:
    double val;
    B() {};
    B(double &v): val(v) {};
    B(double &&v): val(std::forward<double> (v)) {};
    operator double() { return val; };
    bool operator== (B &b) { if(b.val == val)  return true; return false;}
};

int main()
{
    A a("hello");
    B b(100.001);
    Map<A, B> m;
    m.insert(a, b);
    cout << m.find(a) << endl;
    m.erase(a);
    A a1("hello world");
    m.insert(a1, b);
    m.insert("world", b);
    cout << m.find(a1) << endl;
    cout << m.find("world") << endl;
}
