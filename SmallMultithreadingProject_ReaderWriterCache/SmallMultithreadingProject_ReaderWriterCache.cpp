#include <iostream>
#include <map>
#include <optional>
#include <string>

class Cache {

public:

    std::optional<std::string> get(const std::string& key) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void put(std::string key, std::string value) {
        cache_.insert_or_assign(std::move(key),std::move(value));
    }

    void remove(const std::string& key) {
        cache_.erase(key);
    }



private:
    std::map<std::string, std::string> cache_;

};

int main()
{
    Cache c;

    c.put("name", "Alice");
    std::cout << c.get("name").value_or("not found") << "\n";

    c.put("name", "Bob");
    std::cout << c.get("name").value_or("not found") << "\n";

    c.remove("name");
    std::cout << c.get("name").value_or("not found") << "\n";

}