#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct TransactionRecord {
    vector<string> fields;
};

vector<string> splitCsvLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream stream(line);

    while (getline(stream, field, ',')) {
        fields.push_back(field);
    }

    return fields;
}

string joinCsvFields(const vector<string>& fields) {
    string line;

    for (size_t i = 0; i < fields.size(); ++i) {
        if (i > 0) {
            line += ',';
        }

        line += fields[i];
    }

    return line;
}

// Partition the records using TransactionDate as the pivot
int partition(vector<TransactionRecord>& records, int low, int high) {

    string pivot = records[high].fields[3];

    int i = low - 1;

    for (int j = low; j < high; ++j) {

        // Compare TransactionDate
        if (records[j].fields[3] <= pivot) {
            ++i;

            swap(records[i], records[j]);
        }
    }

    swap(records[i + 1], records[high]);

    return i + 1;
}

// Quick Sort
void quickSort(vector<TransactionRecord>& records, int low, int high) {

    if (low < high) {

        int pivotIndex = partition(records, low, high);

        quickSort(records, low, pivotIndex - 1);

        quickSort(records, pivotIndex + 1, high);
    }
}

int main(int argc, char* argv[]) {

    string inputPath =
        argc > 1 ? argv[1] : "bank_transactions_data_2.csv";

    string outputPath =
        argc > 2 ? argv[2] : "quick_sorted_bank_transactions_data_2.csv";

    // Open input CSV file
    ifstream inputFile(inputPath);

    if (!inputFile.is_open()) {
        cerr << "Failed to open input file: "
             << inputPath << endl;

        return 1;
    }

    // Read header
    string headerLine;

    if (!getline(inputFile, headerLine)) {
        cerr << "Input file is empty." << endl;
        return 1;
    }

    vector<TransactionRecord> records;
    string line;

    // Read all transaction records
    while (getline(inputFile, line)) {

        // Remove carriage return if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        vector<string> fields = splitCsvLine(line);

        // Dataset must have at least 4 columns
        if (fields.size() >= 4) {
            records.push_back({fields});
        }
    }

    // Apply Quick Sort
    if (!records.empty()) {

        quickSort(
            records,
            0,
            static_cast<int>(records.size()) - 1
        );
    }

    // Create output file
    ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        cerr << "Failed to create output file: "
             << outputPath << endl;

        return 1;
    }

    // Write header
    outputFile << headerLine << '\n';

    // Write sorted records
    for (const auto& record : records) {
        outputFile << joinCsvFields(record.fields) << '\n';
    }

    cout << "Quick Sort completed successfully." << endl;

    cout << "Sorted "
         << records.size()
         << " records by TransactionDate." << endl;

    cout << "Output file: "
         << outputPath << endl;

    return 0;
}
