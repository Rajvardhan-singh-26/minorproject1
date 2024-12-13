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

// Function to generate a recommendation based on symptoms
pair<string, string> generateRecommendation(const vector<string>& symptoms) {
    // Improved rule-based system with more specific disease-symptom associations
    if (find(symptoms.begin(), symptoms.end(), "fever") != symptoms.end() &&
        find(symptoms.begin(), symptoms.end(), "cough") != symptoms.end() &&
        find(symptoms.begin(), symptoms.end(), "sore throat") != symptoms.end()) {
        return make_pair("Possible Disease: Common Cold", "Recommended Food: Warm broths, garlic, and ginger.");
    } else if (find(symptoms.begin(), symptoms.end(), "fever") != symptoms.end() &&
               find(symptoms.begin(), symptoms.end(), "headache") != symptoms.end() &&
               find(symptoms.begin(), symptoms.end(), "muscle aches") != symptoms.end()) {
        return make_pair("Possible Disease: Flu", "Recommended Food: Light soups, herbal tea, and hydration.");
    } else if (find(symptoms.begin(), symptoms.end(), "fatigue") != symptoms.end() &&
               find(symptoms.begin(), symptoms.end(), "weakness") != symptoms.end() &&
               find(symptoms.begin(), symptoms.end(), "loss of appetite") != symptoms.end()) {
        return make_pair("Possible Disease: Anemia", "Recommended Food: Iron-rich foods like spinach, lentils, and red meat.");
    } else if (find(symptoms.begin(), symptoms.end(), "diarrhea") != symptoms.end() &&
               find(symptoms.begin(), symptoms.end(), "abdominal pain") != symptoms.end() &&
               find(symptoms.begin(), symptoms.end(), "nausea") != symptoms.end()) {
        return make_pair("Possible Disease: Food Poisoning", "Recommended Food: Clear liquids like water and broth.");
    } else if (find(symptoms.begin(), symptoms.end(), "chest pain") != symptoms.end() &&
               find(symptoms.begin(), symptoms.end(), "shortness of breath") != symptoms.end()) {
        return make_pair("Possible Disease: Heart Attack or Pulmonary Embolism", "Recommended Food: Consult a doctor immediately.");
    } else if (find(symptoms.begin(), symptoms.end(), "severe headache") != symptoms.end() &&
               find(symptoms.begin(), symptoms.end(), "stiff neck") != symptoms.end()) {
        return make_pair("Possible Disease: Meningitis", "Recommended Food: Hydration, consult a doctor immediately.");
    } else if (find(symptoms.begin(), symptoms.end(), "sudden vision loss") != symptoms.end()) {
        return make_pair("Possible Disease: Stroke or Glaucoma", "Recommended Food: Consult a doctor immediately.");
    } else if (find(symptoms.begin(), symptoms.end(), "severe abdominal pain") != symptoms.end()) {
        return make_pair("Possible Disease: Appendicitis or Kidney Stones", "Recommended Food: Consult a doctor immediately.");
    } else {
        // Default recommendation based on symptoms
        if (find(symptoms.begin(), symptoms.end(), "fever") != symptoms.end()) {
            return make_pair("Possible Disease: Fever", "Recommended Food: Light soups, herbal tea, and hydration.");
        } else if (find(symptoms.begin(), symptoms.end(), "fatigue") != symptoms.end()) {
            return make_pair("Possible Disease: Fatigue", "Recommended Food: Whole grains, nuts, and fresh fruits.");
        } else if (find(symptoms.begin(), symptoms.end(), "cold") != symptoms.end()) {
            return make_pair("Possible Disease: Cold", "Recommended Food: Warm broths, garlic, and ginger.");
        } else if (find(symptoms.begin(), symptoms.end(), "cough") != symptoms.end()) {
            return make_pair("Possible Disease: Cough", "Recommended Food: Honey, ginger tea, and warm liquids.");
        } else {
            return make_pair("", "Recommended Food: Balanced diet including fruits, vegetables, and protein.");
        }
    }
}

// Function to search CSV for disease based on symptoms
void searchDiseaseBySymptoms(const vector<vector<string>>& data, const vector<string>& symptoms) {
    bool found = false;
    vector<string> lowerSymptoms;

    // Convert symptoms to lowercase and trim
    for (const auto& symptom : symptoms) {
        lowerSymptoms.push_back(toLowerCase(trim(symptom)));
    }

    cout << "Entered Symptoms: ";
    for (const auto& symptom : lowerSymptoms) {
        cout << symptom << " ";
    }
    cout << endl;

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
            cout << "Possible Disease: " << row[1] << endl;
            cout << "Recommended Foods: " << row[2] << endl;
            break;
        }
    }

    // If no match found in CSV, use the generateRecommendation function
    if (!found) {
        auto recommendation = generateRecommendation(symptoms);
        cout << recommendation.first << endl;
        cout << recommendation.second << endl;
    }
}
// Main function to test the code
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