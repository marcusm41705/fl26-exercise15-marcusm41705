// thread_safe_vector.hpp: starter code -- please refer to in-class example for ThreadSafeQueue
#ifndef THREAD_SAFE_VECTOR_HPP
#define THREAD_SAFE_VECTOR_HPP

#include <vector>
#include <mutex>
#include <condition_variable>
#include <utility>
template<typename T>
class ThreadSafeVector{
    private:
    std::vector<T> data_vec;
    mutable std::mutex the_mutex;
    std::condition_variable condition;
    public:
    ThreadSafeVector() = default;
    ThreadSafeVector& operator=(const ThreadSafeVector&) = delete;
    void push_back(const T& value){
        {
        std::lock_guard<std::mutex> lock(the_mutex);
        data_vec.push_back(value);
        }
        condition.notify_one();
    }
    void wait_and_pop_back(T& value){
        std::unique_lock<std::mutex> lock(the_mutex);
        condition.wait(lock, [this]{
            return !data_vec.empty();
        });
        value = std::move(data_vec.back());
        data_vec.pop_back();
    }
    bool empty() const{
        std::lock_guard<std::mutex> lock(the_mutex);
        return data_vec.empty();
    }
    std::size_t size() const{
                std::lock_guard<std::mutex> lock(the_mutex);
        return data_vec.size();
    } 
    
};

#endif