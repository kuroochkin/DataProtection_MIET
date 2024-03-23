#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, char* argv[])
{
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    std::string filename;
    std::cout << "Введите имя файла: ";
    std::cin >> filename;

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }

    std::unordered_map<unsigned char, int> byteFrequency;

    char byte;
    while (file.get(byte)) {
        byteFrequency[static_cast<unsigned char>(byte)]++;
    }

    file.close();

    std::cout << "Частота появления каждого байта в файле:" << std::endl;
    for (const auto& pair : byteFrequency) {
        std::cout << "Байт: " << static_cast<int>(pair.first) << ", Частота: " << pair.second << std::endl;
    }

    return 0;
}
