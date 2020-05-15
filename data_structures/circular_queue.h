#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H
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
template <typename T> class CircularQueue {
private:
    unsigned int maxSize;
    unsigned int size;
    int front;
    int rear;
    T *storage;

public:
    /*!
     * @brief Construct a new Circular Queue object
     *
     */
    CircularQueue() {
        maxSize = 50;
        size = 0;
        front = 0;
        rear = 0;
        storage = new T[maxSize];
    }

    /*!
     * @brief Construct a new Circular Queue object
     *
     * @param maxSize the maximum number of elements in the queue
     */
    CircularQueue(int maxSize) {
        this->maxSize = maxSize;
        size = 0;
        front = 0;
        rear = 0;
        storage = new T[maxSize];
    }

    /*!
     * @brief Destroy the Circular Queue object
     *
     */
    ~CircularQueue() { delete[] storage; }

    /*!
     * @brief Checks if the queue is empty
     *
     * @return true if it is
     * @return false if it is not
     */
    bool empty() const { return size == 0; }

    /*!
     * @brief Places the data on the queue will overwrite data at back
     * @param elem the data to place
     */
    void push(T elem) {
        storage[rear] = elem;
        // Move rear along queue and looped back around if neccessary
        rear++;
        rear %= maxSize;
        // increase size, but cap at maxSize
        if (size == maxSize) {
            // if size == maxSize then increment front and loop back if
            // neccessary
            front++;
            front %= maxSize;
        } else {
            size++;
        }
    }

    /*!
     * @brief Looks at the top element does not pop()
     *
     * @param elem a pass by reference variable to place the top in
     * @return true queue not empty
     * @return false queue empty
     */
    bool peek(T &elem){
        if (empty()) {
            return false;
        }
        elem = storage[front];
        return true;
    }

    /*!
     * @brief Returns the top element on the queue and pops
     *
     * @param elem a pass by reference variable to place the top in
     * @return true queue not empty
     * @return false queue empty
     */
    bool pop(T &elem) {
        if (empty()) {
            return false;
        }
        elem = storage[front];
        // Move front back and wrap
        front++;
        front %= maxSize;
        // decrement size
        size--;
        return true;
    }

    /*!
     * @brief pops the top element from the queue
     *
     * @return true queue not empty
     * @return false queue empty
     */
    bool pop() {
        if (empty()) {
            return false;
        }
        // Move front back and wrap
        front++;
        front %= maxSize;
        // decrement size
        size--;
        return true;
    }
};

#endif
