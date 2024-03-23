#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    std::ifstream file("D:/proga/Veni.doc", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end); // Переходим в конец файла
    const int fileLength = file.tellg(); // Получаем текущую позицию, которая равна длине файла
    file.close();

    if (fileLength == -1) {
        std::cerr << "Не удалось определить длину файла." << std::endl;
        return 1;
    }

    std::cout << "Длина файла в байтах: " << fileLength << std::endl;

    return 0;
}