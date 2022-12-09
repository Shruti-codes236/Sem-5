/*
    Name: Shruti Tulshidas Avhad
    Roll No.: 20074030
    DBMS Lab
    Extendible Hashing Implementation

    ALGORITHM:
    1. Taking input of Data records, I have taken the input as integers.
    2. Converting them into binary format to consider their trailing k bits to insert into the directory.
    3. Checking the Global Depth of the directory.
    4. Identifying the Directory: Considering the ‘Global-Depth’ and matching it with trailing bits of binary data elements in the directory.
    5. Navigation: Navigating to the bucket pointed by the directory with the required bucket_id.
    6. Insertion : we will check if the bucket overflows when new data is entered. If an overflow is occuring, we will re-allocate the directories accordingly.
    7. In case of overflow, we first Check if the local depth is less than or equal to the global depth.
        Then choose one of the cases below.
        i). If local depth == global depth, then Directory grows, as well as Bucket Split happens. Then incrementing the global_depth and the local_depth value by 1.
            And, assign appropriate pointers. The directory grows two fold everytime.
        ii). If local depth < global depth, then only
        Bucket Split takes place. Then increment only the local_depth value by 1. And, assign appropriate pointers.
    8. The records present in the overflowing bucket that is split are rehashed w.r.t the new global depth of the directory.
    9. The element is successfully hashed.
    10. Deletion: Here after deletion as well two cases arise,
        i). bucket_size > 2*local_depth of the bucket then the bucket is merged into its respective co-bucket and pointer.
        ii). Nothing is to be done otherwise.
*/

#include <bits/stdc++.h>
using namespace std;

class Bucket
{
public:
    int depth, bucket_size;
    vector<int> records;
    Bucket(int depth, int size);
    int Insert(int key);
    vector<int> copy();
    int Delete(int key);
    void Key_Search(int key);
    int is_Empty();
    int Change_Depth(int amount);
    void Display();
};

class Directory
{
public:
    int global_depth, bucket_size, dir_size;
    vector<pair<Bucket *, int> > buckets;
    int hash(int n);
    int PairIndex(int bucket_no, int depth);
    void grow();
    void shrink();
    void split(int bucket_no);
    void merge(int bucket_no);
    string bucket_id(int n);
    Directory(int depth, int bucket_size);
    void Insert(int key);
    void Delete(int key);
    void Key_Search(int key);
    void Display();
};

//sort function based on second element of vector pairs
bool condition(pair<Bucket *, int> &a, pair<Bucket *, int> &b)
{
    return a.second < b.second;
}

Directory::Directory(int depth, int bucket_size)
{
    this->global_depth = depth;
    this->bucket_size = bucket_size;
    this->dir_size = 1 << (global_depth + 1);
    for (int i = 0; i < 1 << depth; i++)
    {
        buckets.push_back({new Bucket(depth, bucket_size), dir_size});
    }
}

int Directory::hash(int n)
{
    return n & ((1 << global_depth) - 1);
}

int Directory::PairIndex(int bucket_no, int depth)
{
    return bucket_no ^ (1 << (depth - 1));
}

void Directory::Key_Search(int key)
{
    int bucket_no = hash(key);
    (buckets[bucket_no].first)->Key_Search(key);
}

void Directory::grow(void)
{
    for (int i = 0; i < 1 << global_depth; i++)
        {buckets.push_back(buckets[i]);}
    global_depth++;
}

void Directory::shrink(void)
{
    for (int i = 0; i < buckets.size(); i++)
    {
        if ((buckets[i].first)->depth == global_depth)
            return;
    }
    global_depth--;
    for (int i = 0; i < 1 << global_depth; i++)
        buckets.pop_back();
}

void Directory::split(int bucket_no)
{
    int local_depth, pair_index, index_diff, dir_size;
    vector<int> temp;
    local_depth = (buckets[bucket_no].first)->Change_Depth(1);
    if (local_depth > global_depth) grow();
    pair_index = PairIndex(bucket_no, local_depth);
    dir_size++;
    buckets[pair_index] = {new Bucket(local_depth, bucket_size), dir_size};
    temp = (buckets[bucket_no].first)->copy();
    (buckets[bucket_no].first)->records.clear();
    index_diff = 1 << local_depth;
    dir_size = 1 << global_depth;
    for (int i = pair_index - index_diff; i >= 0; i -= index_diff)
        {buckets[i] = buckets[pair_index];}
    for (int i = pair_index + index_diff; i < dir_size; i += index_diff)
        {buckets[i] = buckets[pair_index];}
    for (auto it = temp.begin(); it != temp.end(); it++)
        {Insert((*it));}
}

void Directory::merge(int bucket_no)
{
    int local_depth, pair_index, index_diff, dir_size;
    local_depth = (buckets[bucket_no].first)->depth;
    pair_index = PairIndex(bucket_no, local_depth);
    index_diff = 1 << local_depth;
    dir_size = 1 << global_depth;
    if ((buckets[pair_index].first)->depth == local_depth)
    {
        (buckets[pair_index].first)->Change_Depth(-1);
        delete (buckets[bucket_no].first);
        buckets[bucket_no].first = nullptr;
        buckets[bucket_no] = buckets[pair_index];
        for (int i = bucket_no - index_diff; i >= 0; i -= index_diff)
            buckets[i] = buckets[pair_index];
        for (int i = bucket_no + index_diff; i < dir_size; i += index_diff)
            buckets[i] = buckets[pair_index];
    }
}

//Converting into binary for k trailing bits
string Directory::bucket_id(int n)
{
    int b;
    string s;
    b = (buckets[n].first)->depth;
    s = "";
    while (n > 0 && b > 0)
    {
        s = (n % 2 == 0 ? "0" : "1") + s;
        n /= 2;
        b--;
    }
    while (b > 0)
    {
        s = "0" + s;
        b--;
    }
    return s;
}

void Directory::Insert(int key)
{
    int bucket_no = hash(key);
    if ((buckets[bucket_no].first)->is_Empty() == 0)
    {
        for (auto i : buckets)
        {
            if (i.first == nullptr)
                continue;
            i.second--;
        }
        buckets[bucket_no].second--;
    }
    int status = (buckets[bucket_no].first)->Insert(key);
    if (status == 0)
    {
        split(bucket_no);
        Insert(key);
    }
}

void Directory::Delete(int key)
{
    int bucket_no = hash(key);
    (buckets[bucket_no].first)->Delete(key);
    if ( ((buckets[bucket_no].first)->records.size() < bucket_size/2) && (buckets[bucket_no].first)->depth > 1)
        merge(bucket_no);
}

void Directory::Display()
{
    int i, j, d;
    int size = 0;
    string s;
    set<string> shown;
    cout <<"current global depth : "<< global_depth << "\n";
    for (i = 0; i < buckets.size(); i++)
    {
        s = bucket_id(i);
        if ( shown.find(s) == shown.end())
        {
            shown.insert(s);
            if ((buckets[i].first)->is_Empty() == 0 )
                size++;
        }
    }

    cout <<"number of buckets : "<< size << "\n";
    cout<<"Keys and local depth of respective Buckets : \n";
    shown.clear();
    vector<pair<Bucket *, int>> temp = buckets;
    sort(temp.begin(), temp.end(), condition);
    for (i = 0; i < temp.size(); i++)
    {
        s = bucket_id(i);
        if (shown.find(s) == shown.end())
        {
            shown.insert(s);
            if ((temp[i].first)->is_Empty() == 0)
                (temp[i].first)->Display();
        }
    }
}

Bucket::Bucket(int depth, int size)
{
    this->depth = depth;
    this->bucket_size = size;
}

int Bucket::Insert(int key)
{
    auto it = find(records.begin(), records.end(), key);
    if (it != records.end())
        return -1;
    if (records.size() == bucket_size)
        return 0;
    records.push_back(key);
    //cout<<"Key Inserted!\n";
    return 1;
}

int Bucket::Delete(int key)
{
    auto it = find(records.begin(), records.end(), key);
    if (it != records.end())
    {
        records.erase(it);
        return 1;
    }
    else
    {
        cout<<"Error : Key Not Found!"<<endl;
        return 0;
    }
}

void Bucket::Key_Search(int key)
{
    auto it = find(records.begin(), records.end(), key);
    if(it!=records.end()) cout<<"Found!"<<endl;
    else cout<<"Not Found!"<<endl;
}

int Bucket::is_Empty(void)
{
    if (records.size() == 0)
        return 1;
    else
        return 0;
}

int Bucket::Change_Depth(int amount)
{
    depth += amount;
    return depth;
}

vector<int> Bucket::copy(void)
{
    vector<int> temp(records.begin(), records.end());
    return temp;
}

void Bucket::Display()
{
    cout << records.size() << " " << depth << "\n";
}

int main()
{
    int max_bucket_size, global_depth;
    int key;
    int choice;
    cout << "Enter the global Depth :\n";
    cin >> global_depth;
    cout << "Enter the max bucket size :\n";
    cin >> max_bucket_size;
    Directory d(global_depth, max_bucket_size);
    cout << " 1 : Insert a new record \n 2 : Search a record\n 3 : Delete a record \n 4 : Display the directory pointing to records \n 5 : Quit \n";
    cin >> choice;
    while (choice != 5)
    {
        switch (choice)
        {
        case 1:
        {
            cout << "Enter the key to be Inserted : ";
            cin >> key;
            d.Insert(key);
            break;
        }
        case 2:
        {
            cout << "Enter the key to be Searched : ";
            cin >> key;
            d.Key_Search(key);
            break;
        }
        case 3:
        {
            cout << "Enter the key to be deleted : ";
            cin >> key;
            d.Delete(key);
            break;
        }
        case 4:
        {
            d.Display();
            break;
        }
        }
        cin >> choice;
    }
    return 0;
}