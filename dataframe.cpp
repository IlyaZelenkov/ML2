#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

using namespace std;
using ColumnData = variant<vector<int>, vector<float>, vector<string>>;

// Data can can be stored column wise.
// Each column is stored in memory.
// Each attributes gets assigned a hash value which points to a particular address of the data associated with the attribute.

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

    // Implement a [] operator so it behaves like a python dataframe.
    // eg. df[attribute] -> vector<int> data.

    const vector<int>& operator[](const string& columnName) const
    {
        if (!attributeMap.contains(columnName))
        {

            throw runtime_error("Column: " + columnName + "not found found in dataset");

        };

        return attributeMap.at(columnName);

    };

    void print() const
    {
        // Prints out each attribute: attributes data.
        for (const auto& pair : attributeMap)
        {
            cout << pair.first << ": ";
            for (int value : pair.second)
            {
                cout << value << " ";
            };

            cout << endl;
        };
    };

    void toString (string attribute) const
    {
        const vector<int>& v = attributeMap.at(attribute);

        cout << attribute << ": ";

        for (int i = 0; i < v.size(); ++i)
        {
            cout << v.at(i) << " ";
        };
    };


    ~DataFrame()= default;
};

int main()
{
    // Assume that a csv extractor yielded two vectors.
    // 1. A string array containing column names. Length (m)
    // 2. A numeric matrix containing values. length (m x n)
    vector<string> attr = {"height", "width", "age", "extra"};
    vector< vector<int> > data = {{4,5,6,7}, {8,9,10,11}, {16,19,13,14} , {1,2,3,4}, {7,4,3,5}, {5,4,3,5}};

    DataFrame df = DataFrame(attr, data);
    df.print();

    vector<int> vec = df["height"];

    return 0;
};
