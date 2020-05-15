#ifndef SAFECIRCULARQUEUE_H
#define SAFECIRCULARQUEUE_H
#include "circular_queue.h"
#include <iostream>
#include <mutex>

/*!
 * @brief This is a thread safe circular queue with some special behaviour
 * data is added to the back until size == maxSize when that happens
 *    new data overwrites the oldest data in the queue (that at the front)
 *    and the front pointer moves along accordingly
 * data is returned in a first in first out manner however if the queue
 *    gets full it might not be the earliest earliest as that was
 *    overriden
 *
 * @tparam T
 */
template <typename T> class SafeCircularQueue {
private:
    CircularQueue<T> storage;
    std::mutex m;

public:
    /*!
     * @brief Construct a new Safe Circular Queue object
     *
     */
    SafeCircularQueue() {}

    /*!
     * @brief Construct a new Safe Circular Queue object
     *
     * @param maxSize the maximum number of elements in the queue
     */
    SafeCircularQueue(int maxSize) :storage(maxSize){}

    /*!
     * @brief Destroy the Safe Circular Queue object
     *
     */
    ~SafeCircularQueue() {}

    /*!
     * @brief Checks if the queue is empty
     *
     * @return true if it is
     * @return false if it is not
     */
    bool empty() { 
        std::lock_guard<std::mutex> lock(m);
        return storage.empty(); 
    }

    /*!
     * @brief Places the data on the queue will overwrite data at back
     * @param elem the data to place
     */
    void push(T elem) {
        std::lock_guard<std::mutex> lock(m);
        storage.push(elem);
    }

    /*!
     * @brief Looks at the top element does not pop()
     *
     * @param elem a pass by reference variable to place the top in
     * @return true queue not empty
     * @return false queue empty
     */
    bool peek(T &elem) {
        std::lock_guard<std::mutex> lock(m);
        return storage.peek(elem);
    }

    /*!
     * @brief Returns the top element on the queue and pops
     *
     * @param elem a pass by reference variable to place the top in
     * @return true queue not empty
     * @return false queue empty
     */
    bool pop(T &elem) {
        std::lock_guard<std::mutex> lock(m);
        return storage.pop(elem);
    }

    /*!
     * @brief pops the top element from the queue
     *
     * @return true queue not empty
     * @return false queue empty
     */
    bool pop() {
        std::lock_guard<std::mutex> lock(m);
        return storage.pop();
    }
};

#endif
