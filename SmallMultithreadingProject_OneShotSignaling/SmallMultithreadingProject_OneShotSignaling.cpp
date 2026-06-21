#include <iostream>
#include <thread>
#include <chrono>
#include <future>

using namespace std::chrono_literals;

void workerFunction(std::future<void> startFuture) {
    std::cout << "Worker: waiting for start signal\n";
    startFuture.wait();
    std::cout << "Worker: received start signal, doing work\n";
}

int main()
{
    std::promise<void> startSignal;
    std::future<void> startFuture = startSignal.get_future();

    std::cout << "Main: Starting with creating workerThread \n";

    std::cout << "Main: creating worker thread\n";
    std::thread workerThread(workerFunction, std::move(startFuture));

    std::cout << "Main: preparing...\n";
    std::this_thread::sleep_for(1s);

    std::cout << "Main: sending start signal\n";
    startSignal.set_value();

    workerThread.join();
    std::cout << "Main: worker finished. exiting\n";
}
