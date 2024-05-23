// Console_App.cpp

/***
* �� �����������:
* - ������ �������� ������
* - �������� ������
* - ���������� ����������� ������
*/

#include <fstream>
#include <string>
#include <Windows.h>
#include <random>

// Debug
#include <iostream>

// ���������� ����� � 64-������ ���������� "id" ���� long long
#include "ThreadEntity.h"

// ���������� ���������� �������
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
    
    // �������� ������� � �������� �� ��������
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

        // ��������� ������������ �������
        if (threadArray[i] == nullptr)
        {
            ExitProcess(3);
        }
    }

    // ������� ���������� �������
    WaitForMultipleObjects(2, threadArray, true, INFINITE);
}

// ������� ������
DWORD WINAPI GetFileList(LPVOID lpParam)
{
    // ���������� lpParameter
    ThreadEntity* instance = static_cast<ThreadEntity*>(lpParam);

    // ��������� �����
    Sleep(GetRandomInt(2, 4));

    // ����� ������ � ����������
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
        // ��������� �����
        do
        {
            const string FILE_NAME = fileData.cFileName;
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // ��������� ��� �����, ����� �������� ����������
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

// ���������� ��������� �����
int GetRandomInt(int x, int y)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(x, y);

    return dis(gen);
}
