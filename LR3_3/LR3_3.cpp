#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Функция для шифрования файла
void encrypt(const string& inputFileName, const string& outputFileName, const vector<int>& key) {
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile) {
        cerr << "Error: Unable to open input file: " << inputFileName << endl;
        return;
    }

    if (!outputFile) {
        cerr << "Error: Unable to create output file: " << outputFileName << endl;
        return;
    }

    char ch;
    int col = 0;
    vector<char> buffer(key.size(), 'z'); // Дополнительные символы 'z' для неполных столбцов

    while (inputFile.get(ch)) {
        buffer[col++] = ch;

        if (col == key.size()) {
            for (int k : key) {
                outputFile << buffer[k - 1];
            }
            col = 0;
        }
    }

    // Дописываем 'z' для завершения последнего столбца, если нужно
    if (col != 0) {
        for (int i = col; i < key.size(); ++i) {
            buffer[i] = 'z';
        }
        for (int k : key) {
            outputFile << buffer[k - 1];
        }
    }

    inputFile.close();
    outputFile.close();

    cout << "Encryption completed successfully." << endl;
}

// Функция для расшифрования файла
void decrypt(const string& inputFileName, const string& outputFileName, const vector<int>& key) {
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile) {
        cerr << "Error: Unable to open input file: " << inputFileName << endl;
        return;
    }

    if (!outputFile) {
        cerr << "Error: Unable to create output file: " << outputFileName << endl;
        return;
    }

    char ch;
    int col = 0;
    vector<char> buffer(key.size());

    while (inputFile.get(ch)) {
        buffer[key[col++] - 1] = ch;

        if (col == key.size()) {
            // Проверка на наличие 'z' в конце строки и удаление их
            while (!buffer.empty() && buffer.back() == 'z') {
                buffer.pop_back();
            }
            for (char c : buffer) {
                outputFile << c;
            }
            col = 0;
        }
    }

    inputFile.close();
    outputFile.close();

    cout << "Decryption completed successfully." << endl;
}

int main() {
    string inputFileName, outputFileName, keyFileName;

    cout << "Enter the input file name: ";
    cin >> inputFileName;

    cout << "Enter the output file name: ";
    cin >> outputFileName;

    cout << "Enter the key file name: ";
    cin >> keyFileName;

    // Загрузка ключа из файла
    ifstream keyFile(keyFileName);
    if (!keyFile) {
        cerr << "Error: Unable to open key file: " << keyFileName << endl;
        return 1;
    }

    vector<int> key;
    int keyValue;
    while (keyFile >> keyValue) {
        key.push_back(keyValue);
    }
    keyFile.close();

    // Проверка на корректность ключа
    if (key.size() != 5) {
        cerr << "Error: Key must contain exactly 5 values." << endl;
        return 1;
    }

    cout << "Choose mode: " << endl;
    cout << "1. Encryption" << endl;
    cout << "2. Decryption" << endl;
    int mode;
    cin >> mode;

    switch (mode) {
        case 1:
            encrypt(inputFileName, outputFileName, key);
            break;
        case 2:
            decrypt(inputFileName, outputFileName, key);
            break;
        default:
            cerr << "Error: Invalid mode selection." << endl;
            return 1;
    }

    return 0;
}