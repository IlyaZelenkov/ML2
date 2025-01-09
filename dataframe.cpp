#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

using namespace std;
using ColumnData = variant<vector<int>, vector<float>, vector<string>>;

// Implementing a Pandas style Dataframe.
// Data stored as a hashmap data structure.
// Ensures a O(1) complexity as attributes increase.
// Less efficient at low attribute levels.


// Arithmatic operator have been overwritten to support vector math.


// TODO: multi-type DataFrames.

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

    // Operator overload functions.
    // DataFrame[attribute] -> vector<type>
    // DataFrame[attribute1] +/- DataFrame[attribute2] -> vector<type>

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
    }

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

int main()
{
    // Assume that a csv extractor yielded two vectors.
    // 1. A string array containing column names. Length (m)
    // 2. A numeric matrix containing values. length (m x n)
    vector<string> attr = {"height", "width", "age", "extra"};
    const vector< vector<int> > data = {{4,5,6,7}, {8,9,10,11}, {16,19,13,14} , {1,2,3,4}, {7,4,3,5}, {5,4,3,5}};

    DataFrame df = DataFrame(attr, data);

    vector<int> vec = df["height"];
    vector<float> division = df["width"] / df["age"];

    return 0;
};
