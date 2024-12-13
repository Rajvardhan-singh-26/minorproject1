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
                row.push_back(trim(item));
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
        if (row.size() < 3) continue; // Ensure there are at least 3 columns: Symptoms, Disease, Foods

        // Split the symptoms from the CSV row
        vector<string> diseaseSymptoms = splitSymptoms(row[0]);

        // Convert disease symptoms to lowercase for comparison
        vector<string> lowerDiseaseSymptoms;
        for (const auto& diseaseSymptom : diseaseSymptoms) {
            lowerDiseaseSymptoms.push_back(toLowerCase(diseaseSymptom));
        }

        // Calculate the number of matching symptoms
        int matchingCount = 0;
        for (const auto& symptom : lowerSymptoms) {
            if (find(lowerDiseaseSymptoms.begin(), lowerDiseaseSymptoms.end(), symptom) != lowerDiseaseSymptoms.end()) {
                matchingCount++;
            }
        }

        // Set a threshold for matching (e.g., 50% of symptoms)
        double threshold = 0.5;
        double matchPercentage = (double)matchingCount / lowerSymptoms.size();

        if (matchPercentage >= threshold) {
            found = true;
            cout << row[1] << endl; // Output only the disease name
            break;
        }
    }
}

int main() {
    string filename = "D:/Minor project 1/dataset in excel.csv"; // Path to the CSV file
    vector<vector<string>> data = readCSV(filename);

    string symptomsInput;
    cout << "Enter the symptoms (comma-separated): ";
    getline(cin, symptomsInput);

    vector<string> symptoms = splitSymptoms(symptomsInput);

    searchDiseaseBySymptoms(data, symptoms);

    return 0;
}
