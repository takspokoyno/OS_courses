//g++ -pthread -o racers racers.cpp

#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>
#include <chrono>

const int iterations = 100000000;

void safe_increment(int &v, std::mutex &mtx) {
    for (int i = 0; i < iterations; i++) {
        mtx.lock();
        v++;
        mtx.unlock();
    }
}

void unsafe_increment(int &v) {
    for (int i = 0; i < iterations; i++) {
        v++;
    }
}

// ./racers type(1/2)
int main() {

    int v1 = 0;
    std::mutex mtx;
    auto start_time1 = std::chrono::high_resolution_clock::now();
    std::thread t1(safe_increment, std::ref(v1), std::ref(mtx));
    std::thread t2(safe_increment, std::ref(v1), std::ref(mtx));
    t1.join();
    t2.join();
    auto end_time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time1 = end_time1 - start_time1;
    std::cout << "With critical section:" << std::endl;
    std::cout << "v1 = " << v1 << "; time: " << elapsed_time1.count() << std::endl;
    std::cout << std::endl;

    int v2 = 0;
    auto start_time2 = std::chrono::high_resolution_clock::now();
    std::thread t3(unsafe_increment, std::ref(v2));
    std::thread t4(unsafe_increment, std::ref(v2));
    t3.join();
    t4.join();
    auto end_time2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time2 = end_time2 - start_time2;
    std::cout << "Without critical section:" << std::endl;
    std::cout << "v2 = " << v2 << "; time: " << elapsed_time2.count() << std::endl;
    std::cout << std::endl;

    return 0;    
}
