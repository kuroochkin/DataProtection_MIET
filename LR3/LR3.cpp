#include <iostream>
#include <fstream>
#include <cstdint>
#include <windows.h> // Для использования структур BITMAPFILEHEADER и BITMAPINFOHEADER

using namespace std;

// Функция для скрытия байта информации в пикселе
void hide_byte_into_pixel(RGBQUAD *pixel, uint8_t hide_byte) {
    // Cброс двух младших битов
    pixel->rgbBlue &= (0xFC);
    // Добавляем два старших бита из сообщения на место двух младших
    pixel->rgbBlue |= (hide_byte >> 6) & 0x3;
    pixel->rgbGreen &= (0xFC);
    pixel->rgbGreen |= (hide_byte >> 4) & 0x3;
    pixel->rgbRed &= (0xFC);
    pixel->rgbRed |= (hide_byte >> 2) & 0x3;
    pixel->rgbReserved &= (0xFC);
    pixel->rgbReserved |= (hide_byte) & 0x3;
    // Каждый из 4 байтов освобождает по 2 бита => освобждается 8 битов => мы шифруем 1 байт
}

// Функция для извлечения скрытого байта из пикселя
uint8_t extract_byte_from_pixel(const RGBQUAD *pixel) {
    uint8_t hide_byte = 0;
    // Извлекаются два младших бита из rgbBlue и занимают 2 последние позиции в hide_byte
    hide_byte |= (pixel->rgbBlue & 0x3) << 6;
    hide_byte |= (pixel->rgbGreen & 0x3) << 4;
    hide_byte |= (pixel->rgbRed & 0x3) << 2;
    hide_byte |= (pixel->rgbReserved & 0x3);
    return hide_byte;
}

int main() {
    const char *input_filename = "1.bmp";
    const char *output_filename = "out.bmp";
    const char *secret_message = "Secret message";
    ifstream input_file(input_filename, ios::binary);
    ofstream output_file(output_filename, ios::binary);

    if (!input_file.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    if (!output_file.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    // Чтение заголовков BMP файла
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    input_file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    input_file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    // Переписываем заголовки в выходной файл
    output_file.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    output_file.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    // Вычисляем размер изображения в байтах
    int image_size = info_header.biWidth * info_header.biHeight * (info_header.biBitCount / 8);
    char *image_data = new char[image_size];
    input_file.read(image_data, image_size);

    // Скрытие сообщения в изображении
    int message_index = 0;
    for (int i = 0; i < image_size; i += 4) {
        if (message_index < strlen(secret_message)) {
            hide_byte_into_pixel(reinterpret_cast<RGBQUAD*>(&image_data[i]), secret_message[message_index]);
            message_index++;
        } else {
            hide_byte_into_pixel(reinterpret_cast<RGBQUAD*>(&image_data[i]), 0xFF); // Признак конца сообщения
            break;
        }
    }

    // Записываем измененное изображение в выходной файл
    output_file.write(image_data, image_size);

    delete[] image_data;

    // Извлечение скрытого сообщения
    ifstream output_file_read(output_filename, ios::binary);
    if (!output_file_read.is_open()) {
        cerr << "Error: Unable to open output file for reading." << endl;
        return 1;
    }

    // Пропускаем заголовки BMP файла
    output_file_read.seekg(sizeof(file_header) + sizeof(info_header), ios::beg);

    // Извлекаем информацию из изображения
    char pixel_data[4];
    string extracted_message = "";
    // Считываем по 4 байта (1 пиксель)
    while (output_file_read.read(pixel_data, 4)) {
        // Из одного пикселя (4 байт), получаем скрытую информацию в 1 байт
        uint8_t extracted_byte = extract_byte_from_pixel(reinterpret_cast<RGBQUAD*>(pixel_data));
        if (extracted_byte == 0xFF) {
            break; // Достигнут признак конца сообщения
        }
        extracted_message += extracted_byte;
    }

    cout << "Extracted message: " << extracted_message << endl;

    input_file.close();
    output_file.close();
    output_file_read.close();

    return 0;
}