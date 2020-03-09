/**
 * @file hash_node.h
 * @author Kaushik Basu
 * @brief A concurrent thread-safe hash map implemented in C++. Anyone can use the hash-map by simply including the header files in the "inc" folder. Currently, copy and move of the hash is not supported.
 *        The hash map is implemented as an array of hash buckets. Each hash bucket is a single linked list of hash nodes and has a mutex associated with it. Multiple threads can read from the same bucket simulatenously, but only one thread can write into the same bucket. Since the mutex is per bucket, if multiple threads try to write into different buckets simulatenously, they will be allowed to do so. This allows some level of concurrency for the hash map.
 *        The mutex is implemented as "std::shared_timed_mutex" from C++14 and uses "std::unique_lock" from C++11 for writes and "std::shared_lock" from C++14 for reading from a bucket.
 * 
 */

#ifndef HASH_NODE_H_
#define HASH_NODE_H_

#include <shared_mutex>

// Class representing a templatized hash node
template <typename K, typename V>
class SafeHashNode
{
    public:
        SafeHashNode() : next(nullptr)
        {}
        SafeHashNode(K key_, V value_) : next(nullptr), key(key_), value(value_)
        {}
        ~SafeHashNode() 
        {
            next = nullptr;
        }

        const K& getKey() const {return key;}
        void setValue(V value_) {value = value_;}
        const V& getValue() const {return value;}

        SafeHashNode *next; //Pointer to the next node in the same bucket
    private:
        K key;   //the hash key
        V value; //the value corresponding to the key
};


//Class representing a hash bucket. The bucket is implemented as a singly linked list.
//A bucket is always constructed with a dummy head node
template <typename K, typename V>
class HashBucket
{
    public:
        HashBucket() : head(nullptr)
        {}

        ~HashBucket() //delete the bucket
        {
            clear();
        }   

        //Function to find an entry in the bucket matching the key
        //If key is found, the corresponding value is copied into the parameter "value" and function returns true.
        //If key is not found, function returns false
        bool find(const K &key, V &value) const
        {
            // A shared mutex is used to enable mutiple concurrent reads
            std::shared_lock<std::shared_timed_mutex> lock(mutex_); 
            SafeHashNode<K, V> * node = head;

            while (node != nullptr)
            {
                if (node->getKey() == key)
                {
                    value = node->getValue();
                    return true;
                }
                node = node->next;
            }
            return false;
        }

        //Function to insert into the bucket
        //If key already exists, update the value, else insert a new node in the bucket with the <key, value> pair
        void insert(const K &key, const V &value)
        {
            //Exclusive lock to enable single write in the bucket
            std::unique_lock<std::shared_timed_mutex> lock(mutex_);
            SafeHashNode<K, V> * prev = nullptr;
            SafeHashNode<K, V> * node = head;

            while (node != nullptr && node->getKey() != key)
            {
                prev = node;
                node = node->next;
            }

            if (nullptr == node) //New entry, create a node and add to bucket
            {
                if(nullptr == head)
                {
                    head = new SafeHashNode<K, V>(key, value);
                }
                else
                {
                    prev->next = new SafeHashNode<K, V>(key, value);                 
                }
            }
            else
            {
                node->setValue(value); //Key found in bucket, update the value
            }
    
        }

        //Function to remove an entry from the bucket, if found
        void erase(const K &key)
        {
            //Exclusive lock to enable single write in the bucket
            std::unique_lock<std::shared_timed_mutex> lock(mutex_);
            SafeHashNode<K, V> *prev  = nullptr;
            SafeHashNode<K, V> * node = head;

            while (node != nullptr && node->getKey() != key)
            {
                prev = node;
                node = node->next;
            }

            if (nullptr == node) //Key not found, nothing to be done
            {
                return;
            }
            else  //Remove the node from the bucket
            {
                if(head == node)
                {
                    head = node->next;
                }
                else
                {
                    prev->next = node->next; 
                }
                delete node; //Free up the memory
            }
        }

        //Function to clear the bucket
        void clear()
        {
            //Exclusive lock to enable single write in the bucket
            std::unique_lock<std::shared_timed_mutex> lock(mutex_);
            SafeHashNode<K, V> * prev = nullptr;
            SafeHashNode<K, V> * node = head;
            while(node != nullptr)
            {
                prev = node;
                node = node->next;
                delete prev;
            }
            head = nullptr;
        }

    private:
        SafeHashNode<K, V> * head; //The head node of the bucket
        mutable std::shared_timed_mutex mutex_; //The mutex for this bucket
};

#endif

