/**
 * @file safe_hash_map.h
 * @author Kaushik Basu
 * @brief A concurrent thread-safe hash map implemented in C++. Anyone can use the hash-map by simply including the header files in the "inc" folder. Currently, copy and move of the hash is not supported.
 *        The hash map is implemented as an array of hash buckets. Each hash bucket is a single linked list of hash nodes and has a mutex associated with it. Multiple threads can read from the same bucket simulatenously, but only one thread can write into the same bucket. Since the mutex is per bucket, if multiple threads try to write into different buckets simulatenously, they will be allowed to do so. This allows some level of concurrency for the hash map.
 *        The mutex is implemented as "std::shared_timed_mutex" from C++14 and uses "std::unique_lock" from C++11 for writes and "std::shared_lock" from C++14 for reading from a bucket.
 */

#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#include <cstdint> 
#include <iostream> 
#include <functional>
#include <mutex> 
#include "safe_hash_node.h" 

constexpr size_t HASH_SIZE_DEFAULT = 1031; // A prime number as hash size gives a better distribution of values in buckets

//The class represting the hash map.
//It is expected for user defined types, the hash function will be provided.
//By default, the std::hash function will be used
//If the hash size is not provided, then a defult size of 1031 will be used
//The hash table itself consists of an array of hash buckets.
//Each hash bucket is implemented as singly linked list with the head as a dummy node created 
//during the creation of the bucket. All the hash buckets are created during the construction of the map.
//Locks are taken per bucket, hence multiple threads can write simultaneously in different buckets in the hash map
template <typename K, typename V, typename F = std::hash<K> >
class SafeHashMap
{
    public:
        SafeHashMap(size_t hashSize_ = HASH_SIZE_DEFAULT) : hashSize(hashSize_)
        {
            hashTable = new HashBucket<K, V>[hashSize]; //create the hash table as an array of hash buckets
        }

        ~SafeHashMap()
        {
            delete [] hashTable;
        }
        //Copy and Move of the HashMap are not supported at this moment
        SafeHashMap(const SafeHashMap&) = delete;
        SafeHashMap(SafeHashMap&&) = delete;
        SafeHashMap& operator=(const SafeHashMap&) = delete;  
        SafeHashMap& operator=(SafeHashMap&&) = delete;
    
        //Function to find an entry in the hash map matching the key.
        //If key is found, the corresponding value is copied into the parameter "value" and function returns true.
        //If key is not found, function returns false.
        bool find(const K &key, V &value) const 
        {
            size_t hashValue = hashFn(key) % hashSize ;
            return hashTable[hashValue].find(key, value);
        }

        //Function to insert into the hash map.
        //If key already exists, update the value, else insert a new node in the bucket with the <key, value> pair.
        void insert(const K &key, const V &value) 
        {
            size_t hashValue = hashFn(key) % hashSize ;
            hashTable[hashValue].insert(key, value);
        }

        //Function to remove an entry from the bucket, if found
        void erase(const K &key) 
        {
            size_t hashValue = hashFn(key) % hashSize ;
            hashTable[hashValue].erase(key);
        }   

        //Function to clean up the hasp map, i.e., remove all entries from it
        void clear()
        {
            for(size_t i = 0; i < hashSize; i++)
            {
                (hashTable[i]).clear();
            }
        }   

    private:
        HashBucket<K, V> * hashTable;
        F hashFn;
        const size_t hashSize;
};

#endif

