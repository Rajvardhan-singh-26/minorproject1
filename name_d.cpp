#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <functional>

using namespace std;

// Function to trim whitespace from the beginning and end of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

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
                row.push_back(trim(item)); // Trim whitespace from each item
            }

            data.push_back(row);
        }
        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return data;
}

// Function to split symptoms into a vector
vector<string> splitSymptoms(const string& input) {
    vector<string> symptoms;
    stringstream ss(input);
    string symptom;
    while (getline(ss, symptom, ',')) {
        symptoms.push_back(trim(symptom));
    }
    return symptoms;
}

// Function to search CSV for disease based on symptoms
void searchDiseaseBySymptoms(const vector<vector<string>>& data, const vector<string>& symptoms) {
    bool found = false;
    vector<string> lowerSymptoms;

    // Convert symptoms to lowercase and trim
    for (const auto& symptom : symptoms) {
        lowerSymptoms.push_back(toLowerCase(trim(symptom)));
    }

    // Search through the CSV data
    for (const auto& row : data) {
        if (row.size() < 2) continue; // Ensure there are symptoms in the row after the disease name

        bool allSymptomsFound = true;

        // Check if all symptoms are found in the row
        for (const auto& symptom : lowerSymptoms) {
            bool symptomFound = false;
            for (size_t i = 1; i < row.size(); ++i) { // Start from index 1 to skip the disease name
                string lowerItem = toLowerCase(trim(row[i]));
                if (lowerItem.find(symptom) != string::npos) {
                    symptomFound = true;
                    break;
                }
            }
            if (!symptomFound) {
                allSymptomsFound = false;
                break;
            }
        }

        // If all symptoms match, print only the disease name (first column)
        if (allSymptomsFound) {
            found = true;
            cout << "Disease: " << row[0] << endl; // Disease name in first column
            break; // Exit after finding the first match (optional: remove to find all matches)
        }
    }

    if (!found) {
        cout << "No matching disease found for the entered symptoms." << endl;
    }
}

// Main function to test the code
int main() {
    string filename = "D:/Minor project 1/dataset in excel.csv";
    vector<vector<string>> data = readCSV(filename);

    int numberOfSymptoms;
    cout << "Enter number of symptoms: ";
    cin >> numberOfSymptoms;

    cin.ignore();  // To discard the newline character left by cin >> numberOfSymptoms

    string symptomsInput;
    cout << "Enter the symptoms (comma-separated): ";
    getline(cin, symptomsInput);  // Get symptoms as a single line

    vector<string> symptoms = splitSymptoms(symptomsInput);  // Split the symptoms into individual items

    searchDiseaseBySymptoms(data, symptoms);

    return 0;
}
