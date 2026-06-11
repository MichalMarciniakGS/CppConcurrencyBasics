#include <iostream>
#include <thread>
#include <syncstream>
#include <vector>

int main()
{
    std::cout << "Hello World!\n";

    auto printIsWorking = []() {
        std::cout << "Thread: " << std::this_thread::get_id() << " is working. \n";
        };

    std::vector<std::thread> threads;
    for (size_t i = 0; i < 5; i++) {
        threads.emplace_back(printIsWorking);
    }
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "End of normal thread cout \n\n";


    // join not needed, automatical join in jthread destructor. However want them to end before osync ones

    std::vector<std::jthread> jthreads;
    for (size_t i = 0; i < 5; i++) {
        jthreads.emplace_back(printIsWorking);
    }
    for (auto& jt : jthreads) {
        jt.join();
    }

    std::cout << "End of jthread cout \n\n";

    auto osyncPrintIsWorking = []() {
        std::osyncstream(std::cout) << "Osync! Thread: " << std::this_thread::get_id() << " is working. \n";
        };

    std::jthread tj10(osyncPrintIsWorking);
    std::jthread tj11(osyncPrintIsWorking);
    std::jthread tj12(osyncPrintIsWorking);
    std::jthread tj13(osyncPrintIsWorking);
    std::jthread tj14(osyncPrintIsWorking);
    std::jthread tj15(osyncPrintIsWorking);
    std::jthread tj16(osyncPrintIsWorking);
    std::jthread tj17(osyncPrintIsWorking);


}
