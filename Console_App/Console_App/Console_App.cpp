// Console_App.cpp

/***
* Не реализовано:
* - Таймер проверки файлов
* - Удаление файлов
* - Глобальная критическая секция
*/

#include <fstream>
#include <string>
#include <Windows.h>
#include <random>

// Debug
#include <iostream>

// Подключаем класс с 64-битной переменной "id" типа long long
#include "ThreadEntity.h"

// Определяем количество потоков
#define MAX_THREADS 2

using namespace std;

DWORD WINAPI GetFileList(LPVOID lpParam);
int GetRandomInt(int x, int y);

int main()
{
    ThreadEntity threadEntity1, threadEntity2;
    ThreadEntity* poolThreadEntities[] = { &threadEntity1, &threadEntity2 };
    threadEntity1.id = 19000111000111;
    threadEntity2.id = 29111000111000;

    HANDLE threadArray[MAX_THREADS];
    
    // Создание потоков и передача им объектов
    for (int i = 0; i < MAX_THREADS; i++)
    {
        threadArray[i] = CreateThread(
            nullptr,
            0,
            GetFileList,
            poolThreadEntities[i],
            0,
            nullptr
        );

        // Проверяем актуальность потоков
        if (threadArray[i] == nullptr)
        {
            ExitProcess(3);
        }
    }

    // Ожидаем завершение потоков
    WaitForMultipleObjects(2, threadArray, true, INFINITE);
}

// Функция потока
DWORD WINAPI GetFileList(LPVOID lpParam)
{
    // Адаптируем lpParameter
    ThreadEntity* instance = static_cast<ThreadEntity*>(lpParam);

    // Случайная пауза
    Sleep(GetRandomInt(2, 4));

    // Поиск файлов в директории
    string filePath = "./files";
    string fileExtension = "txt";
    string searchField = filePath + "\\*";

    ofstream logfile(filePath + "/list.log");

    WIN32_FIND_DATAA fileData;
    HANDLE hFind = FindFirstFileA(searchField.c_str(), &fileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        cout << "File search error." << endl;
        return 1;
    }
    else
    {
        // Проверяем файлы
        do
        {
            const string FILE_NAME = fileData.cFileName;
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // Исключаем все файлы, кроме целевого расширения
                if (FILE_NAME.substr(FILE_NAME.find_last_of(".") + 1) == fileExtension)
                {
                    logfile << FILE_NAME << endl;
                    logfile << instance->id << endl;
                }
            }
        } while (FindNextFileA(hFind, &fileData) != 0);

        FindClose(hFind);
    }

    logfile.close();

    return 0;
}

// Генерируем случайное число
int GetRandomInt(int x, int y)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(x, y);

    return dis(gen);
}
