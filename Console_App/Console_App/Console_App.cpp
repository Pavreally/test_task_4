// Console_App.cpp

#include <fstream>
#include <string>
#include <Windows.h>
#include <random>

// Debug
#include <iostream>

// 64-bit Number
#include "ThreadEntity.h"

// Set the Maximum Number of Threads
#define MAX_THREADS 2

using namespace std;

DWORD WINAPI GetFileList(LPVOID LpParam);
int GetRandomInt(int Start, int End);

int main()
{
    ThreadEntity threadEntity1, threadEntity2;
    ThreadEntity* poolThreadEntities[] = { &threadEntity1, &threadEntity2 };
    threadEntity1.id = 19000111000111;
    threadEntity2.id = 29111000111000;

    HANDLE threadArray[MAX_THREADS];
    
    // Create Threads
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

        // Checking the Thread Status
        if (threadArray[i] == nullptr)
        {
            ExitProcess(3);
        }
    }

    // Wait for the Array of Threads to Complete
    WaitForMultipleObjects(2, threadArray, true, INFINITE);
}

// Streaming Function
DWORD WINAPI GetFileList(LPVOID LpParam)
{
    // Convert Parameter
    ThreadEntity* Instance = static_cast<ThreadEntity*>(LpParam);

    // Get a Random Number
    Sleep(GetRandomInt(2, 4));

    // Create a Log File
    string FilePath = "./files";
    string FileExtension = "txt";
    string SearchField = FilePath + "\\*";

    ofstream LogFile(FilePath + "/list.log");

    WIN32_FIND_DATAA fileData;
    HANDLE hFind = FindFirstFileA(SearchField.c_str(), &fileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        cout << "File search error." << endl;
        return 1;
    }
    else
    {
        // Send Data to a File
        do
        {
            const string FILE_NAME = fileData.cFileName;
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // Search for Only the Specified Extension
                if (FILE_NAME.substr(FILE_NAME.find_last_of(".") + 1) == FileExtension)
                {
                    LogFile << FILE_NAME << endl;
                    LogFile << Instance->id << endl;
                }
            }
        } while (FindNextFileA(hFind, &fileData) != 0);

        FindClose(hFind);
    }

    LogFile.close();

    return 0;
}

int GetRandomInt(int Start, int End)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(Start, End);

    return dis(gen);
}
