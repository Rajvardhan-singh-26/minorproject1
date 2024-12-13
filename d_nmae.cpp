#include <iostream>
#include <fstream>
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

// Function to split a CSV line into a dynamic array, handling quotes and commas properly
string* parseCSVLine(const string& line, int& size) {
    string* parsed = new string[100]; // Allocate a fixed-size dynamic array
    int count = 0;
    string temp;
    bool inQuotes = false;

    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes; // Toggle quotes
        } else if (ch == ',' && !inQuotes) {
            parsed[count++] = trim(temp); // Add to array and increment count
            temp.clear();
        } else {
            temp += ch;
        }
    }
    if (!temp.empty()) parsed[count++] = trim(temp);

    size = count; // Set the size of parsed items
    return parsed;
}

// Function to read CSV file with disease-symptoms-foods data
string** readCSV(const string& filename, int& rows) {
    ifstream file(filename);
    string line;
    rows = 0;

    string** data = new string*[100]; // Allocate array of pointers for each row

    if (file.is_open()) {
        while (getline(file, line)) {
            int size;
            string* row = parseCSVLine(line, size); // Parse line
            if (size > 0) data[rows++] = row;       // Assign row pointer and increment row count
        }
        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return data;
}

// Function to split user input symptoms by commas and trim whitespace
string* splitSymptoms(const string& input, int& count) {
    stringstream ss(input);
    string symptom;
    string* symptoms = new string[100];
    count = 0;

    while (getline(ss, symptom, ',')) {
        symptoms[count++] = toLowerCase(trim(symptom));
    }

    return symptoms;
}

// Function to search CSV for disease based on symptoms
void searchDiseaseBySymptoms(string** data, int rows, string* symptoms, int symptomCount) {
    bool found = false;

    cout << "Entered Symptoms: ";
    for (int i = 0; i < symptomCount; ++i) {
        cout << symptoms[i] << " ";
    }
    cout << endl;

    // Search through the CSV data
    for (int i = 0; i < rows; ++i) {
        string* row = data[i];
        if (row[0].empty() || row[1].empty() || row[2].empty()) continue; // Ensure valid data

        // Debug: Print the symptoms for each disease in the CSV
        cout << "Checking disease: " << row[1] << ", Symptoms: " << row[0] << endl;

        // Split the disease symptoms and convert them to lowercase
        int diseaseSymptomCount;
        string* diseaseSymptoms = splitSymptoms(row[0], diseaseSymptomCount);

        // Check if all user symptoms are present in the disease's symptoms
        bool allSymptomsMatch = true;
        for (int j = 0; j < symptomCount; ++j) {
            bool symptomFound = false;
            for (int k = 0; k < diseaseSymptomCount; ++k) {
                if (diseaseSymptoms[k].find(symptoms[j]) != string::npos) {
                    symptomFound = true;
                    break;
                }
            }
            if (!symptomFound) {
                allSymptomsMatch = false;
                break;
            }
        }

        delete[] diseaseSymptoms; // Free disease symptoms array

        if (allSymptomsMatch) {
            found = true;
            cout << "Possible Disease: " << row[1] << endl;
            cout << "Recommended Foods: " << row[2] << endl;
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
    int rows;
    string** data = readCSV(filename, rows);

    string symptomsInput;
    cout << "Enter the symptoms (comma-separated): ";
    getline(cin, symptomsInput); // Get symptoms as a single line

    int symptomCount;
    string* symptoms = splitSymptoms(symptomsInput, symptomCount);

    searchDiseaseBySymptoms(data, rows, symptoms, symptomCount);

    // Free dynamically allocated memory
    for (int i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;
    delete[] symptoms;

    return 0;
}
