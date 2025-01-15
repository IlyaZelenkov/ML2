#include <iostream>
#include <vector>
#include <immintrin.h>

using namespace std;

// Numerical concept = {int, float, double}
template <typename N>
concept arithmetic = std::is_arithmetic_v<N>;

// String || Char concept.
template <typename S>
concept str =
    std::is_same_v<S, std::string> ||
    std::is_same_v<S, char> ||
    std::is_same_v<S, const char*> ||
    std::is_same_v<S, char*> ||
    std::is_same_v<S, std::string_view>;

template <typename N>
N dot(const vector<N>& a, const vector<N>& b)
{
    /*
     * @brief Central dot product function that takes in a template N numerical data type and forwards it to the appropriate function.
     *
     * @param a vector<N>
     * @param b vector<N>
     * @return N: numerical dot product.
     */

    static_assert(std::is_arithmetic_v<N>);

    if constexpr (std::is_same_v<N, int>)
    {
        return dot_i32(a, b);
    }

    else if constexpr (std::is_same_v<N, float>)
    {
        return dot_f32(a, b);
    }

    else if constexpr (std::is_same_v<N, double>)
    {
        return dot_f64(a, b);
    }

    else
    {
        // fallback scalar version, or compile error, etc.
        throw invalid_argument("Unsupported type for dot product");
    };
}

// 32-bit int dot product implementation using intrinsics.
int dot_i32(const vector<int>& a, const vector<int>& b)
{
    /*
     * @brief Calculates the dot product using vectors with int 32bit data types.
     *
     * @param a vector<int>
     * @param b vector<int>
     * @return int: dot product.
     */

    if (a.size() != b.size()) throw invalid_argument("Int32 dot product cannot be performed due to varying vector sizes.");

    // Size of array
    const size_t n = a.size();

    // constant expression of the data size.
    // a 256 register can hold 8 32-bit ints.
    const size_t simd_size = 8; ;

    // set a 256bit sum to 0.
    __m256i sum = _mm256_setzero_si256();

    for (size_t i = 0; i < n; i+= simd_size)
    {
        // Load 8 32bit integers from each vector.
        __m256i a_values = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&a[i]));
        __m256i b_values = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&b[i]));

        // multiply each value.
        __m256i product = _mm256_mullo_epi32(a_values, b_values );

        // add product to sum.
        sum = _mm256_add_epi32(sum, product);

    };

    // Reducing register to a 32-bit integer sum.
    // Aligning data to 32 bits to increase performance.
    // may remove because the alignment itself might reduce performance.
    alignas(32) int buffer[simd_size];
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer), sum);


    int result = 0;
    for (size_t i = 0; i < 8; ++i)
    {
        result += buffer[i];
        cout << buffer[i] << " ";
    };

    // if constexpr  (n % 8 != 0)
    // {
    //     for (int i = simd_size; i < n; ++i)
    //     {
    //         result += a[i] * b[i];
    //         cout << "Calculating Remainder";
    //     };
    //
    // };

    return result;

};

// 32-bit float dot product implementation using intrinsics.
float dot_f32(const vector<float>& a, const vector<float>& b)
{
    /*
     * @brief Calculates the dot product using vectors with float 32bit data types.
     *
     * @param a vector<float>
     * @param b vector<float>
     * @return float: dot product.
     */
    if (a.size() != b.size()) throw invalid_argument("32bit-float dot product cannot be performed due to varying vector sizes.");

    // Input size
    const size_t n = a.size();

    // size of number of 32bit float values in 256bit register
    constexpr size_t simd_size = 8;

    // Setting a 256bit float sum value to 0.
    __m256 sum = _mm256_setzero_ps();

    for (size_t i = 0; i < n; i+= simd_size)
    {
        // Load 8 floats from a & b.
        __m256 a_values = _mm256_load_ps(reinterpret_cast<const float*>(&a[i]));
        __m256 b_values = _mm256_load_ps(reinterpret_cast<const float*>(&b[i]));

        // Multiply a[i] * b[i].
        __m256 product = _mm256_mul_ps(a_values, b_values );

        // Add the product to the sum.
        sum = _mm256_add_ps(sum, product);

    };

    // Reducing register to a 32-bit float sum.
    // Aligning data to 32 bits to increase performance.
    alignas(32) float buffer[simd_size];

    _mm256_store_ps(reinterpret_cast<float*>(buffer), sum);

    float result = 0.0f;

    for (size_t i = 0; i < 8; ++i)
    {
        result += a[i] * b[i];
    };

    return result;
};

// 64-bit double dot product implementation using intrinsics.
double dot_f64(const vector<double>& a, const vector<double>& b)
{
    if (a.size() != b.size()) throw invalid_argument("Int32 dot product cannot be performed due to varying vector sizes.");

    const size_t n = a.size();

    constexpr size_t simd_size = 4;

    __m256d sum = _mm256_setzero_pd();
    for (size_t i = 0; i < n; i+= simd_size) {
        // Load 4 64bit double values from each vector.
        __m256d a_values = _mm256_load_pd(&a[i]);
        __m256d b_values = _mm256_load_pd(&b[i]);

        // multiply the values.
        __m256d product = _mm256_mul_pd(a_values, b_values );

        // Add them up.
        sum = _mm256_add_pd(sum, product);

    }

    // Reducing 256bit register to 64bits
    alignas(64) double buffer[simd_size];

    _mm256_store_pd(reinterpret_cast<double*>(buffer), sum);

    double result = 0.0;

    for (size_t i = 0; i < 8; ++i) {
        result += a[i] * b[i];

    }
    return result;
}


int main()
{

    const vector<int> vec1 = {1,1,2,3,4,5,6,7};
    const vector<int> vec2 = {1,2,3,4,5,6,8,7};
    cout << "i32 Dot product is: " << dot_i32(vec1, vec2) <<endl;

    const vector<float> vec3 = {1.2,1.3,2.4,3.5,4.6,5.7,6.8,7.9};
    const vector<float> vec4 = {1.9,2.8,3.5,4.4,5.3,6.2,8.1,7.0};
    cout << "f32 Dot product is: " << dot_f32(vec3, vec4) <<endl;

    const vector<double> vec5 = {1.2123123123,1.3123123123,2.4123123123,3.5123123123,4.6123123123,5.7123123123,6.8123123123,7.9123123123};
    const vector<double> vec6 = {1.9123123123,2.8123123123,3.5123123123,4.4123123123,5.3121231233,6.2123123123,8.1112321323,7.0123123123};
    cout << "d64 Dot product is: " << dot_f64(vec5, vec6) <<endl;




}
