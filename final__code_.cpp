#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Function to trim whitespace from the beginning and end of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (first == string::npos || last == string::npos) return ""; // Handle empty strings
    return str.substr(first, (last - first + 1));
}

// Function to convert a string to lowercase
string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Function to split a CSV line into a vector, handling quotes and commas properly
vector<string> parseCSVLine(const string& line) {
    vector<string> parsed;
    stringstream ss(line);
    string item;
    bool inQuotes = false;
    string temp;

    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes; // Toggle quotes
        } else if (ch == ',' && !inQuotes) {
            parsed.push_back(trim(temp));
            temp.clear();
        } else {
            temp += ch;
        }
    }
    if (!temp.empty()) parsed.push_back(trim(temp));
    return parsed;
}

// Function to read CSV file with disease-symptoms-foods data
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            vector<string> row = parseCSVLine(line);
            if (!row.empty()) data.push_back(row);
        }
        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return data;
}

// Function to split user input symptoms by commas and trim whitespace
vector<string> splitSymptoms(const string& input) {
    vector<string> symptoms;
    stringstream ss(input);
    string symptom;
    while (getline(ss, symptom, ',')) {
        symptoms.push_back(toLowerCase(trim(symptom)));
    }
    return symptoms;
}

// Function to search CSV for disease based on symptoms
void searchDiseaseBySymptoms(const vector<vector<string>>& data, const vector<string>& symptoms) {
    bool found = false;

    cout << "Entered Symptoms: ";
    for (const auto& symptom : symptoms) {
        cout << symptom << " ";
    }
    cout << endl;

    // Search through the CSV data
    for (const auto& row : data) {
        if (row.size() < 3) continue; // Ensure the row has Disease, Symptoms, and Food Recommendations

        // Split the disease symptoms and convert them to lowercase
        vector<string> diseaseSymptoms = splitSymptoms(row[0]); // Symptoms are in the first column
        vector<string> lowerDiseaseSymptoms;
        for (const auto& diseaseSymptom : diseaseSymptoms) {
            lowerDiseaseSymptoms.push_back(toLowerCase(diseaseSymptom));
        }

        // Check if all user symptoms are present in the disease's symptoms
        bool allSymptomsMatch = all_of(symptoms.begin(), symptoms.end(), [&](const string& symptom) {
            return any_of(lowerDiseaseSymptoms.begin(), lowerDiseaseSymptoms.end(), [&](const string& diseaseSymptom) {
                return diseaseSymptom.find(symptom) != string::npos;
            });
        });

        if (allSymptomsMatch) {
            found = true;
            cout << "Possible Disease: " << row[1] << endl; // Disease name in second column
            cout << "Recommended Foods: " << row[2] << endl; // Foods in third column
            cout << "-----------------------------" << endl;
        }
    }

    if (!found) {
        cout << "No matching diseases found for the entered symptoms." << endl;
    }
}

// Main function to test the code
int main() {
    string filename = "D:/Minor project 1/dataset in excel.csv"; // Path to the CSV file
    vector<vector<string>> data = readCSV(filename);

    string symptomsInput;
    cout << "Enter the symptoms (comma-separated): ";
    getline(cin, symptomsInput); // Get symptoms as a single line

    vector<string> symptoms = splitSymptoms(symptomsInput); // Split the symptoms into individual items

    searchDiseaseBySymptoms(data, symptoms);

    return 0;
}
