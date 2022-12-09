#include <bits/stdc++.h>
using namespace std;

int MaxChildren = 3;
int MinChildren = 2;
int MaxKey = 2;
int MinKey = 1;
int MAX = 2;
int level = 0;
vector<int> records;

class Node
{
private:
    int *key, size;
    Node **chld;
    bool isleaf;
    friend class BplusTree;

public:
    Node()
    {
        key = new int[MaxKey];
        chld = new Node *[MaxChildren];
    };
};

class BplusTree
{
private:
    Node *root;
    void InsertInternalNode(int, Node *, Node *);
    Node *findParent(Node *, Node *);
    void DeleteInternalNode(int, Node *, Node *);

public:
    void Key_Search(int);
    void InsertNode(int);
    void DeleteNode(int);
    void Display(Node *);
    Node *getRoot();
    BplusTree()
    {
        root = nullptr;
    };
};

void BplusTree::Key_Search(int x)
{
    if (root == nullptr)
    {
        cout << "Tree is empty\n";
    }
    else
    {
        Node *ptr = root;
        while (ptr->isleaf == false)
        {
            for (int i = 0; i < ptr->size; i++)
            {
                if (x < ptr->key[i])
                {
                    ptr = ptr->chld[i];
                    break;
                }
                if (i == ptr->size - 1)
                {
                    ptr = ptr->chld[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < ptr->size; i++)
        {
            if (ptr->key[i] == x)
            {
                cout << "Found!\n";
                return;
            }
        }
        cout << "Not found!\n";
    }
}

void BplusTree::InsertNode(int x)
{
    if (root == nullptr)
    {
        root = new Node;
        root->key[0] = x;
        root->isleaf = true;
        root->size = 1;
    }
    else
    {
        Node *ptr = root;
        Node *parent;
        while (ptr->isleaf == false)
        {
            parent = ptr;
            for (int i = 0; i < ptr->size; i++)
            {
                if (x <= ptr->key[i])
                {
                    ptr = ptr->chld[i];
                    break;
                }
                if (i == ptr->size - 1)
                {
                    ptr = ptr->chld[i + 1];
                    break;
                }
            }
        }
        if (ptr->size < MaxKey)
        {
            int i = 0;
            while (x > ptr->key[i] && i < ptr->size)
            {
                i++;
            }
            for (int j = ptr->size; j > i; j--)
            {
                ptr->key[j] = ptr->key[j - 1];
            }
            ptr->key[i] = x;
            ptr->size++;
            ptr->chld[ptr->size] = ptr->chld[ptr->size - 1];
            ptr->chld[ptr->size - 1] = nullptr;
        }
        else
        {
            Node *newLeaf = new Node;
            int TempNode[MaxChildren];
            for (int i = 0; i < MaxKey; i++)
            {
                TempNode[i] = ptr->key[i];
            }
            int i = 0, j;
            while (x > TempNode[i] && i < MaxKey)
            {
                i++;
            }
            for (int j = MaxChildren; j > i; j--)
            {
                TempNode[j] = TempNode[j - 1];
            }
            TempNode[i] = x;
            newLeaf->isleaf = true;
            ptr->size = (MaxChildren) / 2;
            newLeaf->size = MaxChildren - (MaxChildren) / 2;
            ptr->chld[ptr->size] = newLeaf;
            newLeaf->chld[newLeaf->size] = ptr->chld[MaxKey];
            ptr->chld[MaxKey] = nullptr;
            for (i = 0; i < ptr->size; i++)
            {
                ptr->key[i] = TempNode[i];
            }
            for (i = 0, j = ptr->size; i < newLeaf->size; i++, j++)
            {
                newLeaf->key[i] = TempNode[j];
            }
            if (ptr == root)
            {
                Node *newRoot = new Node;
                newRoot->key[0] = newLeaf->key[0];
                newRoot->chld[0] = ptr;
                newRoot->chld[1] = newLeaf;
                newRoot->isleaf = false;
                newRoot->size = 1;
                root = newRoot;
            }
            else
            {
                InsertInternalNode(newLeaf->key[0], parent, newLeaf);
            }
        }
    }
}

void BplusTree::InsertInternalNode(int x, Node *ptr, Node *child)
{
    if (ptr->size < MaxKey)
    {
        int i = 0;
        while (x > ptr->key[i] and i < ptr->size)
        {
            i++;
        }
        for (int j = ptr->size; j > i; j--)
        {
            ptr->key[j] = ptr->key[j - 1];
        }
        for (int j = ptr->size + 1; j > i + 1; j--)
        {
            ptr->chld[j] = ptr->chld[j - 1];
        }
        ptr->key[i] = x;
        ptr->size++;
        ptr->chld[i + 1] = child;
    }
    else
    {
        Node *newInternal = new Node;
        int TempKey[MaxChildren];
        Node *virtualPtr[MaxKey + 2];
        for (int i = 0; i < MaxKey; i++)
        {
            TempKey[i] = ptr->key[i];
        }
        for (int i = 0; i < MaxChildren; i++)
        {
            virtualPtr[i] = ptr->chld[i];
        }
        int i = 0, j;
        while (x > TempKey[i] and i < MaxKey)
            i++;
        for (int j = MaxChildren; j > i; j--)
        {
            TempKey[j] = TempKey[j - 1];
        }
        TempKey[i] = x;
        for (int j = MaxKey + 2; j > i + 1; j--)
        {
            virtualPtr[j] = virtualPtr[j - 1];
        }
        virtualPtr[i + 1] = child;
        newInternal->isleaf = false;
        ptr->size = (MaxChildren) / 2;
        newInternal->size = MaxKey - (MaxChildren) / 2;
        for (i = 0, j = ptr->size + 1; i < newInternal->size; i++, j++)
        {
            newInternal->key[i] = TempKey[j];
        }
        for (i = 0, j = ptr->size + 1; i < newInternal->size + 1; i++, j++)
        {
            newInternal->chld[i] = virtualPtr[j];
        }
        if (ptr != root)
        {
            InsertInternalNode(ptr->key[ptr->size], findParent(root, ptr), newInternal);
        }
        else
        {
            Node *newRoot = new Node;
            newRoot->key[0] = ptr->key[ptr->size];
            newRoot->chld[0] = ptr;
            newRoot->chld[1] = newInternal;
            newRoot->isleaf = false;
            newRoot->size = 1;
            root = newRoot;
        }
    }
}

Node *BplusTree::findParent(Node *ptr, Node *child)
{
    Node *parent;
    if (ptr->isleaf or (ptr->chld[0])->isleaf)
    {
        return nullptr;
    }
    for (int i = 0; i < ptr->size + 1; i++)
    {
        if (ptr->chld[i] == child)
        {
            parent = ptr;
            return parent;
        }
        else
        {
            parent = findParent(ptr->chld[i], child);
            if (parent != nullptr)
                return parent;
        }
    }
    return parent;
}

void BplusTree::Display(Node *ptr)
{
    if (ptr != nullptr)
    {
        cout << "level : " << level << '\n';
        for (int i = 0; i < ptr->size; i++)
        {
            cout << ptr->key[i] << " ";
        }
        cout << '\n';
        // cout <<"------"<< "\n";
        if (ptr->isleaf == false)
        {
            level++;
            for (int i = 0; i < ptr->size + 1; i++)
            {
                Display(ptr->chld[i]);
            }
        }
    }
    else
    {
        level = 0;
    }
}

Node *BplusTree::getRoot()
{
    return root;
}

int main()
{
    BplusTree node;
    level = 0;
    node.InsertNode(5);
    node.Display(node.getRoot());
    cout << "--------" << '\n';
    level = 0;
    node.InsertNode(15);
    node.Display(node.getRoot());
    level = 0;
    cout << "--------" << '\n';
    node.InsertNode(25);
    node.Display(node.getRoot());
    cout << "--------" << '\n';
    level = 0;
    node.InsertNode(35);
    node.Display(node.getRoot());
    cout << "--------" << '\n';
    level = 0;
    node.InsertNode(45);
    node.Display(node.getRoot());
    cout << "--------" << '\n';
    level = 0;
    node.InsertNode(55);
    node.Display(node.getRoot());
    cout << "--------" << '\n';
    level = 0;
    node.InsertNode(40);
    node.Display(node.getRoot());
    cout << "--------" << '\n';
    level = 0;
    node.InsertNode(30);
    node.Display(node.getRoot());
    cout << "--------" << '\n';
    level = 0;
    node.InsertNode(20);
    node.Display(node.getRoot());
    cout << "--------" << '\n';

    node.Key_Search(15);
}