#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <shared_mutex>
#include <vector>
#include <thread>
#include <syncstream>

class Cache {

public:

    std::optional<std::string> get(const std::string& key) const {
        std::shared_lock lock(mtx_);
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void put(std::string key, std::string value) {
        std::unique_lock lock(mtx_);
        cache_.insert_or_assign(std::move(key),std::move(value));
    }

    void remove(const std::string& key) {
        std::unique_lock lock(mtx_);
        cache_.erase(key);
    }



private:
    std::map<std::string, std::string> cache_;
    mutable std::shared_mutex mtx_;

};

int main()
{
    Cache c;
    
    std::vector<std::jthread> jthreads;
    for (size_t i{}; i < 5; ++i) {
        jthreads.emplace_back([&,i]() {
            c.put( "key" + std::to_string(i), "val" + std::to_string(i));
            std::osyncstream(std::cout) << "Current writer thread id is: " << std::this_thread::get_id() << "\n";
            });
    }

    for (auto& t : jthreads) {
        t.join();
    }
    jthreads.clear();

    for (size_t i{}; i < 6; ++i) {
        jthreads.emplace_back([&,i]() {
            const auto currVal = c.get("key" + std::to_string(i));
            std::osyncstream(std::cout) << "Current reader thread id is: " << std::this_thread::get_id() << ", it's value is: " << currVal.value_or("not found") << "\n";
            });
    }

}