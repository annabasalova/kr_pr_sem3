#ifndef sett.h

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

enum clr { Red, Black };

template<typename t>
struct node {
    t key = 0;
    clr color;
    node<t>* left = nullptr;
    node<t>* right = nullptr;
    node<t>* parent = nullptr;
    node();
    node(t a, clr b, node* l, node* r, node* p) {
        key = a;
        color = b;
        left = l;
        right = r;
        parent = p;
    }
};

template<typename T>
class sett {
public:
    sett();
    ~sett();

    void insert(T key);
    void del(T key);
    void print();
    node<T>* search(T key);
    vector<pair<pair<int, int>, pair<int, int>>> draw1();

private:
    void leftrotate(node<T>*& root, node<T>* x);
    void rightrotate(node<T>*& root, node<T>* y);

    void insert(node<T>*& root, node<T>* nod);
    void fixinsert(node<T>*& root, node<T>* nod);
    void dest(node<T>*& nod);
    node<T>* search(node<T>* nod, T key) const;

    void del(node<T>*& root, node<T>* nod);
    void fixdel(node<T>*& root, node<T>* nod, node<T>* parent);

    void print(node<T>* nod)const;
    void preOrder(node<T>* tree)const;
    void inOrder(node<T>* tree)const;
    void postOrder(node<T>* tree)const;

private:
    node<T>* root;

};

template<class T>
sett<T>::sett() {
    root = nullptr;
}

template<class T>
sett<T>::~sett() {
    dest(root);
}

template<class T>
void sett<T>::leftrotate(node<T>*& root, node<T>* x) {
    node<T>* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == NULL)
        root = y;
    else {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

template<class T>
void sett<T>::rightrotate(node<T>*& root, node<T>* y) {
    node<T>* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == NULL)
        root = x;
    else {
        if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

template<class T>
void sett<T>::insert(T key) {
    node<T>* z = new node<T>(key, Red, nullptr, nullptr, nullptr);
    insert(root, z);
}

template<class T>
void sett<T>::insert(node<T>*& root, node<T>* nod) {
    node<T>* x = root;
    node<T>* y = NULL;
    while (x != NULL)
    {
        y = x;
        if (nod->key > x->key)
            x = x->right;
        else
            x = x->left;
    }
    nod->parent = y;
    if (y != NULL)
    {
        if (nod->key > y->key)
            y->right = nod;
        else
            y->left = nod;
    }
    else
        root = nod;
    nod->color = Red;
    fixinsert(root, nod);
}

template<class T>
void sett<T>::fixinsert(node<T>*& root, node<T>* nod) {
    node<T>* parent;
    parent = nod->parent;
    while (nod != sett<T>::root && parent->color == Red) {
        node<T>* gparent = parent->parent;
        if (gparent->left == parent)
        {
            node<T>* uncle = gparent->right;
            if (uncle != NULL && uncle->color == Red)
            {
                parent->color = Black;
                uncle->color = Black;
                gparent->color = Red;
                nod = gparent;
                parent = nod->parent;
            }
            else
            {
                if (parent->right == nod)
                {
                    leftrotate(root, parent);
                    swap(nod, parent);
                }
                rightrotate(root, gparent);
                gparent->color = Red;
                parent->color = Black;
                break;
            }
        }
        else {
            node<T>* uncle = gparent->left;
            if (uncle != NULL && uncle->color == Red)
            {
                gparent->color = Red;
                parent->color = Black;
                uncle->color = Black;

                nod = gparent;
                parent = nod->parent;
            }
            else
            {
                if (parent->left == nod)
                {
                    rightrotate(root, parent);
                    swap(parent, nod);
                }
                leftrotate(root, gparent);
                parent->color = Black;
                gparent->color = Red;
                break;
            }
        }
    }
    root->color = Black;
}

template<class T>
void sett<T>::dest(node<T>*& nod) {
    if (nod == NULL)
        return;
    dest(nod->left);
    dest(nod->right);
    delete nod;
    nod = nullptr;
}

template<class T>
void sett<T>::del(T key) {
    node<T>* delnode = search(key);
    if (delnode != nullptr) {
        del(root, delnode);
    }
}

template<class T>
void sett<T>::del(node<T>*& root, node<T>* nod) {
    node<T>* parent;
    node<T>* child;
    clr color;
    if (nod->left && nod->right) {
        node<T>* replace;
        replace = nod->right;
        while (replace->left)replace = replace->left;
        if (nod->parent) {
            if (nod->parent->left == nod)
                nod->parent->left = replace;
            else
                nod->parent->right = replace;
        }
        else
            root = replace;
        child = replace->right;
        parent = replace->parent;
        color = replace->color;
        if (parent == nod) {
            parent = replace;
        }
        else {
            if (child != NULL)
                child->parent = parent;
            parent->left = child;

            replace->right = nod->right;
            nod->right->parent = replace;
        }
        replace->parent = nod->parent;
        replace->color = nod->color;
        replace->left = nod->left;
        nod->left->parent = replace;
        if (color == Black)
            fixdel(root, child, parent);

        delete nod;
        return;
    }
    if (nod->left)
        child = nod->left;
    else
        child = nod->right;
    parent = nod->parent;
    color = nod->color;
    if (child) {
        child->parent = parent;
    }
    if (parent) {
        if (nod == parent->left)
            parent->left = child;
        else
            parent->right = child;
    }
    else {
        sett<T>::root = child;
    }
    if (color == Black) {
        fixdel(root, child, parent);
    }
    delete nod;
}

template<class T>
void sett<T>::fixdel(node<T>*& root, node<T>* nod, node<T>* parent) {
    node<T>* othernode;
    while (parent && ((!nod) || (nod->color == Black && nod != sett<T>::root))) {
        if (parent && parent->left == nod) {
            othernode = parent->right;
            if (othernode->color == Red)
            {
                othernode->color = Black;
                parent->color = Red;
                leftrotate(root, parent);
                othernode = parent->right;
            }
            else {
                if (!(othernode->right) || othernode->right->color == Black)
                {
                    othernode->left->color = Black;
                    othernode->color = Red;
                    rightrotate(root, othernode);
                    othernode = parent->right;
                }
                othernode->color = parent->color;
                parent->color = Black;
                othernode->right->color = Black;
                leftrotate(root, parent);
                nod = root;
                break;
            }
        }
        else {
            if (parent) {
                othernode = parent->left;
                if (othernode->color == Red)
                {
                    othernode->color = Black;
                    parent->color = Red;
                    rightrotate(root, parent);
                    othernode = parent->left;
                }
                if ((!othernode->left || othernode->left->color == Black) && (!othernode->right || othernode->right->color == Black))
                {
                    othernode->color = Red;
                    nod = parent;
                    parent = nod->parent;
                }
                else {
                    if (!(othernode->left) || othernode->left->color == Black)
                    {
                        othernode->right->color = Black;
                        othernode->color = Red;
                        leftrotate(root, othernode);
                        othernode = parent->left;
                    }
                    othernode->color = parent->color;
                    parent->color = Black;
                    othernode->left->color = Black;
                    rightrotate(root, parent);
                    nod = root;
                    break;
                }
            }
        }
    }
    if (nod)
        nod->color = Black;
}

template<class T>
node<T>* sett<T>::search(T key) {
    return search(root, key);
}

template<class T>
node<T>* sett<T>::search(node<T>* nod, T key)const {
    if (nod == nullptr || nod->key == key) {
        return nod;
    }
    else {
        if (key > nod->key)
            return search(nod->right, key);
        else
            return search(nod->left, key);
    }
}

template<class T>
void sett<T>::print() {
    if (root == nullptr)
        cout << "empty tree" << "\n";
    else {
        print(root);
    }
}

template<class T>
void sett<T>::print(node<T>* nod)const {
    if (nod == nullptr)
        return;
    if (nod->parent == nullptr) {
        cout << nod->key << "(" << nod->color << ")" << "is root" << "\n";
    }
    else {
        if (nod->parent->left == nod) {
            cout << nod->key << "(" << nod->color << ") is " << nod->parent->key << "'s " << "left child" << "\n";
        }
        else {
            cout << nod->key << "(" << nod->color << ") is " << nod->parent->key << "'s " << "right child" << "\n";
        }
    }
    print(nod->left);
    print(nod->right);
}

template<class T>
vector<pair<pair<int, int>, pair<int, int>>> sett<T>::draw1() {
    int x = 200, y = 10, dl = 200;
    queue <pair< pair<node<T>,int>, pair<int, int>>>q;
    node<T> nod = *sett<T>::root;
    q.push({{ nod,dl } ,{x, y}});
    vector<pair<pair<int,int>,pair<int, int>>>v;
    v.push_back({ { (T)nod.key,nod.color}, {x,y} });
    int cn = 40;
    while (!q.empty()) {
        int a = q.front().second.first;
        int b = q.front().second.second;
        int dl = q.front().first.second;
        auto nod = q.front().first.first;
        q.pop();
        dl = dl / 2;
        if (nod.left) {
            auto n = *nod.left;
            q.push({ { n,dl }, { a - dl,b+cn} });
            v.push_back({ {(T)n.key,n.color},{a - dl,b + cn} });
        }
        if (nod.right) {
            auto n = *nod.right;
            q.push({ { n,dl }, { a + dl,b+cn } });
            v.push_back({ { (T)n.key,n.color},{a + dl,b + cn} });
        }
    }
    return v;
}
#endif
