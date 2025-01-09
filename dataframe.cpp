#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

using namespace std;
using ColumnData = variant<vector<int>, vector<float>, vector<string>>;

// Version 1.0: 08/ 01 / 2025 
// Implementing a Pandas style Dataframe.
// Data stored as a hashmap data structure.
// Ensures a O(1) complexity as attributes increase.
// Less efficient at low attribute levels.

// Version 1.1: 09 / 01 / 2025
// Arithmatic operator have been overwritten to support vector math.



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

            for (int j = 0; j < data_in.size(); ++j)
            {
                temp.push_back(data_in.at(j).at(i));
            };

            attributeMap[attributes.at(i)] = temp;

        };
    };

    void toString (const string& attribute) const
    {
        const vector<int>& v = attributeMap.at(attribute);

        cout << attribute << ": ";

        for (int i = 0; i < v.size(); ++i)
        {
            cout << v.at(i) << " ";
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
vector<int> operator+(vector<int>& vec1, vector<int>& vec2)
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
vector<int> operator-(vector<int>& vec1, vector<int>& vec2)
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
vector<float> operator/(vector<int>& vec1, vector<int>& vec2)
{
    if (vec1.size() != vec2.size())
    {
        throw out_of_range("Vector addition cannot be completed, vectors are of different sizes");
    };

    vector<float> temp(vec1.size());

    for (int i = 0; i < vec1.size(); ++i)
    {
        if (float(vec2.at(i)) == 0)
        {
            throw runtime_error("Value divisor at index: " + to_string(i) + " cannot be zero");
        }
        temp[i] = float(vec1.at(i)) / float(vec2.at(i));
    }

    return temp;
};

// Vector multiplication operator.
vector<int> operator*(vector<int>& vec1, vector<int>& vec2)
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
