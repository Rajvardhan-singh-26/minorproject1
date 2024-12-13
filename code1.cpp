   #include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm> // For transform function to convert string to lowercase
#include <map>

using namespace std;

// Struct to store client profile
struct ClientProfile {
    string name;  // Client's name
    int age;      // Client's age
    double weight;// Client's weight in kg
    double height;// Client's height in cm
};

// Global variables
map<string, ClientProfile> clientProfiles; // Stores client profiles with their unique ID
vector<string> foodItems;                  // List of food items

// Function to convert a string to lowercase
string toLowerCase(const string& str) {
    string lowerStr = str; // Copy the input string
    // Convert each character to lowercase using std::transform and ::tolower
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr; // Return the lowercase string
}

// Function to read CSV file
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data; // To store the CSV content
    string line;

    ifstream file(filename); // Open the file
    if (file.is_open()) { // Check if the file is open
        while (getline(file, line)) { // Read each line from the file
            vector<string> row; // To store each row's data
            stringstream ss(line); // Use stringstream to break the line into items
            string item;

            // Read each item separated by commas
            while (getline(ss, item, ',')) {
                row.push_back(item); // Add item to the current row
            }

            data.push_back(row); // Add the row to the data vector
        }
        file.close(); // Close the file after reading
    } else {
        cerr << "Error opening file: " << filename << endl; // Error message if the file doesn't open
    }

    return data; // Return the parsed CSV data
}

// Function to search CSV for food recommendations based on symptoms
void searchCSVBySymptoms(const vector<vector<string>>& data, const vector<string>& symptoms) {
    bool found = false; // To track if a match is found
    vector<string> lowerSymptoms; // Convert symptoms to lowercase for case-insensitive comparison

    // Convert each symptom to lowercase
    for (const auto& symptom : symptoms) {
        lowerSymptoms.push_back(toLowerCase(symptom));
    }

    // Iterate over each row in the CSV data
    for (const auto& row : data) {
        // Flag to track if all symptoms match in a row
        bool allSymptomsFound = true;

        // Check if each symptom exists in the row
        for (const auto& symptom : lowerSymptoms) {
            bool symptomFound = false;
            for (const auto& item : row) {
                string lowerItem = toLowerCase(item); // Convert each item to lowercase
                if (lowerItem.find(symptom) != string::npos) { // If the symptom is found
                    symptomFound = true;
                    break;
                }
            }
            // If a symptom is not found, mark as false and break
            if (!symptomFound) {
                allSymptomsFound = false;
                break;
            }
        }

        // If all symptoms are found in this row, print the row as a match
        if (allSymptomsFound) {
            found = true;
            for (const auto& cell : row) {
                cout << cell << " "; // Print each item in the row
            }
            cout << endl;
        }
    }

    // If no match is found, print a message
    if (!found) {
        cout << "No matching dietary recommendations found for the entered symptoms." << endl;
    }
}

// Function to input or update a client profile
void inputClientProfile() {
    ClientProfile profile; // Create a new profile
    string id; // Client's unique ID

    // Get client information
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

    // Store or update the client profile in the map (id is the key)
    clientProfiles[id] = profile;

    cout << "Client profile saved/updated!" << endl; // Confirmation message
}

// Function to handle multiple symptoms and provide suggestions based on symptoms
void processSymptoms(const vector<vector<string>>& data) {
    int numberOfSymptoms; // To store the number of symptoms entered by the user
    cout << "Enter number of symptoms: ";
    cin >> numberOfSymptoms; // Get the number of symptoms from the user

    vector<string> symptoms; // Vector to store symptoms
    string symptom;
    cout << "Enter the symptoms one by one: " << endl;
    for (int i = 0; i < numberOfSymptoms; i++) {
        cin >> symptom; // Get each symptom
        symptoms.push_back(symptom); // Add to the vector
    }

    // Call the search function to look for dietary recommendations based on the symptoms
    searchCSVBySymptoms(data, symptoms);
}

int main() {
    string filename = "D:/Minor project 1/dataset in excel.xlsx";
    vector<vector<string>> data = readCSV(filename); // Read the CSV data

    // Input client profile
    inputClientProfile(); // Function to input or update client profile

    // Handle multiple symptoms and search for dietary recommendations
    processSymptoms(data); // Call function to input and process symptoms

    return 0;
} 
