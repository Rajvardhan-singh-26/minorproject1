#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm> // For transform function to convert string to lowercase
#include <map>
using namespace std;
// Global variables
map<string, pair<string, string>> symptomDiseaseFoodMap; // Maps symptom to a pair of disease and food item
// Function to trim whitespace from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos)
        return ""; // Return empty string if no non-space character is found
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
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
// Function to load CSV data into the map (symptom -> disease, food)
void loadSymptomDiseaseFoodMap(const vector<vector<string>>& data) {
    for (const auto& row : data) {
        if (row.size() < 3) continue; // Ensure there are enough columns (symptom, disease, food)
        string symptomList = row[0];
        string disease = row[1];
        string foodItem = row[2];
        // Split symptoms if there are multiple in one cell
        stringstream symptomStream(symptomList);
        string symptom;
        while (getline(symptomStream, symptom, ',')) {
            // Trim and convert symptom to lowercase
            symptom = toLowerCase(trim(symptom));

            // Store the symptom with disease and food
            symptomDiseaseFoodMap[symptom] = make_pair(disease, foodItem);
        }
    }
}
// Function to search for disease and food recommendations based on symptoms
void searchBySymptoms(const vector<string>& symptoms) {
    bool found = false; // To track if a match is found
for (const auto & symptom : symptoms) {
        // Trim and convert user input symptom to lowercase
        string lowerSymptom = toLowerCase(trim(symptom));
        // Check if the symptom exists in the map
        if (symptomDiseaseFoodMap.find(lowerSymptom) != symptomDiseaseFoodMap.end()) {
            found = true;
            pair<string, string> diseaseFood = symptomDiseaseFoodMap[lowerSymptom]; // Get the disease and food pair
            cout << "Symptom: " << symptom << "\nDisease: " << diseaseFood.first << "\nRecommended Food: " << diseaseFood.second << endl << endl;
        }
    }
    // If no match is found, print a message
if (!found) {
        cout << "No matching disease or food recommendations found for the entered symptoms." << endl;
    }
}
// Function to search for a term (disease or food) in the CSV data (case-insensitive)
void searchCSV(const vector<vector<string>>& data, const string& searchTerm) {
    bool found = false; // To keep track if a match is found
    string lowerSearchTerm = toLowerCase(trim(searchTerm)); // Convert search term to lowercase and trim
    for (const auto& row : data) {
        for (const auto& item : row) {
            string lowerItem = toLowerCase(trim(item)); // Convert each item to lowercase and trim
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
// Function to handle multiple symptoms and provide suggestions based on symptoms
void processSymptoms() {
    int numberOfSymptoms;
    cout << "Enter number of symptoms: ";
    cin >> numberOfSymptoms;
    vector<string> symptoms;
    string symptom;
    cout << "Enter the symptoms one by one: " << endl;
    cin.ignore(); // Clear the input buffer before reading symptoms
    for (int i = 0; i < numberOfSymptoms; i++) {
        getline(cin, symptom);
        symptoms.push_back(toLowerCase(trim(symptom))); // Trim and convert to lowercase before adding
    }
    // Search for disease and food recommendations based on the symptoms
    searchBySymptoms(symptoms);
}
int main() {
    string filename ="D:/Minor project 1/dataset in excel.csv"; // Update the path to your dataset
    vector<vector<string>> data = readCSV(filename); // Read CSV data
    // Load the CSV data into the map (symptom -> disease, food)
    loadSymptomDiseaseFoodMap(data);
    // Search for disease and food based on user symptoms
    processSymptoms();
    // Additionally, search for a specific term (disease or food) in the CSV
    cout << "Enter the term you want to search for: ";
    string searchTerm;
    getline(cin, searchTerm); // Get the full line input as search term
    searchCSV(data, searchTerm);
    return 0;
}
