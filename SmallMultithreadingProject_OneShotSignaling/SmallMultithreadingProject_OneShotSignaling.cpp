#include <iostream>
#include <thread>
#include <chrono>
#include <condition_variable>

using namespace std::chrono_literals;

std::mutex mtx;
std::condition_variable condVar;
bool ready = false;

void workerFunction() {
    std::cout << "Worker: waiting for start signal \n";

    std::unique_lock<std::mutex> lock(mtx);
    condVar.wait(lock, []() {return ready;});

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

    //Main thread sends signal
    std::cout << "Main: sending start signal \n";
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }

    //Worker proceeds
    condVar.notify_one();

    //Main thread joins the worker
    workerThread.join();

    std::cout << "Main: worker finished. exiting \n";
}
