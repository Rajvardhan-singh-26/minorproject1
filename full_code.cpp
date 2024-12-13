#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <functional>
#include <queue>

using namespace std;

// Struct to store client profile
struct ClientProfile {
    string name;
    int age;
    double weight;
    double height;
};

// Define a struct for the user profile
struct UserProfile {
    string name;
    string symptoms;  // A string representing symptoms entered by the user
    string recommendation;  // Food recommendation based on symptoms
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

// A simple function to simulate food recommendation based on symptoms (you can replace this with more complex logic)
string generateRecommendation(const string& symptoms) {
    // Parse the symptoms (split by comma)
    vector<string> symptomsList;
    stringstream ss(symptoms);
    string symptom;
    while (getline(ss, symptom, ',')) {
        symptomsList.push_back(toLowerCase(symptom));
    }

    // Implement your recommendation logic here, possibly using the CSV data or other knowledge base
   
    // For now, let's use a simple example based on the provided symptoms:
    if (find(symptomsList.begin(), symptomsList.end(), "fever") != symptomsList.end()) {
        return "Recommended Food: Light soups, herbal tea, and hydration.";
    } else if (find(symptomsList.begin(), symptomsList.end(), "fatigue") != symptomsList.end()) {
        return "Recommended Food: Whole grains, nuts, and fresh fruits.";
    } else if (find(symptomsList.begin(), symptomsList.end(), "cold") != symptomsList.end()) {
        return "Recommended Food: Warm broths, garlic, and ginger.";
    } else {
        return "Recommended Food: Balanced diet including fruits, vegetables, and protein.";
    }
}

// Function to handle user input and generate recommendation
void handleUserInput() {
    string name, symptoms;
    cout << "Enter your name: ";
    cin >> name;

    cout << "Enter your symptoms (comma-separated): ";
    getline(cin, symptoms);

    // Process the symptoms and generate a recommendation
    string recommendation = generateRecommendation(symptoms);  // Replace with your recommendation logic

    cout << "Recommendation for " << name << ": " << recommendation << endl;
}

int main() {
    string filename = "D:/Minor project 1/dataset in excel.csv";
    vector<vector<string>> data = readCSV(filename);

    // Input client profile
    inputClientProfile();

    // Handle multiple symptoms and provide suggestions based on symptoms
    processSymptoms(data);

    // Sort and display an array
    sortAndDisplayArray();

    // Handle user input and generate recommendation
    handleUserInput();

    return 0;
}