#include <iostream>
#include <thread>
#include <chrono>
#include <condition_variable>

using namespace std::chrono_literals;

std::mutex mutex_;
std::condition_variable condVar;
bool ready = false;

void workerFunction() {
    std::cout << "Worker: waiting for start signal \n";
    std::this_thread::sleep_for(1s);
    std::cout << "Worker: received start signal, doing work \n";
}

int main()
{

    std::cout << "Main: Starting with creating workerThread \n";

    //Spawn worker thread
    std::thread workerThread(workerFunction);

    //Worker blocks
    std::cout << "Main: preparing... \n";
    std::this_thread::sleep_for(1s);

    //Main thread sleep
    std::this_thread::sleep_for(1s);

    //Main thread sends signal
    std::cout << "Main: sending start signal \n";

    //Worker proceeds

    //Main thread joins the worker
    workerThread.join();

    std::cout << "Main: worker finished. exiting \n";
}
