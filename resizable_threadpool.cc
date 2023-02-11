#include <thread>
#include <condition_variable>
#include <vector>
#include <functional>
#include <queue>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <unordered_set>

class ThreadPool {
public:
    ThreadPool(size_t s) {
        for (size_t i = 0; i < s; ++i)
            threads_.emplace_back(std::thread([this] {Loop();}));
    }
    void AddJob(std::function<void()>&& f) {
        if (threads_.empty())
            return;
        {
            std::lock_guard<std::mutex> g(m_);
            queue_.emplace(std::move(f));
        }
        cv_.notify_one();
    }

    void WaitAllDone() {
        std::unique_lock<std::mutex> l(m_);
        if (queue_.empty())
            return;
        queue_empty_.wait(l, [this] { return queue_.empty(); });
    }

    ~ThreadPool() {
        downscale(0);
    }

    void resize(size_t target) {
        std::cout << "RESIZE " << threads_.size() << " -> " << target << "\n";
        upscale(target);
        downscale(target);
    }

    auto size() const { return threads_.size(); }

private:
    void downscale(size_t target) {
        if (target >= threads_.size())
            return;
        {
            std::lock_guard<std::mutex> g(m_);
            for (size_t i = target; i < threads_.size(); ++i)
                shutdown_threads_.emplace(threads_[i].get_id());
        }
        cv_.notify_all();
        for (size_t i = target; i < threads_.size(); ++i)
            threads_[i].join();
        threads_.resize(target);
    }
    
    void upscale(size_t target) {
        for (size_t i = threads_.size(); i < target; ++i)
            threads_.emplace_back(std::thread([this] {Loop();}));
    }

    void Loop() {
        std::unique_lock<std::mutex> l(m_);
        for (;;) {
            cv_.wait(l, [this] { return !queue_.empty() || shutdown_threads_.count(std::this_thread::get_id()); });
            if (shutdown_threads_.erase(std::this_thread::get_id()))
                return;
            auto cb = std::move(queue_.front()); queue_.pop();
            l.unlock();
            cb();
            l.lock();
            if (queue_.empty())
                queue_empty_.notify_one();
        }
    }
    std::mutex m_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> queue_;
    std::unordered_set<std::thread::id> shutdown_threads_;
    std::vector<std::thread> threads_;
    std::condition_variable queue_empty_;
};



int main() {
    ThreadPool tp(5);
    for (int i = 0; i < 20; ++i) {
        switch (i) {
            case 2: tp.resize(7); break;
            case 5: tp.resize(3); break;
        }
        tp.AddJob([i] {
            int s = rand() % 100;
            std::this_thread::sleep_for(std::chrono::milliseconds(s));
            std::cout << "t" << std::this_thread::get_id() << ": " << i << "; slept for " << s << "ms\n";
        });
    }
    tp.WaitAllDone();
    return 0;
}
