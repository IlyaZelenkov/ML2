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

// 32-bit int dot product implementation using intrinsics.
int dot_i32(const vector<int>& a, const vector<int>& b, const int& remainder, const int& n, const size_t& d_size)
{
    /*
     * @brief Calculates the dot product using vectors with int 32bit data types.
     *
     * @param a vector<int>
     * @param b vector<int>
     * @return int: dot product.
     */

    // set a 256bit sum to 0.
    __m256i sum = _mm256_setzero_si256();

    for (size_t i = 0; i < n - remainder; i+= d_size)
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
    alignas(32) int buffer[d_size];
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer), sum);

    int result = 0;
    for (size_t i = 0; i < 8; ++i)
    {
        result += buffer[i];
        cout << buffer[i] << " ";
    };

    if (remainder != 0)
    {
        for (size_t i = n - remainder; i < n; ++i)
        {
            cout << i << endl;
            result += a[i] * b[i];
        };
    };

    return result;
};

// 32-bit float dot product implementation using intrinsics.
float dot_f32(const vector<float>& a, const vector<float>& b, const int& remainder, const int& n, const size_t& d_size)
{
    /*
     * @brief Calculates the dot product using vectors with float 32bit data types.
     *
     * @param a vector<float>
     * @param b vector<float>
     * @return float: dot product.
     */

    // Setting a 256bit float sum value to 0.
    __m256 sum = _mm256_setzero_ps();

    for (size_t i = 0; i < n - remainder; i+= d_size)
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
    alignas(32) float buffer[d_size];

    _mm256_store_ps(reinterpret_cast<float*>(buffer), sum);

    float result = 0.0f;

    for (size_t i = n - remainder; i < n; ++i)
    {
        result += a[i] * b[i];
    };

    return result;
};

// 64-bit double dot product implementation using intrinsics.
double dot_f64(const vector<double>& a, const vector<double>& b, const int& remainder, const int& n, const size_t& d_size)
{

    const size_t n = a.size();

    constexpr size_t simd_size = 4;

    __m256d sum = _mm256_setzero_pd();

    for (size_t i = 0; i < n - remainder; i+= simd_size) {
        // Load 4 64bit double values from each vector.
        __m256d a_values = _mm256_load_pd(&a[i]);
        __m256d b_values = _mm256_load_pd(&b[i]);

        // multiply the values.
        __m256d product = _mm256_mul_pd(a_values, b_values );

        // Add them up.
        sum = _mm256_add_pd(sum, product);

    };

    // Reducing 256bit register to 64bits
    alignas(64) double buffer[simd_size];

    _mm256_store_pd(reinterpret_cast<double*>(buffer), sum);

    double result = 0.0;

    for (size_t i = n - remainder - 1; i < n; ++i)
    {
        result += a[i] * b[i];
    };

    return result;
}

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

    if (a.size() != b.size()) throw invalid_argument("Int32 dot product cannot be performed due to varying vector sizes.");

    if constexpr (std::is_same_v<N, int>)
    {
        cout << "Central Dot detected int32 values." << endl;
        return dot_i32(a, b,  a.size() % 8 , a.size(), 8);
    }

    else if constexpr (std::is_same_v<N, float>)
    {
        cout << "Central Dot detected float32 values." << endl;
        return dot_f32(a, b,  a.size() % 8 , a.size(), 8);
    }

    else if constexpr (std::is_same_v<N, double>)
    {
        cout << "Central Dot detected float64 values." << endl;
        return dot_f64(a, b,  a.size() % 4 , a.size(), 4);
    }

    else
    {
        // fallback scalar version, or compile error, etc.
        throw invalid_argument("Unsupported type for dot product");
    };
}

int main()
{

    const vector<int> vec0 = {1,2,3,4,5,6,7};
    const vector<int> vec01 = {1,2,3,4,5,6,7};
    cout << dot(vec0, vec01) << endl;

    const vector<int> vec1 = {1,1,2,3,4,5,6,7};
    const vector<int> vec2 = {1,2,3,4,5,6,8,7};
    cout << dot(vec1, vec2) <<endl;

    const vector<float> vec3 = {1.2,1.3,2.4,3.5,4.6,5.7,6.8,7.9};
    const vector<float> vec4 = {1.9,2.8,3.5,4.4,5.3,6.2,8.1,7.0};
    cout << dot(vec3, vec4) <<endl;

    const vector<double> vec5 = {1.2123123123,1.3123123123,2.4123123123,3.5123123123,4.6123123123,5.7123123123,6.8123123123,7.9123123123};
    const vector<double> vec6 = {1.9123123123,2.8123123123,3.5123123123,4.4123123123,5.3121231233,6.2123123123,8.1112321323,7.0123123123};
    cout << dot(vec5, vec6) <<endl;


}
