#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mutex;
std::condition_variable cv;
static bool ready = false;

void increment(int& x) {
    for (int i = 0; i < 1000; i++) {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, []{ return ready; }); // чекаємо, доки обидва потоки будуть готові
        if (x < 1000) {
            x++;
        }
        cv.notify_all(); // повідомляємо інші потоки, що ми закінчили ітерацію
    }
}

int main() {
    int x = 0;
    std::thread t1(increment, std::ref(x));
    std::thread t2(increment, std::ref(x));

    // чекаємо, доки обидва потоки будуть готові
    {
        std::unique_lock<std::mutex> lock(mutex);
        ready = true;
    }
    cv.notify_all(); // повідомляємо потоки, що вони можуть починати роботу

    t1.join();
    t2.join();

    std::cout << "Result: " << x << std::endl;

    return 0;
}
