#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template <typename T>
class SafeQueue {
   public:
    // constructor
    SafeQueue() {}
    SafeQueue(const SafeQueue& other) {
        std::lock_guard<std::mutex> lock{other.mutex_};
        queue_ = other.queue;
    }

    void Push(const T& new_value) {
        std::lock_guard<std::mutex> lock{mutex_};
        queue_.push(new_value);
        con_var_.notify_one();
    }

    void Push(T&& new_value) {
        std::lock_guard<std::mutex> lock{mutex_};
        queue_.emplace(std::forward<T>(new_value));
        con_var_.notify_one();
    }

    void WaitAndPop(T& value) {
        std::unique_lock<std::mutex> lock{mutex_};
        con_var_.wait(lock, [this] { return !queue_.empty(); });
        value = queue_.front();
        queue_.pop();
    }

    std::shared_ptr<T> WaitAndPop() {
        std::unique_lock<std::mutex> lock{mutex_};
        con_var_.wait(lock, [this] { return !queue_.empty(); });
        std::shared_ptr<T> res{std::make_shared<T>(queue_.front())};
        queue_.pop();
        return res;
    }

    bool TryPop(T& value) {
        std::lock_guard<std::mutex> lock{mutex_};
        if (queue_.empty()) return false;
        value = queue_.front();
        queue_.pop();
        return true;
    }

    std::shared_ptr<T> TryPop() {
        std::lock_guard<std::mutex> lock{mutex_};
        if (queue_.empty()) return std::shared_ptr<T>{};
        std::shared_ptr<std::mutex> res{std::make_shared<T>(queue_.front())};
        queue_.pop();
        return res;
    }

    void PopToQueue(SafeQueue<T>& other) {
        std::lock_guard<std::mutex> guard(mutex_);
        while (!queue_.empty()) {
            other.push(queue_.front());
            queue_.pop();
        }
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock{mutex_};
        return queue_.empty();
    }

   private:
    mutable std::mutex mutex_;
    std::condition_variable con_var_;
    std::queue<T> queue_;
};