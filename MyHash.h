// MyHash.h

#ifndef MYHASH_H
#define MYHASH_H

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;


private:
    struct Node {
        KeyType key;
        ValueType value;
        Node* next;
    };
    
    void cleanup();
    void reset(int buckets);
    unsigned int getHashedNum(const KeyType& key) const;
    ValueType* findInTable(Node** table, const KeyType& key) const;
    void addToTable(Node** table, const KeyType& key, const ValueType& value);
    void reassociate();
    
    Node** m_hashTable;
    double m_maxLoadFactor;
    int m_numBuckets;
    int m_numItems;
};

/**
 * Constructs a new hash table.
 * @runtime O(B), B = number of buckets
 */
template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor) {
    m_numBuckets = 100;
    m_numItems = 0;
    m_hashTable = new Node*[m_numBuckets];
    
    // initalize hash table to nullptr
    for (int i = 0; i < m_numBuckets; i++) { // O(B)
        m_hashTable[i] = nullptr;
    }
    
    // if load factor > 2.0, set it to 2.0
    m_maxLoadFactor = (maxLoadFactor > 2.0) ? 2.0 : maxLoadFactor;
}

/**
 * Destructs a hash table.
 * @runtime O(B), B = number of buckets
 */
template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash() {
    cleanup();
}

/**
 * Frees all memory associated with the current hash table, then
 * allocates a new empty hash table.
 * @runtime O(B)
 */
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset() {
    reset(100);
}

/**
 * Adds a new association to the hash table.
 * If key already exists, overwrite the old value with the new one.
 * If load factor is exceeded, allocate a new array with twice the number of buckets
 * (computing new bucket locations for each association).
 * @runtime O(1) if num buckets doesn't change, O(B) if it does
 */
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
    // check if this association will exceed the current load factor
    if (1.0 * (m_numItems + 1) / m_numBuckets > m_maxLoadFactor) {
        reassociate();
    }
    
    // we check reassociation first for a TINY (essentially negligible) time save
    addToTable(m_hashTable, key, value);
}

/**
 * Finds an item in the hash table with given key.
 * @return pointer to value associated with key if found
 * @return nullptr if not found
 * @runtime O(1)
 */
template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const {
    return findInTable(m_hashTable, key);
}

/**
 * @return the number of unique associations in the hash table.
 */
template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const {
    return m_numItems;
}

/**
 * @return current load factor of the hash table.
 * @runtime O(1)
 */
template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const {
    return m_numItems / m_numBuckets;
}

// Private member variables

/**
 * Frees all memory associated with the current hash table.
 * @runtime O(B)
 */
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::cleanup() {
    // delete all linked lists in each bucket
    for (int i = 0; i < m_numBuckets; i++) { // O(B)
        Node* cur = m_hashTable[i];
        while (cur != nullptr) { // O(1) assuming relatively distributed hash table
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }
    
    // delete table
    delete[] m_hashTable;
}

/**
 * Deletes current hash table and constructs a new one with specified number of buckets.
 */
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset(int buckets) {
    cleanup();
    m_hashTable = new Node*[buckets];
    m_numBuckets = buckets;
    m_numItems = 0;
}

/**
 * Given a key, return the hashed version of that key (modulo number of buckets).
 * @return hashed version of key
 */
template<typename KeyType, typename ValueType>
unsigned int MyHash<KeyType, ValueType>::getHashedNum(const KeyType& key) const {
    unsigned int hash(const KeyType& k);
    return hash(key) % m_numBuckets;
}

/**
 * Finds an item in the specified hash table with given key.
 * @runtime O(1)
 */
template<typename KeyType, typename ValueType>
ValueType* MyHash<KeyType, ValueType>::findInTable(Node** table, const KeyType& key) const {
    unsigned int h = getHashedNum(key);
    
    // If hashed to an empty location in the table, return nullptr
    if (table[h] == nullptr) return nullptr;
    Node* p = table[h];
    while (p != nullptr) { // otherwise loop through the linked list: technically O(X) but not really
        if (p->key == key) return &p->value; // if we find the key, return the value
        p = p->next;
    }
    return nullptr; // if we didn't find the key, return nullptr
}

/**
 * Adds a key-value pairing to the given hash table.
 * @runtime O(1)
 */
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::addToTable(Node** table, const KeyType& key, const ValueType& value) {
    ValueType* v = findInTable(table, key); // find() is also O(1)
    if (v != nullptr) { // key was found; overwrite old value
        *v = value;
    } else { // key wasn't found; create new Node
        unsigned int h = getHashedNum(key);
        
        Node* p = table[h]; // start of linked list
        Node* newNode = new Node;
        newNode->key = key;
        newNode->value = value;
        newNode->next = p; // add to start of linked list for time efficiency
        table[h] = newNode;
        
        m_numItems++;
    }
}

/**
 * Allocates a new hash table with double the bucket size and
 * reassociate every association to fit in the new bucket.
 * @runtime O(B)
 */
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reassociate() {
    
    // initalize a temp table to hold our new hash table
    Node** tempTable = new Node*[m_numBuckets * 2];
    
    for (int i = 0; i < m_numBuckets * 2; i++) { // O(B)
        tempTable[i] = nullptr; // initalize each bucket in the temp table to nullptr
    }
    
    for (int i = 0; i < m_numBuckets; i++) { // O(B)
        Node* p = m_hashTable[i];
        while (p != nullptr) { // loop through each linked list: O(1) assuming even dist.
            addToTable(tempTable, p->key, p->value); // O(1)
            p = p->next;
        }
    }
    
    // addToTable() still increments m_numItems, which means we need to divide by 2 to fix it
    m_numItems /= 2;
    
    // destroy the original hash table and set it to our temp table
    cleanup();
    m_hashTable = tempTable;
    
    // set the new number of buckets
    m_numBuckets *= 2;
}

#endif /* MYHASH_H */
