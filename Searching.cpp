#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm> // For transform function to convert string to lowercase

using namespace std;

// Function to convert a string to lowercase
string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Function to read CSV file
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    string line;

    ifstream file(filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            vector<string> row;
            stringstream ss(line);
            string item;

            while (getline(ss, item, ',')) {
                row.push_back(item);
            }

            data.push_back(row);
        }

        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return data;
}

// Function to search for a term in the CSV data (case-insensitive)
void searchCSV(const vector<vector<string>>& data, const string& searchTerm) {
    bool found = false; // To keep track if a match is found
    string lowerSearchTerm = toLowerCase(searchTerm); // Convert search term to lowercase

    for (const auto& row : data) {
        for (const auto& item : row) {
            string lowerItem = toLowerCase(item); // Convert each item to lowercase
            if (lowerItem.find(lowerSearchTerm) != string::npos) { // Case-insensitive comparison
                found = true;
                // If found, print the whole row
                for (const auto& cell : row) {
                    cout << cell << " ";
                }
                cout << endl;
                break; // Exit the inner loop since the row has been printed
            }
        }
    }

    if (!found) {
        cout << "No match found for: " << searchTerm << endl;
    }
}

int main() {
    string filename = "D:/Minor project 1/dataset in excel.csv";
    vector<vector<string>> data = readCSV(filename);

    // Input search term from the user
    cout << "Enter the term you want to search for: ";
    string searchTerm;
    getline(cin, searchTerm); // Get the full line input as search term

    // Search the CSV data for the given term (case-insensitive)
    searchCSV(data, searchTerm);

    return 0;
}
