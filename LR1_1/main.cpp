#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    std::ifstream file("D:/proga/Veni.doc", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "�� ������� ������� ����." << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end); // ��������� � ����� �����
    const int fileLength = file.tellg(); // �������� ������� �������, ������� ����� ����� �����
    file.close();

    if (fileLength == -1) {
        std::cerr << "�� ������� ���������� ����� �����." << std::endl;
        return 1;
    }

    std::cout << "����� ����� � ������: " << fileLength << std::endl;

    return 0;
}