#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H
#include <mutex>
#include <queue>

#include <iostream>

template <typename T> class SafeQueue {
private:
    std::queue<T> q;
    std::mutex m;

public:
    SafeQueue() {}

    /*! Places the data on the queue
     * @param elem the data to place
     */
    void push(T elem) {
        std::lock_guard<std::mutex> lock(m);
        q.push(elem);
    }

    /*! Looks at the top element does not pop()
     *
     * @param elem a pass by reference variable to place the top in
     * @return true queue not empty
     * @return false queue empty
     */
    bool peek(T &elem) {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) {
            return false;
        }
        elem = q.front();
        return true;
    }

    /*! Returns the top element on the queue and pops
     *
     * @param elem a pass by reference variable to place the top in
     * @return true queue not empty
     * @return false queue empty
     */
    bool pop(T &elem) {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) {
            return false;
        }
        elem = q.front();
        q.pop();
        return true;
    }
};

#endif