/*
    Name : Shruti T. Avhad
    Roll No. : 20074030
    DBMS Lab B+ Trees
*/
#include<bits/stdc++.h>
using namespace std;

int MaxChildren = 3;
int MinChildren = 2;
int MaxKey = 2;
int MinKey = 1;
int level = 0;
vector <int> records;

class Node
{
    public:
        int *key, size;
        Node **chld;
        bool isleaf;
        //friend class BplusTree;

    public:
        Node()
        {
            key = new int[MaxKey];
            chld = new Node *[MaxChildren];
        };
};


class BplusTree
{
    public:
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
    auto y = find(records.begin(),records.end(),x);
    if(y==records.end())
    {cout<<"Not Found!\n";}
    else{cout<<"Found!\n";}
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
        Node *virtualPtr[MaxKey+ 2];
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
        {
            i++;
        }
        for (int j = MaxChildren; j > i; j--)
        {
            TempKey[j] = TempKey[j - 1];
        }
        TempKey[i] = x;
        for (int j = MaxKey+2; j > i + 1; j--)
        {
            virtualPtr[j] = virtualPtr[j - 1];
        }
        virtualPtr[i + 1] = child;
        newInternal->isleaf = false;
        ptr->size = (MaxChildren) / 2;
        newInternal->size = MaxKey- (MaxChildren) / 2;
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
    int i=0;
    while(i < (ptr->size + 1))
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
        i++;
    }
    return parent;
}

void BplusTree::DeleteNode(int k)
{

    if (root == NULL)
    {
        cout << "Tree is empty\n";
    }
    else
    {
        Node *ptr = root;
        Node *parentnode;
        int leftsib,rightsib;
        while (ptr->isleaf == false)
        {
            for (int i = 0; i < ptr->size; i++)
            {
                parentnode = ptr;
                leftsib = i - 1;
                rightsib = i + 1;
                if (k < ptr->key[i])
                {
                    ptr = ptr->chld[i];
                    break;
                }
                if (i == ptr->size - 1)
                {
                    leftsib = i;
                    rightsib = i + 2;
                    ptr = ptr->chld[i + 1];
                    break;
                }
            }
        }
        bool found = false;
        int pos;
        for (pos = 0; pos < ptr->size; pos++)
        {
            if (ptr->key[pos] == k)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Not found\n";
            return;
        }
        for (int i = pos; i < ptr->size; i++)
        {
            ptr->key[i] = ptr->key[i + 1];
        }
        ptr->size--;
        if (ptr == root)
        {
            for (int i = 0; i < MaxChildren; i++)
            {
                ptr->chld[i] = NULL;
            }
            if (ptr->size == 0)
            {
                cout << "Tree died\n";
                delete[] ptr->key;
                delete[] ptr->chld;
                delete ptr;
                root = NULL;
            }
            return;
        }
        ptr->chld[ptr->size] = ptr->chld[ptr->size + 1];
        ptr->chld[ptr->size + 1] = NULL;
    }
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
        cout<<'\n';
        //cout <<"------"<< "\n";
        if (ptr->isleaf == false)
        {
            level++;
            for (int i = 0; i < ptr->size + 1; i++)
            {
                Display(ptr->chld[i]);
            }
        }
        
    }
    level=0;
}

Node *BplusTree::getRoot()
{
    return root;
}

int main()
{
    cout << " 1 : Insert a new record \n 2 : Search a record\n 3 : Delete a record \n 4 : Display the Bplus Tree \n 5 : Quit \n";
    int choice, key;
    cout<<"->";
    cin >> choice;
    BplusTree n;
    while (choice != 5)
    {
        switch (choice)
        {
        case 1:
        {
            cout << "Enter the key to be Inserted : ";
            cin >> key;
            records.push_back(key);
            level=0;
            n.InsertNode(key);
            break;
        }
        case 2:
        {
            cout << "Enter the key to be Searched : ";
            cin >> key;
            n.Key_Search(key);
            break;
        }
        case 3:
        {
            cout << "Enter the key to be deleted : ";
            cin >> key;
            n.DeleteNode(key);
            break;
        }
        case 4:
        {
            n.Display(n.getRoot());
            break;
        }
        }
        cout << "->";
        cin >> choice;
    }
}