//g++ -pthread matrix.cpp -o matrix

#include <iostream>
#include <vector>
#include <mutex>
#include <fstream>
#include <thread>
#include <chrono>

namespace {
    constexpr size_t maxElem{100};

    template<typename T>
    using Matrix = std::vector<std::vector<T>>;

    std::mutex cout_mtx{};

    Matrix<int> generate_rand_matrix(size_t rows, size_t columns) {
        Matrix<int> res(rows, std::vector<int>(columns));
        for (auto &row: res) {
            for (auto &el: row) {
                el = std::rand() % maxElem;
            }
        }
        return res;
    }

    void print_matrix(const Matrix<int> &matrix) {
        for (const auto &row: matrix) {
            for (const auto &el: row) {
                std::lock_guard<std::mutex> lck{cout_mtx};
                std::cout << el << " ";
            }
            std::cout << "\n";
        }
    }

    void compute_el(int &res, const Matrix<int> &a, const Matrix<int> &b, size_t i, size_t j) {
        if (a[i].size() != b.size()) {
            exit(-2);
        }
        res = 0;
        size_t m = a[i].size();
        for (size_t cntr = 0; cntr < m; ++cntr) {
            res += a[i][cntr] * b[cntr][j];
        }

        {
            std::lock_guard<std::mutex> lck{cout_mtx};
            std::cout << "[" << i << ":" << j << "] computed\n";
        }
    }

    void compute_row(std::vector<int> &row, const Matrix<int> &a, const Matrix<int> &b, size_t i) {
        for (size_t j = 0; j < b[0].size(); ++j) {
            int sum = 0;
            for (size_t x = 0; x < a[i].size(); ++x) {
                sum += a[i][x] * b[x][j];
            }
            row[j] = sum;
            {
                std::lock_guard<std::mutex> lck{cout_mtx};
                std::cout << "[" << i << ":" << j << "] computed\n";
            }            
        }
    }
}

// ./matrix n m k computingtype(1/2/3)
int main(int argc, char **argv) {

    if (!(argc == 5)) {
        return -1;
    }

    Matrix<int> first;
    Matrix<int> second;

    size_t n = std::stoul(argv[1]);
    size_t m = std::stoul(argv[2]);
    size_t k = std::stoul(argv[3]);

    int type = std::stoul(argv[4]);

    first = generate_rand_matrix(n, m);
    second = generate_rand_matrix(m, k);

    print_matrix(first);
    std::cout << '\n';
    print_matrix(second);

    std::vector<std::thread> threads;

    Matrix<int> res(n, std::vector<int>(k));

    auto start_time = std::chrono::high_resolution_clock::now();

    switch(type){
        case 1:
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < k; ++j) {
                    threads.emplace_back(compute_el, std::ref(res[i][j]), first, second, i, j);
                }
            }
            for (auto &thrd: threads) {
                thrd.join();
            }
            std::cout << "\nfor_elem\n";
            break;

        case 2:
            for (size_t i = 0; i < n; ++i) {
                threads.emplace_back(compute_row, std::ref(res[i]), first, second, i);
            }
            for (auto &thrd: threads) {
                thrd.join();
            }
            std::cout << "\nfor_row\n";
            break;

        case 3:
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < k; ++j) {
                    compute_el(std::ref(res[i][j]), first, second, i, j);
                }
            }
            std::cout << "\nfor_matrix\n";
            break;

        default:
            return -1;            
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "Elapsed time: " << elapsed_time.count() << " s\n";

    print_matrix(res);

    return 0;
}
