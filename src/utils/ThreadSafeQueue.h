#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace utils {

/**
 * @brief Thread-safe queue for inter-thread communication
 * @tparam T Data type to store in queue
 */
template<typename T>
class ThreadSafeQueue {
public:
    explicit ThreadSafeQueue(size_t capacity = 0)
    : capacity_(capacity) {}

    /**
     * Push an item to the queue
     * @param value Item to push
     */
    void push(const T& value) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if(queue_.size() >= capacity_ && capacity_ > 0) {
                // pop the oldest item to make space
                queue_.pop();
            }
            queue_.push(value);
        }
        cond_var_.notify_one();
    }

    /**
     * Push an item to the queue (move semantics)
     * @param value Item to push
     */
    void push(T&& value) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if(queue_.size() >= capacity_ && capacity_ > 0) {
                // pop the oldest item to make space
                queue_.pop();
            }
            queue_.push(std::move(value));
        }
        cond_var_.notify_one();
    }

    /**
     * Try to pop an item from the queue (non-blocking)
     * @param value Reference to store the popped value
     * @return true if an item was popped, false if queue is empty
     */
    bool tryPop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    /**
     * Wait and pop an item from the queue (blocking)
     * @param value Reference to store the popped value
     */
    void waitAndPop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this] { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
    }

    /**
     * Get the size of the queue
     * @return Current queue size
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    /**
     * Check if queue is empty
     * @return true if queue is empty, false otherwise
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    /**
     * Clear the queue
     */
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!queue_.empty()) {
            queue_.pop();
        }
    }

private:
    size_t capacity_{0};
    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
    std::queue<T> queue_;
};

}  // namespace utils

#endif  // THREAD_SAFE_QUEUE_H
