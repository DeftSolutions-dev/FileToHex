#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <Windows.h>

void fileToHex(const char* filename) {
    //Открываем файл с заданным именем в бинарном режиме
    std::ifstream file(filename, std::ios::binary);
    //Если файл не открылся, то просто иди нахуй :D
    if (!file) return;
    //Переходим в конец файла чтобы получить его размер, затем возвращаемся в начало файла
    file.seekg(0, std::ios::end);
    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    //Создаем буфер размером с файл и читаем в него данные из файла
    std::vector<uint8_t> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    //Создаем объект для записи шестнадцатеричных значений байтов
    std::ostringstream hex;
    hex << std::hex << std::setfill('0');
    //Преобразуем каждый байт в шестнадцатеричное значение и записываем его в объект hex byte 0xXX,0xXX
    for (size_t i = 0; i < buffer.size(); i++) {
        hex << "0x" << std::setw(2) << static_cast<int>(buffer[i]) << ",";
    }
    //Создаем файл с расширением ".txt" и записываем в него содержимое объекта hex
    std::ofstream hexFile(std::string(filename) + ".txt");
    hexFile << hex.str();
    hexFile.close();
    //Копируем hex byte в буфер обмена
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, hex.str().size() + 1);
        if (hMem != NULL) {
            LPSTR lpMem = static_cast<LPSTR>(GlobalLock(hMem));
            if (lpMem != NULL) {
                strcpy_s(lpMem, hex.str().size() + 1, hex.str().c_str());
                GlobalUnlock(hMem);
                SetClipboardData(CF_TEXT, hMem);
            }
        }
        CloseClipboard();
    }
}
int main() {
    //Запрашиваем имя файла, путь или просто перетащить
    std::cout << "Enter filename: ";
    std::string filename;
    std::getline(std::cin, filename);
    //Вызываем функцию для конвертации файла в шестнадцатеричное представление в формате Hex Byte 0xXX,0xXX
    fileToHex(filename.c_str());
    return 0;
}
