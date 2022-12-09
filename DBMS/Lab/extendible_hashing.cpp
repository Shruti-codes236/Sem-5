/*
    Name: Shruti T. Avhad
    Roll No.: 20074030
    DBMS Lab 9
    Extendible Hashing 
*/

#include <bits/stdc++.h>
using namespace std;
#define DELETION_MODE 0

int swap(long long int *a, long long int *b)
{
    long long int temp;
    temp = *a;
    *a = *b;
    *b = temp;
    return 0;
}

class Directory
{
private:
    int bucket_size, global_depth, clock;
    vector<pair<Bucket *, int>> buckets;
public:
    Directory(int depth, int bucket_size);
    int Bucket_Count() { return 1 << global_depth; };
    int pairIndex(int bucket_no, int depth);
    void grow(void);
    int Hash_Func(int n);
    void split(int bucket_no);
    void merge(int bucket_no);
    void Insert(int bucket_no);
    int search (int bucket_no);
    void Delete(int bucket_no);
    string getBucket_id(int n);
};

class Bucket
{
private:
    int bucket_depth, local_depth;
    vector<int> records;
public:
    string bucket_id(int n);
    int is_Full(void);
    int is_Empty(void);
    int get_Depth(void);
    Bucket(int depth, int size);
    void search(int key);
    int insert (int key);
    void merge(int key);
    int Find_Key(int key);
    int Delete (int key);
    vector <int> copy(void);
    int changeDepth(int value);
    void clear();
    string get_Value_From_Key(int key);
};

void Directory::Insert(int key)
{
    int bucket_no= Hash_Func(key);
    if (buckets[bucket_no].first->is_Empty()==0)
    {
        for (auto i : buckets) 
        {
            if (i.first == nullptr)
            {
                continue;
            }
            i.second--;
        }
        buckets[bucket_no].second--;
    }
    int status = (buckets[bucket_no].first)->insert(key);
    if (status == 0)
    {
        split(bucket_no);
        Insert(key);
    }
}

void Directory::Delete(int key)
{

}

void Directory::split(int bucket_no)
{
    int local_depth, pair_index, index_diff, dir_size;
    vector<int> temp;
    local_depth = (buckets[bucket_no].first)->changeDepth(1);
    if (local_depth > global_depth)
        grow();
    pair_index = pairIndex(bucket_no, local_depth);
    clock++;
    buckets[pair_index] = {new Bucket(local_depth, bucket_size), clock};
    temp = (buckets[bucket_no].first)->copy();
    (buckets[bucket_no].first)->clear();
    index_diff = 1 << local_depth;
    dir_size = 1 << global_depth;
    for (int i = pair_index - index_diff; i >= 0; i -= index_diff)
        buckets[i] = buckets[pair_index];
    for (int i = pair_index + index_diff; i < dir_size; i += index_diff)
        buckets[i] = buckets[pair_index];
    for (auto it = temp.begin(); it != temp.end(); it++)
        Insert((*it), 1);
}

void Directory::merge(int bucket_no)
{
    int local_depth, pair_index, index_diff, dir_size;
    local_depth = (buckets[bucket_no].first)->get_Depth();
    pair_index = pairIndex(bucket_no, local_depth);
    index_diff = 1 << local_depth;
    dir_size = 1 << global_depth;
    if ((buckets[pair_index].first)->get_Depth() == local_depth)
    {
        (buckets[pair_index].first)->changeDepth(-1);
        delete(buckets[bucket_no].first);
        buckets[bucket_no].first = nullptr;
        buckets[bucket_no] = buckets[pair_index];
        for (int i = bucket_no - index_diff; i >= 0; i -= index_diff)
            buckets[i] = buckets[pair_index];
        for (int i = bucket_no + index_diff; i < dir_size; i += index_diff)
            buckets[i] = buckets[pair_index];
    }
}

Directory::Directory(int depth, int bucket_size)
{
    this->global_depth = depth;
    this->bucket_size = bucket_size;
    this->clock = 1 << (global_depth + 1);
    for (int i = 0; i < 1 << depth; i++)
    {
        buckets.push_back({new Bucket(depth, bucket_size), clock});
    }
}

int Directory::Hash_Func(int n)
{
    return n & ((1 << global_depth) - 1);
}

string Directory::getBucket_id(int n)
{
    int b;
    string s;
    b = (buckets[n].first)->get_Depth();
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

int Directory::pairIndex(int bucket_no, int depth)
{
    return bucket_no ^ (1 << (depth - 1));
}

void Directory::grow(void)
{
    for (int i = 0; i < 1 << global_depth; i++)
        buckets.push_back(buckets[i]);
    global_depth++;
}

int Directory::search(int key)
{
    int bucket_no = Hash_Func(key);
    (buckets[bucket_no].first)->search(key);
}
void Directory::split(int bucket_no)
{
    int local_depth, pair_index, index_diff, dir_size;
    vector<int> temp;
    local_depth = (buckets[bucket_no].first)->changeDepth(1);
    if (local_depth > global_depth)
        grow();
    pair_index = pairIndex(bucket_no, local_depth);
    clock++;
    buckets[pair_index] = {new Bucket(local_depth, bucket_size), clock};
    temp = (buckets[bucket_no].first)->copy();
    (buckets[bucket_no].first)->clear();
    index_diff = 1 << local_depth;
    dir_size = 1 << global_depth;
    for (int i = pair_index - index_diff; i >= 0; i -= index_diff)
        buckets[i] = buckets[pair_index];
    for (int i = pair_index + index_diff; i < dir_size; i += index_diff)
        buckets[i] = buckets[pair_index];
    for (auto it = temp.begin(); it != temp.end(); it++)
        Insert((*it));
}

Directory::~Directory()
{
    for (int i = 0; i < buckets.size(); i++)
    {
        if (buckets[i].first == nullptr)
            continue;
        (buckets[i].first)->clear();
        Delete((buckets[i].first));
    }
}

Bucket::Bucket(int depth, int size)
{
    this->bucket_depth = depth;
    this->local_depth = size;
}

int Bucket::get_Depth(void)
{
    return bucket_depth;
}

int Bucket::is_Empty(void)
{
    if (records.size() == 0)
        return 1;
    else
        return 0;
}

int Bucket::is_Full()
{
    if (records.size() == local_depth)
        return 1;
    else
        return 0;
}

int Bucket::is_Empty(void)
{
    if (records.size() == 0)
        return 1;
    else
        return 0;
}

void Bucket::search(int key)
{
    auto it = find(records.begin(), records.end(), key);
    // if(it!=records.end()) cout<<"Found!"<<endl;
    // else cout<<"Not Found!"<<endl;
}

int Bucket::insert(int key)
{
    auto it = find(records.begin(), records.end(), key);
    if (it != records.end())
        return -1;
    if (is_Full())
        return 0;
    records.push_back(key);
    return 1;
}
vector<int> Bucket::copy(void)
{
    vector<int> temp(records.begin(), records.end());
    return temp;
}

void Bucket::clear(void)
{
    records.clear();
}

int Bucket::Delete (int key)
{
    auto it = find(records.begin(), records.end(), key);
    if (it != records.end())
    {
        records.erase(it);
        return 1;
    }
    else
    {
        // cout<<"Error : Key Not Found!"<<endl;
        return 0;
    }
}

int Bucket::changeDepth(int value)
{
    local_depth += value;
    return local_depth;
}

int main()
{
    ios_base::sync_with_stdio(0), cin.tie(0);

    int max_bucket_size, global_depth;
    int key;
    int choice;
    //bool show_duplicate_buckets = 0;
    cout << "Enter the global Depth :\n";
    cin >> global_depth;
    cout << "Enter the max bucket size :\n";
    cin >> max_bucket_size;
    Directory d(global_depth, max_bucket_size);
    cout << "2: Insert a new record \n3 : Search a record\n 4 : Delete a record \n5 : Display status of the hash table \n6 : Quit \n";
    cin >> choice;
    while (choice != 6)
    {
        switch (choice)
        {
        case 2:
        {
            cout << "Enter the key to be inserted : ";
            cin >> key;
            d.Insert(key);
            break;
        }
        case 3:
        {
            cout << "Enter the key to be searched : ";
            cin >> key;
            d.search(key);
            break;
        }
        case 4:
        {
            cout << "Enter the key to be deleted : ";
            cin >> key;
            d.Delete(key);
            break;
        }
        case 5:
        {
            d.Display();
            break;
        }
        }
        cin >> choice;
    }
    return 0;
}
