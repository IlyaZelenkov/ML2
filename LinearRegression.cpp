#include <iostream>
#include <vector>
#include <cassert>
#include <type_traits>
#include <cmath>
#include <concepts>

using namespace std;

// Simple Linear Regression Model:
//

// Arithmetic concept.
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

template <arithmetic N, str S>
class LinearRegression
{
private:

  float weight;
  float bias;
  float learn_rate;
  int epochs;


  N sse(const vector<N>& y, const vector<N>& y_pred)
  {
    if (y.size() != y_pred.size()) {throw std::invalid_argument("Vectors must have the same size to perform SSE"); };
    N sum = 0;

    for (size_t i = 0; i < y.size(); ++i)
    {

      sum += pow((y[i] - y_pred[i]), 2);

    };

    return sum;

  };



public:

  LinearRegression(float l_rate, int epoch)
  {
      if (l_rate <= 0) { throw invalid_argument("Learning rate must be above 0"); };
      if (epoch <= 0) { throw invalid_argument("Epochs must be above 0"); };

      this->weight = 0.1f;
      this->bias = 0.1;
      this->learn_rate = l_rate;
      this->epochs = epoch;

  };


  void fit(vector<N>& X, vector<N>& y)
  {
    if (X.size() != y.size()) { throw invalid_argument("Vectors must have the same size to perform Fit"); };

    for (size_t i = 0; i < this -> epochs; ++i)
    {
        vector<N> y_pred = predict(X);

        N d_weight = - ( 2 / X.size() ) * dot(X, (y - y_pred));
        N d_bias = - (2 / X.size()) * sum(y - y_pred);

        this -> weight += d_weight * this->learn_rate;
        this -> bias += d_bias * this->learn_rate;

        if (i % 50 == 0)
          {
             cout << "At iteration: " + to_string(i) + "\n" + "The SSE is: " + to_string(this -> sse(y, y_pred)) << endl;
          };

    };

  };

  vector<N> predict(vector<N>& X)
  {
     return this -> weight * X - this->bias;
  };

  ~LinearRegression()

};

template <arithmetic N>
vector<N> operator*(const vector<N>& a, N scalar)
{
  vector<N> result(a.size());

  for (size_t i = 0; i < a.size(); ++i)
  {
    result[i] = a[i] * scalar;
  }

  return result;
}

template <arithmetic N>
vector<N> operator+(const vector<N>& a, N scalar)
{
  vector<N> result(a.size());

  for (size_t i = 0; i < a.size(); ++i)
  {
    result[i] = a[i] + scalar;
  };

  return result;
}

template <arithmetic N>
N dot(const vector<N>& a, const vector<N>& b)
{
   N sum = 0;

   for (size_t i = 0; i < a.size(); ++i)
   {
     sum += a[i] * b[i];
   };

   return sum;
};

template <arithmetic N>
N sum(const vector<N>& a)
{
  N sum = 0;

  for (size_t i = 0; i < a.size(); ++i)
  {
    sum += a[i];
  };

  return sum;
};


