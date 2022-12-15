#include "myUnorderedMap.h"

template<class Key, class T>
myUnorderedMap<Key, T>::myUnorderedMap() : size(701), begin(nullptr),
    cbegin(nullptr), max_hash_value(0)
{
    cout << "Constructor!" << "\n";
    hash_set.resize(size, nullptr);
}


template<class Key, class T>
myUnorderedMap<Key, T>
    ::myUnorderedMap<Key, T>(const myUnorderedMap& other_map)
    : size(701), begin(nullptr), cbegin(nullptr), max_hash_value(0)
{
    cout << "Copy operator()!" << endl;
    if (this == &other_map) return;

    copy_handler(other_map);
}



template<class Key, class T>
myUnorderedMap<Key, T>& myUnorderedMap<Key, T>
    ::operator=(const myUnorderedMap& other_map)
{
    cout << "Copy operator=!" << endl;

    if (this == &other_map) return *this;

    copy_handler(other_map);

    return *this;
}


template<class Key, class T>
myUnorderedMap<Key, T>
    ::myUnorderedMap<Key, T>(myUnorderedMap&& other_map) noexcept
    : size(701), begin(nullptr), cbegin(nullptr), max_hash_value(0)
{
    cout << "Move operator()!" << "\n";
    if (this == &other_map) return;

    hash_set = move(other_map.hash_set);
    begin = other_map.begin;
    cbegin = other_map.cbegin;

    other_map.begin = nullptr;
    other_map.cbegin = nullptr;
}


template<class Key, class T>
myUnorderedMap<Key, T>& myUnorderedMap<Key, T>
    ::operator=(myUnorderedMap&& other_map) noexcept
{
    if (this == &other_map) return *this;

    hash_set = move(other_map.hash_set);
    begin = other_map.begin;
    cbegin = other_map.cbegin;

    other_map.begin = nullptr;
    other_map.cbegin = nullptr;

    return *this;
}


template<class Key, class T>
T& myUnorderedMap<Key, T>::operator[](const Key& key)
{
    size_t hash_val = hash_func(key);

    if (hash_set[hash_val] == nullptr)
    {
        // if node with corresponding hash_val doesn't exists,
        // create new node, connect it to the main linked list and
        // put it's pointer to the hash_set.
        shared_ptr<ListNode<Key, T>> node
            = make_shared<ListNode<Key, T>>(key, hash_val);
        if (isEmpty())
        {
            begin = node;
            cbegin = node;
        }
        else
        {
            cbegin->next = node;
            cbegin = node;
        }
        hash_set[hash_val] = node;
        
        return node->value_type.second;
    }
    else
    {
        // if node with corresponding hash_val exists,
        // create new node and connect it to the main linked list.
        shared_ptr<ListNode<Key, T>> it = hash_set[hash_val];
        while (it->next != nullptr)
        {
            it = it->next;
        }
        shared_ptr<ListNode<Key, T>> node
            = make_shared<ListNode<Key, T>>(key, hash_val);
        it->next = node;
        
        if (node->hash_val > max_hash_value)
        {
            max_hash_value = hash_val;
            cbegin = node;
        }

        return node->value_type.second;
    }
}


template<class Key, class T>
int myUnorderedMap<Key, T>::hash_func(const Key& key)
{
    int hash_val = static_cast<int>(key) % size;
    return hash_val;
}


template<class Key, class T>
void myUnorderedMap<Key, T>::print()
{
    if (begin == nullptr) cout << "Called map is empty!" << endl;
    shared_ptr<ListNode<Key, T>> it = begin;
    while (it != nullptr)
    {
        cout << it->value_type.second << " ";
        it = it->next;
    }
    cout << endl;
}


template<class Key, class T>
bool myUnorderedMap<Key, T>::isEmpty()
{
    bool isEmpty = 1;
    for (const auto& pListNode : hash_set)
    {
        if (pListNode != nullptr)
        {
            isEmpty = 0;
            break;
        }
    }
    return isEmpty;
}


template<class Key, class T>
void myUnorderedMap<Key, T>::hand(vector<shared_ptr<ListNode<Key, T>>>& vec)
{
    for (auto ptr : vec)
    {
        if (ptr == nullptr) cout << "NULL!";
        else cout << ptr->hash_val;
    }
    cout << endl;
}


template<class Key, class T>
void myUnorderedMap<Key, T>::copy_handler(const myUnorderedMap& other_map)
{
    hash_set = other_map.hash_set;
    shared_ptr<ListNode<Key, T>> node
        = make_shared<ListNode<Key, T>>(other_map.begin);
    for (shared_ptr<ListNode<Key, T>> pListNode : hash_set)
    {
        if (pListNode == nullptr) continue;
        pListNode = node;
        while (node->next != nullptr
            && node->hash_val == pListNode->hash_val)
        {
            shared_ptr<ListNode<Key, T>> tmp = node;
            node = make_shared<ListNode<Key, T>>(node->next);
            tmp->next = node;
        }
    }

    for (int i = 0; i < size; i++)
    {
        if (hash_set[i] != nullptr)
        {
            begin = hash_set[i];
            break;
        }
    }

    for (int i = size - 1; i >= 0; i--)
    {
        if (hash_set[i] != nullptr)
        {
            cbegin = hash_set[i];
            break;
        }
    }
}

template class myUnorderedMap<int, int>;
template class myUnorderedMap<int, double>;
template class myUnorderedMap<double, int>;
template class myUnorderedMap<double, double>;