#include <iostream>
#include <thread>
#include <syncstream>

int main()
{
    std::cout << "Hello World!\n";

    auto printIsWorking = []() {
        std::cout << "Thread: " << std::this_thread::get_id() << " is working. \n";
        };

    std::thread t1(printIsWorking);
    std::thread t2(printIsWorking);
    std::thread t3(printIsWorking);
    std::thread t4(printIsWorking);
    std::thread t5(printIsWorking);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    std::cout << "End of normal thread cout \n";


    std::jthread tj1(printIsWorking);
    std::jthread tj2(printIsWorking);
    std::jthread tj3(printIsWorking);
    std::jthread tj4(printIsWorking);
    std::jthread tj5(printIsWorking);

    // join not needed, automatical join in jthread destructor. However want them to end before osync ones

    tj1.join();
    tj2.join();
    tj3.join();
    tj4.join();
    tj5.join();

    std::cout << "End of jthread cout \n" << std::endl;

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
