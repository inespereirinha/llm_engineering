
#include <cstdio>
#include <cstdint>
#include <chrono>

int main() {
    constexpr uint32_t iterations = 200000000u;

    auto start = std::chrono::steady_clock::now();

    double result = 1.0;

    // Denominators for i=1: (4*i - 1)=3, (4*i + 1)=5
    double d1 = 3.0;
    double d2 = 5.0;

    uint32_t n = iterations;

    // Unroll by 8; compute divisions ahead to increase ILP while preserving update order.
    while (n >= 8) {
        const double d1_0 = d1;
        const double d2_0 = d2;
        const double d1_1 = d1_0 + 4.0,  d2_1 = d2_0 + 4.0;
        const double d1_2 = d1_0 + 8.0,  d2_2 = d2_0 + 8.0;
        const double d1_3 = d1_0 + 12.0, d2_3 = d2_0 + 12.0;
        const double d1_4 = d1_0 + 16.0, d2_4 = d2_0 + 16.0;
        const double d1_5 = d1_0 + 20.0, d2_5 = d2_0 + 20.0;
        const double d1_6 = d1_0 + 24.0, d2_6 = d2_0 + 24.0;
        const double d1_7 = d1_0 + 28.0, d2_7 = d2_0 + 28.0;

        const double a0 = 1.0 / d1_0, b0 = 1.0 / d2_0;
        const double a1 = 1.0 / d1_1, b1 = 1.0 / d2_1;
        const double a2 = 1.0 / d1_2, b2 = 1.0 / d2_2;
        const double a3 = 1.0 / d1_3, b3 = 1.0 / d2_3;
        const double a4 = 1.0 / d1_4, b4 = 1.0 / d2_4;
        const double a5 = 1.0 / d1_5, b5 = 1.0 / d2_5;
        const double a6 = 1.0 / d1_6, b6 = 1.0 / d2_6;
        const double a7 = 1.0 / d1_7, b7 = 1.0 / d2_7;

        result -= a0; result += b0;
        result -= a1; result += b1;
        result -= a2; result += b2;
        result -= a3; result += b3;
        result -= a4; result += b4;
        result -= a5; result += b5;
        result -= a6; result += b6;
        result -= a7; result += b7;

        d1 = d1_0 + 32.0;
        d2 = d2_0 + 32.0;
        n -= 8;
    }

    while (n--) {
        result -= 1.0 / d1;
        result += 1.0 / d2;
        d1 += 4.0;
        d2 += 4.0;
    }

    result *= 4.0;

    auto end = std::chrono::steady_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();

    std::printf("Result: %.12f\n", result);
    std::printf("Execution Time: %.6f seconds\n", seconds);
    return 0;
}
