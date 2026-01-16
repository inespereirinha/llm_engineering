
#include <chrono>
#include <cstdio>
#include <cstdint>

int main() {
    using clock = std::chrono::steady_clock;

    const auto start_time = clock::now();

    constexpr uint32_t ITER = 200'000'000u;

    double result = 1.0;
    double d = 3.0;                  // 4*i - 1 for i=1
    constexpr double step = 4.0;
    constexpr double plus_offset = 2.0; // (4*i + 1) = (4*i - 1) + 2

    // Unrolled loop (8x); ITER is divisible by 8.
    for (uint32_t blocks = ITER / 8; blocks; --blocks) {
        result -= 1.0 / d; result += 1.0 / (d + plus_offset); d += step;
        result -= 1.0 / d; result += 1.0 / (d + plus_offset); d += step;
        result -= 1.0 / d; result += 1.0 / (d + plus_offset); d += step;
        result -= 1.0 / d; result += 1.0 / (d + plus_offset); d += step;
        result -= 1.0 / d; result += 1.0 / (d + plus_offset); d += step;
        result -= 1.0 / d; result += 1.0 / (d + plus_offset); d += step;
        result -= 1.0 / d; result += 1.0 / (d + plus_offset); d += step;
        result -= 1.0 / d; result += 1.0 / (d + plus_offset); d += step;
    }

    const double final_result = result * 4.0;

    const auto end_time = clock::now();
    const double seconds = std::chrono::duration<double>(end_time - start_time).count();

    std::printf("Result: %.12f\n", final_result);
    std::printf("Execution Time: %.6f seconds\n", seconds);
    return 0;
}
