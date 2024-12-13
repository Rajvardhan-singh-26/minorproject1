#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <functional>

using namespace std;

// Struct to store client profile
struct ClientProfile {
    string name;
    int age;
    double weight;
    double height;
};

// Global variables
map<string, ClientProfile> clientProfiles;

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

// Function to search CSV for food recommendations based on symptoms
void searchCSVBySymptoms(const vector<vector<string>>& data, const vector<string>& symptoms) {
    bool found = false;
    vector<string> lowerSymptoms;

    for (const auto& symptom : symptoms) {
        lowerSymptoms.push_back(toLowerCase(symptom));
    }

    for (const auto& row : data) {
        bool allSymptomsFound = true;

        for (const auto& symptom : lowerSymptoms) {
            bool symptomFound = false;
            for (const auto& item : row) {
                string lowerItem = toLowerCase(item);
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

        if (allSymptomsFound) {
            found = true;
            for (const auto& cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }

    if (!found) {
        cout << "No matching dietary recommendations found for the entered symptoms." << endl;
    }
}

// Function to input or update a client profile
void inputClientProfile() {
    ClientProfile profile;
    string id;

    cout << "Enter Client ID: ";
    cin >> id;

    cout << "Enter Name: ";
    cin >> profile.name;

    cout << "Enter Age: ";
    cin >> profile.age;

    cout << "Enter Weight (kg): ";
    cin >> profile.weight;

    cout << "Enter Height (cm): ";
    cin >> profile.height;

    clientProfiles[id] = profile;
    cout << "Client profile saved/updated!" << endl;
}

// Function to handle multiple symptoms and provide suggestions based on symptoms
void processSymptoms(const vector<vector<string>>& data) {
    int numberOfSymptoms;
    cout << "Enter number of symptoms: ";
    cin >> numberOfSymptoms;

    vector<string> symptoms;
    string symptom;
    cout << "Enter the symptoms one by one: " << endl;
    for (int i = 0; i < numberOfSymptoms; i++) {
        cin >> symptom;
        symptoms.push_back(symptom);
    }

    searchCSVBySymptoms(data, symptoms);
}

// Function to sort an array in ascending and descending order
void sortAndDisplayArray() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    if (n <= 0) {
        cerr << "Number of elements must be positive!" << endl;
        return;
    }

    vector<int> arr(n);

    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Sort in ascending order
    sort(arr.begin(), arr.end());
    cout << "Sorted array in ascending order: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    // Sort in descending order
    sort(arr.begin(), arr.end(), greater<int>());
    cout << "Sorted array in descending order: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    string filename = "D:/Minor project 1/dataset in excel.csv";
    vector<vector<string>> data = readCSV(filename);

    // Input client profile
    inputClientProfile();

    // Handle multiple symptoms and search for dietary recommendations
    processSymptoms(data);

    // Sort and display an array
    sortAndDisplayArray();

    return 0;
}