#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <cmath>

using namespace std;
using ColumnData = variant<vector<int>, vector<float>, vector<string>>;

// Version 1.0 08/01/2025
// Implementing a Pandas style Dataframe.
// Data stored as a hashmap data structure.
// Ensures a O(1) complexity as attributes increase.
// Less efficient at low attribute levels.

// Version 1.1 09/01/2025
// Arithmatic operator have been overwritten to support vector math.
// Added Min / Max / StDev / Mean functions.

// TODO: multi-type DataFrames

class DataFrame
{
private:

    vector <string> attributes;
    unordered_map <string, vector<int> > attributeMap;

public:

    // Default constructor.
    DataFrame(const vector<string>& atr, const vector< vector <int> >& data_in)
        : attributes(atr)
    {
        construct_map(data_in);
    };

    // Map based constructor.
    explicit DataFrame(const unordered_map<string, vector<int> >& data_in) : attributeMap(data_in){};

    // Maps data vectors with respect to their column (attribute) name.
    void construct_map(const vector< vector<int> >& data_in)
    {
        // Creates a HashMap of attributes : attribute data.
        // Parameters: data_in: input data matrix.
        // Returns: None

        for (int i = 0; i < attributes.size();  ++i)
        {
            vector<int> temp;

            temp.reserve(data_in.size());

            for (const auto & j : data_in)
            {
                temp.push_back(j.at(i));
            };

            attributeMap[attributes.at(i)] = temp;

        };
    };

    void toString (const string& attribute) const
    {
        const vector<int>& v = attributeMap.at(attribute);

        cout << attribute << ": ";

        for (int i : v)
        {
            cout << i << " ";
        };

    };

    // const operator[] overload to extract specific attributes. Works like a map
    const vector<int>& operator[](const string& columnName) const
    {
        if (!attributeMap.contains(columnName))
        {

            throw runtime_error("Column: " + columnName + "not found found in dataset");

        };

        return attributeMap.at(columnName);

    };

    // operator[] overload to extract specific attributes for arithmatic operations.
    vector<int>& operator[](const string& columnName)
    {
        if (!attributeMap.contains(columnName))
        {

            throw runtime_error("Column: " + columnName + " not found found in dataset");

        };

        return attributeMap[columnName];

    };

    // Statistical Functions.
    // Calculate mean.
    float mean(const string& columnName) const
    {
        if (!attributeMap.contains(columnName))
        {
            throw runtime_error("Column: " + columnName + " not found found in dataset");
        }

        if (attributeMap.at(columnName).empty())
        {
            throw runtime_error("Cannot calculate the mean of an empty array");
        }

        const vector<int> column = attributeMap.at(columnName);

        float sum = 0;

        for (int i = 0; i < static_cast<int>(column.size()); ++i)
        {
            sum += static_cast<float>(column.at(i));
        };

        return sum / static_cast<float>(column.size());
    };

    // Calculate StDev
    float stdev(const string& columnName) const
    {
        if (!attributeMap.contains(columnName)) {
            throw runtime_error("Column: " + columnName + " not found found in dataset");
        }

        const float mean = this -> mean(columnName);
        const vector<int> column = attributeMap.at(columnName);
        float error = 0;

        for (int i = 0; i < static_cast<int>(attributeMap.at(columnName).size()); ++i)
        {
            error += pow((column.at(i) - mean), 2);
        };

        return sqrt(error / static_cast<float>(column.size()));
    };

    // Calc Variance
    float variance(const string& columnName) const {
        if (!attributeMap.contains(columnName)) {
            throw runtime_error("Column: " + columnName + " not found in dataset");
        }

        const vector<int>& column = attributeMap.at(columnName);
        float meanValue = mean(columnName);

        float sum = 0.0f;
        for (int val : column) {
            sum += pow(val - meanValue, 2);
        }

        return sum / column.size();
    }

    ~DataFrame()= default;
};

// Vector addition operator.
vector<int> operator+(const vector<int>& vec1, const vector<int>& vec2)
{
    if (vec1.size() != vec2.size())
    {
        throw out_of_range("Vector addition cannot be completed, vectors are of different sizes");
    };

    vector<int> temp(vec1.size());

    for (int i = 0; i < vec1.size(); ++i)
    {
        temp[i] = vec1[i] + vec2[i];
    };

    return temp;
};

// Vector subtraction operator.
vector<int> operator-(const vector<int>& vec1, const vector<int>& vec2)
{
    if (vec1.size() != vec2.size())
    {
        throw out_of_range("Vector addition cannot be completed, vectors are of different sizes");
    };

    vector<int> temp(vec1.size());

    for (int i = 0; i < vec1.size(); ++i)
    {
        temp[i] = vec1[i] - vec2[i];
    }

    return temp;
};

// Vector division operator.
vector<float> operator/(const vector<int>& vec1, const vector<int>& vec2)
{
    if (vec1.size() != vec2.size())
    {
        throw out_of_range("Vector addition cannot be completed, vectors are of different sizes");
    };

    vector<float> temp(vec1.size());

    for (int i = 0; i < vec1.size(); ++i)
    {
        if (static_cast<float>(vec2.at(i)) == 0)
        {
            throw runtime_error("Value divisor at index: " + to_string(i) + " cannot be zero");
        }
        temp[i] = static_cast<float>(vec1.at(i)) / static_cast<float>(vec2.at(i));
    }

    return temp;
};

// Vector multiplication operator.
vector<int> operator*(const vector<int>& vec1, const vector<int>& vec2)
{
    if (vec1.size() != vec2.size())
    {
        throw out_of_range("Vector addition cannot be completed, vectors are of different sizes");
    };

    vector<int> temp(vec1.size());

    for (int i = 0; i < vec1.size(); ++i)
    {
        temp[i] = vec1[i] * vec2[i];
    }

    return temp;
};

// Dot product function.
int dot(const vector<int>& vec1, const vector<int>& vec2)
{
    if (vec1.size() != vec2.size()) {
        throw runtime_error("Dot product cannot be completed, vectors are of different sizes");
    }

    int sum = 0;
    for (int i = 0; i < vec1.size(); ++i) {
        sum += vec1.at(i) * vec2.at(i);
    };

    return sum;
};

// Find Min Value
int min(const vector<int>& vec) {

    int min = vec.at(0);

    for (int i = 1; i < vec.size(); ++i) {
        if (vec.at(i) == 0 || vec.at(i) < min ) {
            min = vec.at(i);
        };
    };

    return min;
}

// Find Max Value
int max(const vector<int>& vec) {
    int max = vec.at(0);

    for (int i = 1; i < vec.size(); ++i)
    {
        if (vec.at(i) == 0 || vec.at(i) > max )
        {
            max = vec.at(i);
        };
    };

    return max;
};



int main()
{
    // Assume that a csv extractor yielded two vectors.
    // 1. A string array containing column names. Length (m)
    // 2. A numeric matrix containing values. length (m x n)
    const vector<string> attr = {"height", "width", "age", "extra"};
    const vector< vector<int> > data = {{4,5,6,7}, {8,9,10,11}, {16,19,13,14} , {1,2,3,4}, {7,4,3,5}, {5,4,3,5}};
    DataFrame df = DataFrame(attr, data);

    vector<int> vec = df["height"];
    vector<float> division = df["width"] / df["age"];
    cout << dot(df["height"], df["width"]) << endl;

    return 0;
};
