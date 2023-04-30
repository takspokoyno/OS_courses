//g++ -pthread fast_racer.cpp -o fast_racer

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

const int iterations = 100000000;

void with_atomic(std::atomic<int> &v) {
    for (int i = 0; i < iterations; i++) {
        v.fetch_add(1);
    }
}

int main() {
    std::atomic<int> v = {0};
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1(with_atomic, std::ref(v));
    std::thread t2(with_atomic, std::ref(v));
    t1.join();
    t2.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "With atomic = " << v << "; time: " << elapsed.count() << "s" << std::endl;
    return 0;
}