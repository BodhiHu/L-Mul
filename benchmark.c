#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

// Function to approximate multiplication using L-Mul algorithm
float l_mul(float a, float b) {
    uint32_t a_bits = *((uint32_t*)&a);
    uint32_t b_bits = *((uint32_t*)&b);

    int sign_a = (a_bits >> 31) & 1;
    int exp_a = (a_bits >> 23) & 0xFF;
    int mant_a = a_bits & 0x7FFFFF;

    int sign_b = (b_bits >> 31) & 1;
    int exp_b = (b_bits >> 23) & 0xFF;
    int mant_b = b_bits & 0x7FFFFF;

    int sign_result = sign_a ^ sign_b;
    int exp_result = exp_a + exp_b - 127;  // Subtract the bias of 127 for float32

    int mant_result = (mant_a + mant_b) >> 1;  // Simple approximation of mantissa addition

    uint32_t result_bits = (sign_result << 31) | ((exp_result & 0xFF) << 23) | (mant_result & 0x7FFFFF);

    float result = *((float*)&result_bits);
    return result;
}

int main() {
    float a = 1.75f;
    float b = 2.5f;
    const int iterations = 1000000;  // Number of iterations to test

    clock_t start, end;
    float result = 0.0f;

    // Measure time for L-Mul
    start = clock();
    for (int i = 0; i < iterations; i++) {
        result = l_mul(a, b);
    }
    end = clock();
    double l_mul_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("L-Mul result: %f, Time taken: %f seconds\n", result, l_mul_time);

    // Measure time for standard multiplication
    start = clock();
    for (int i = 0; i < iterations; i++) {
        result = a * b;
    }
    end = clock();
    double float_mul_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Standard float multiplication result: %f, Time taken: %f seconds\n", result, float_mul_time);

    // Performance comparison
    printf("Performance difference: L-Mul is %.2f%% of the speed of standard multiplication\n", (l_mul_time / float_mul_time) * 100);

    return 0;
}
