#include <cstdio>
#include <cstdint>
#include <chrono>

static inline double calculate(int64_t iterations, int64_t param1, int64_t param2) {
    double result = 1.0;
    int64_t j = param1 - param2;        // initial j = i*param1 - param2 with i=1
    const int64_t step = param1;        // increment of j each iteration
    const int64_t delta = 2 * param2;   // difference between the two denominators in one iteration

    int64_t i = 1;
    // Manual unroll by 8 while preserving exact operation order
    for (; i + 7 <= iterations; i += 8) {
        // 1
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
        // 2
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
        // 3
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
        // 4
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
        // 5
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
        // 6
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
        // 7
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
        // 8
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
    }

    for (; i <= iterations; ++i) {
        result -= 1.0 / static_cast<double>(j);
        result += 1.0 / static_cast<double>(j + delta);
        j += step;
    }

    return result;
}

int main() {
    const int64_t iterations = 200000000;
    const int64_t param1 = 4;
    const int64_t param2 = 1;

    auto start = std::chrono::steady_clock::now();
    double result = calculate(iterations, param1, param2) * 4.0;
    auto end = std::chrono::steady_clock::now();

    double elapsed = std::chrono::duration<double>(end - start).count();

    std::printf("Result: %.12f\n", result);
    std::printf("Execution Time: %.6f seconds\n", elapsed);
    return 0;
}