#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <list>

class SharedCache {

public:

    std::optional<std::string> get(const std::string& key) const {
        std::shared_lock lock(mtx_);
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void put(std::string key, std::string value) {
        std::unique_lock lock(mtx_);
        auto [it, inserted] = cache_.insert_or_assign(std::move(key), std::move(value));
        if (inserted) {
            cacheOrder_.push_back(it->first);
            if (cache_.size() > max_size) {
                auto oldestVal = cacheOrder_.front();
                cache_.erase(oldestVal);
                cacheOrder_.pop_front();
            }
        }
    }

    void remove(const std::string& key) {
        std::unique_lock lock(mtx_);
        cache_.erase(key);
        cacheOrder_.remove(key);
    }

    template<std::invocable Factory>
    std::string get_or_insert(const std::string& key, Factory factory) {
        {
            std::shared_lock lock(mtx_);
            auto it = cache_.find(key);
            if (it != cache_.end()) {
                return it->second;
            }
        }

        {
            std::string newValue = factory();

            std::unique_lock lock(mtx_);

            auto it = cache_.find(key);
            if (it != cache_.end()) {
                return it->second;
            }
        
            cache_.insert_or_assign(key, newValue);
            cacheOrder_.push_back(key);

            if (cache_.size() > max_size) {
                auto oldestVal = cacheOrder_.front();
                cache_.erase(oldestVal);
                cacheOrder_.pop_front();
            }

            return newValue;

        }
    }



private:
    std::map<std::string, std::string> cache_;
    std::list<std::string> cacheOrder_;
    mutable std::shared_mutex mtx_;
    static constexpr size_t max_size = 3;

};

class ExclusiveCache {

public:

    std::optional<std::string> get(const std::string& key) const{
        std::unique_lock lock(mtx_);
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void put(std::string key, std::string value) {
        std::unique_lock lock(mtx_);
        cache_.insert_or_assign(std::move(key), std::move(value));
    }

private:
    std::map<std::string, std::string> cache_;
    mutable std::mutex mtx_;
};

template<typename CacheType>
std::chrono::duration<double, std::milli> testReadsTime(CacheType& cache, int numThreads) {
    std::vector<std::jthread> jthreads;

    auto startTime = std::chrono::steady_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        jthreads.emplace_back([&, i]() { cache.get("key0"); });
    }

    for (auto& t : jthreads) {
        t.join();
    }

    return std::chrono::steady_clock::now() - startTime;
}

int main()
{
    SharedCache sC;

    //Testing max_size
    sC.put("a", "1");
    sC.put("b", "2");
    sC.put("c", "3");
    sC.put("d", "4");

    std::cout << sC.get("a").value_or("removed") << "\n"; // this should be removed
    std::cout << sC.get("b").value_or("removed") << "\n";
    std::cout << sC.get("c").value_or("removed") << "\n";
    std::cout << sC.get("d").value_or("removed") << "\n";

    //Testing get_or_insert template function

    //miss path
    auto firstRun = sC.get_or_insert("user1", []() {
        std::cout << "factory was used, inserting value for user1 \n";
        return "Alice";
        });
    std::cout << "user1: " << firstRun << "\n"; // Will print Alice

    //hit path
    auto secondRun = sC.get_or_insert("user1", []() {
        std::cout << "factory was used, inserting value for user1 \n"; // Won't print, since user1 is found already
        return "Bob";
        });
    std::cout << "user1: " << secondRun << "\n"; // Will still print Alice


    //Time comparison Shared vs Exclusive:
    ExclusiveCache eC;

    sC.put("key0", "val0");
    eC.put("key0","val0");

    auto sharedCacheTime = testReadsTime(sC, 20);
    auto exclusiveCacheTime = testReadsTime(eC, 20);

    std::cout << "shared mutex (concurrent reads): " << sharedCacheTime.count() << " ms \n";
    std::cout << "exclusive mutex (serialized reads): " << exclusiveCacheTime.count() << " ms \n";
}