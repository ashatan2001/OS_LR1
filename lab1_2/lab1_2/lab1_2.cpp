#include<iostream>
#include<windows.h>
#include<fileapi.h>
#include <iomanip>

#define CLUSTER_SIZE 4096 // размер кластера

using namespace std;

void inputData();
void preparationData(wchar_t*, wchar_t*, int, int);
void copyFileOverlapped(int, DWORD, DWORD64, DWORD64, DWORD, HANDLE, HANDLE);
void readFileOverlapped(int, DWORD, DWORD64, DWORD64, HANDLE, OVERLAPPED*, char**);
void writeFileOverlapped(int, DWORD, DWORD64, DWORD64, HANDLE, OVERLAPPED*, char**);
void outputInfo(int, DWORD, DWORD64, int);


int callback = 0;

// Функция завершения, вызывается при завершении операции в/в (ReadFileEx и WriteFileEx)
VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    callback++;
}

int main() {

    int item = 0;

    do {
        cout << "Any number - Start Asynchronous File Copy" << endl;
        cout << "0 - Exit" << endl << endl;
        cout << "Choose item: ";

        while (!(cin >> item) || (cin.peek() != '\n'))
        {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Input error! Repeat please...\n";
        }

        if (item) {
            system("cls");
            inputData();
        }

        system("cls");
    } while (item);

    return 0;
}

void inputData() {

    wchar_t inPath[MAX_PATH], outPath[MAX_PATH];
    int nOperations = 0, factorBlock = 0;

    cout << "Enter the file path: ";
    wcin >> inPath;

    cout << "Enter the file copy path: ";
    wcin >> outPath;

    cout << "Enter the number of overlapping I/O operations: ";
    cin >> nOperations;

    cout << "Enter the block size factor: ";
    cin >> factorBlock;

    preparationData(inPath, outPath, nOperations, factorBlock);
}

void preparationData(wchar_t* inPath, wchar_t* outPath, int nOperations, int factorBlock) {

    char* readingBytes, * pointerToBeginning;
    HANDLE inHandle, outHandle;
    DWORD blockSize;
    DWORD low, high;
    DWORD64 fileSize, fileSizeInBlocks = 0;

    inHandle = CreateFile(inPath, GENERIC_READ, 0,
        NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
    outHandle = CreateFile(outPath, GENERIC_WRITE, 0,
        NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

    // Размер блока
    blockSize = factorBlock * CLUSTER_SIZE;

    low = GetFileSize(inHandle, &high);
    fileSize = ((DWORD64)high << 32) + low; // 64-разрядный размер файла

    //Размер файла, кратный размеру блока ()
    while (fileSizeInBlocks < fileSize) {
        fileSizeInBlocks += blockSize;
    }

    copyFileOverlapped(nOperations, blockSize, fileSizeInBlocks,
        fileSize, high, inHandle, outHandle);
}

void copyFileOverlapped(int nOperations, DWORD blockSize, DWORD64 fileSizeInBlocks,
    DWORD64 fileSize, DWORD high, HANDLE inHandle, HANDLE outHandle) {

    DWORD64 curSize = 0;
    char** buffer = new char* [nOperations];
    OVERLAPPED* rOver = new OVERLAPPED[nOperations],
        * wOver = new OVERLAPPED[nOperations];

    for (int i = 0; i < nOperations; i++)
    {
        buffer[i] = new char[blockSize];

        rOver[i].OffsetHigh = wOver[i].OffsetHigh = ((DWORD64)(i * blockSize) >> 32);
        if (wOver[i].OffsetHigh)
            rOver[i].Offset = wOver[i].Offset = ((DWORD64)(i * blockSize)) - ((DWORD64)rOver[i].OffsetHigh << 32);
        else 
            rOver[i].Offset = wOver[i].Offset = (DWORD64)(i * blockSize);
        rOver[i].hEvent = wOver[i].hEvent = NULL;
    }

    cout << "\nCopying..." << endl;
    int sTime = GetTickCount();
    do {
        readFileOverlapped(nOperations, blockSize, fileSize, fileSizeInBlocks, inHandle, rOver, buffer);
        writeFileOverlapped(nOperations, blockSize, fileSize, fileSizeInBlocks, outHandle, wOver, buffer);

        curSize += (DWORD64)(blockSize * nOperations);
    } while (curSize < fileSizeInBlocks);

    int fTime = GetTickCount();

    long lHigh = (long)high;
    SetFilePointer(outHandle, fileSize, &lHigh, FILE_BEGIN);
    SetEndOfFile(outHandle);
    CloseHandle(inHandle);
    CloseHandle(outHandle);

    for (int i = 0; i < nOperations; i++)
        delete[] buffer[i];
    delete[] buffer;
    delete[] rOver;
    delete[] wOver;

    system("cls");

    outputInfo(nOperations, blockSize, fileSize, fTime - sTime);
}

void readFileOverlapped(int nOperations, DWORD blockSize, DWORD64 fileSize,
    DWORD64 fileSizeInBlocks, HANDLE hFile, OVERLAPPED* over, char** buffer) {

    int operCounter = 0;
    DWORD64 bytes = 0, hBytes = 0;

    for (int i = 0; i < nOperations && fileSizeInBlocks; i++) {
        operCounter++;
        ReadFileEx(hFile, buffer[i], blockSize, &over[i], FileIOCompletionRoutine);
        fileSizeInBlocks -= blockSize;
    }

    while (callback < operCounter)
        SleepEx(-1, true);

    for (int i = 0; i < nOperations; i++) {
        hBytes = bytes = ((DWORD64)over[i].Offset) + (DWORD64)(blockSize * nOperations) + ((DWORD64)over[i].OffsetHigh << 32);
        over[i].OffsetHigh = (hBytes >> 32);
        if (over[i].OffsetHigh) 
            over[i].Offset = bytes - ((DWORD64)over[i].OffsetHigh << 32);
        else 
            over[i].Offset = bytes;
    }

    callback = 0;
}

void writeFileOverlapped(int nOperations, DWORD blockSize, DWORD64 fileSize,
    DWORD64 fileSizeInBlocks, HANDLE hFile, OVERLAPPED* over, char** buffer) {

    int operCounter = 0;
    DWORD64 bytes = 0, hBytes = 0;

    for (int i = 0; i < nOperations && fileSizeInBlocks; i++) {
        operCounter++;
        WriteFileEx(hFile, buffer[i], blockSize, &over[i], FileIOCompletionRoutine);
        fileSizeInBlocks -= blockSize;
    }

    while (callback < operCounter)
        SleepEx(-1, true);
    
    for (int i = 0; i < nOperations; i++) {
        hBytes = bytes = ((DWORD64)over[i].Offset) + (DWORD64)(blockSize * nOperations) + ((DWORD64)over[i].OffsetHigh << 32);
        over[i].OffsetHigh = (hBytes >> 32);
        if (over[i].OffsetHigh) 
            over[i].Offset = bytes - ((DWORD64)over[i].OffsetHigh << 32);
        else
            over[i].Offset = bytes;
    }

    callback = 0;
}

void outputInfo(int nOperations, DWORD blockSize, DWORD64 fileSize, int time) {
    cout << "Information Received" << endl;
    cout << "  Number of overlapping I/O operations: " << nOperations << endl;
    cout << "  Block size: " << blockSize << endl;
    cout << "  File size: " << fileSize << endl;
    cout << "  Copy time: " << time << " ms" << endl;
    system("pause");
}