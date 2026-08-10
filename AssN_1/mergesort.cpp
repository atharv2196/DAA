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

void merge(vector<TransactionRecord>& records,
           int left, int mid, int right) {

    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<TransactionRecord> leftRecords(n1);
    vector<TransactionRecord> rightRecords(n2);

    for (int i = 0; i < n1; ++i) {
        leftRecords[i] = records[left + i];
    }

    for (int i = 0; i < n2; ++i) {
        rightRecords[i] = records[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {

        // fields[3] = TransactionDate
        if (leftRecords[i].fields[3] <= rightRecords[j].fields[3]) {
            records[k++] = leftRecords[i++];
        }
        else {
            records[k++] = rightRecords[j++];
        }
    }

    while (i < n1) {
        records[k++] = leftRecords[i++];
    }

    while (j < n2) {
        records[k++] = rightRecords[j++];
    }
}

void mergeSort(vector<TransactionRecord>& records,
               int left, int right) {

    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(records, left, mid);
    mergeSort(records, mid + 1, right);

    merge(records, left, mid, right);
}

int main(int argc, char* argv[]) {

    string inputPath =
        argc > 1 ? argv[1] : "bank_transactions_data_2.csv";

    string outputPath =
        argc > 2 ? argv[2] : "sorted_bank_transactions_data_2.csv";

    ifstream inputFile(inputPath);

    if (!inputFile.is_open()) {
        cerr << "Failed to open input file: "
             << inputPath << endl;
        return 1;
    }

    string headerLine;

    if (!getline(inputFile, headerLine)) {
        cerr << "Input file is empty." << endl;
        return 1;
    }

    vector<TransactionRecord> records;
    string line;

    while (getline(inputFile, line)) {

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        vector<string> fields = splitCsvLine(line);

        if (fields.size() >= 4) {
            records.push_back({fields});
        }
    }

    if (!records.empty()) {
        mergeSort(
            records,
            0,
            static_cast<int>(records.size()) - 1
        );
    }

    ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        cerr << "Failed to open output file: "
             << outputPath << endl;
        return 1;
    }

    outputFile << headerLine << '\n';

    for (const auto& record : records) {
        outputFile << joinCsvFields(record.fields) << '\n';
    }

    cout << "Sorted "
         << records.size()
         << " records by TransactionDate and wrote "
         << outputPath << endl;

    return 0;
}
