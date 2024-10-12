#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Convert float to half-precision format (f16)
uint16_t float_to_f16(float f) {
    uint32_t f_bits = *((uint32_t*)&f);
    
    uint16_t sign = (f_bits >> 31) & 0x1;
    uint16_t exponent = ((f_bits >> 23) & 0xFF) - 112; // Adjust bias (127 for f32 to 15 for f16)
    uint16_t mantissa = (f_bits >> 13) & 0x3FF; // Take 10 bits of mantissa

    return (sign << 15) | (exponent << 10) | mantissa;
}

// Convert half-precision format (f16) to float
float f16_to_float(uint16_t f16) {
    uint32_t sign = (f16 >> 15) & 0x1;
    uint32_t exponent = ((f16 >> 10) & 0x1F) + 112; // Adjust bias back to f32
    uint32_t mantissa = f16 & 0x3FF;

    uint32_t f32 = (sign << 31) | (exponent << 23) | (mantissa << 13);
    return *((float*)&f32);
}

// Approximate multiplication using L-Mul for f16
uint16_t l_mul_f16(uint16_t a, uint16_t b) {
    // Extract sign, exponent, and mantissa
    uint16_t sign_a = (a >> 15) & 1;
    uint16_t exp_a = (a >> 10) & 0x1F;
    uint16_t mant_a = a & 0x3FF;

    uint16_t sign_b = (b >> 15) & 1;
    uint16_t exp_b = (b >> 10) & 0x1F;
    uint16_t mant_b = b & 0x3FF;

    // Compute result's sign (XOR the signs)
    uint16_t sign_result = sign_a ^ sign_b;

    // Add the exponents (subtract bias 15 for f16)
    uint16_t exp_result = exp_a + exp_b - 15;

    // Approximate mantissa multiplication using L-Mul
    uint16_t mant_result = (mant_a + mant_b) >> 1; // Simple approximation of mantissa addition

    // Recompose the f16 result
    return (sign_result << 15) | (exp_result << 10) | (mant_result & 0x3FF);
}

int main() {
    // Example values in float (which we'll convert to f16)
    float fa = 1.75f;
    float fb = 2.5f;

    // Convert the float values to f16
    uint16_t a_f16 = float_to_f16(fa);
    uint16_t b_f16 = float_to_f16(fb);

    // Perform L-Mul on f16 values
    uint16_t result_f16 = l_mul_f16(a_f16, b_f16);

    // Convert the result back to float for display
    float result_float = f16_to_float(result_f16);

    // Print results
    printf("L-Mul f16 result (in float): %f\n", result_float);

    return 0;
}
