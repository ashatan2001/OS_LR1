#include <iostream>
#include <Windows.h>
#include <string>
#include <map>
#pragma

using namespace std;


void menu();
void getListDrives();
void getDriveInfo();
void createDirectory();
void removeDirectory();
void createFile();
void copyFile();
void moveFile();
void getFileAttributes();
void setFileAttributes();
void getFileInformationByHandle();
void getFileTime(); 
void setFileTime();


int main() {
	int item = 0;
    
    do {
        menu();
        while (!(cin >> item) || (cin.peek() != '\n'))
        {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Input error. Repeat...\n";
        }
        system("cls");

        switch (item) {
        case 1:
            cout << "List of drives\n" << endl;
            getListDrives();
            break;
        case 2:
            cout << "Drive information\n" << endl;
            getDriveInfo();
            break;
        case 3:
            cout << "Create directory\n" << endl;
            createDirectory();
            break;
        case 4:
            cout << "Remove directory\n" << endl;
            removeDirectory();
            break;
        case 5:
            cout << "Create file\n" << endl;
            createFile();
            break;
        case 6:
            cout << "Copy file\n" << endl;
            copyFile();
            break;
        case 7:
            cout << "Move file\n" << endl;
            moveFile();
            break;
        case 8:
            cout << "Get file attributes\n" << endl;
            getFileAttributes();
            break;
        case 9:
            cout << "Set file attributes\n" << endl;
            setFileAttributes();
            break;
        case 10:
            cout << "Get file information By Handle\n" << endl;
            getFileInformationByHandle();
            break;
        case 11:
            cout << "Get file time\n" << endl;
            getFileTime();
            break;
        case 12:
            cout << "Set file time\n" << endl;
            setFileTime();
            break;
        case 0:
            break;
        default:
            cout << "Non-existent menu item";
            break;
        }

        cout << endl;
        system("pause");
        system("cls");
    } while (item);

	return 0;
 }

void menu() {
    cout << "1. List of drives" << endl;
    cout << "2. Drive information" << endl;
    cout << "3. Create directory" << endl;
    cout << "4. Remove directory" << endl;
    cout << "5. Create file" << endl;
    cout << "6. Copy file" << endl;
    cout << "7. Move file" << endl;
    cout << "8. Get file attributes" << endl;
    cout << "9. Set file attributes" << endl;
    cout << "10. Get file information by handle" << endl;
    cout << "11. Get file time" << endl;
    cout << "12. Set file time" << endl;
    cout << "0. Exit" << endl << endl;

    cout << "Choose item: ";
}

void getListDrives() {
    DWORD dr = GetLogicalDrives();

    cout << "Logical drivers: " << endl;
    for (int i = 0; i < 26; i++) {
        if (((dr >> i) & 1) == 1) {
            cout << char('A' + i) << endl;
        }
    }
}

void getDriveInfo() {
    map <unsigned long, string> typeTable = { {0, "UNKNOWN"},
                                              {1, "NO ROOT DIRECTORY"},
                                              {DRIVE_REMOVABLE, "REMOVABLE DRIVE"},
                                              {DRIVE_FIXED, "HARD DRIVE"},
                                              {DRIVE_REMOTE, "REMOTE DRIVE"},
                                              {DRIVE_CDROM, "CD ROM"},
                                              {DRIVE_RAMDISK, "RAM"} };

    map <unsigned long, string> fsFlagsTable = {{FILE_CASE_SENSITIVE_SEARCH, "FILE_CASE_SENSITIVE_SEARCH"},
                                                {FILE_CASE_PRESERVED_NAMES, "FILE_CASE_PRESERVED_NAMES"},
                                                {FILE_UNICODE_ON_DISK, "FILE_UNICODE_ON_DISK"},
                                                {FILE_PERSISTENT_ACLS, "FILE_PERSISTENT_ACLS"},
                                                {FILE_FILE_COMPRESSION, "FILE_FILE_COMPRESSION"},
                                                {FILE_VOLUME_QUOTAS, "FILE_VOLUME_QUOTAS"},
                                                {FILE_SUPPORTS_SPARSE_FILES, "FILE_SUPPORTS_SPARSE_FILES"},
                                                {FILE_NAMED_STREAMS, "FILE_NAMED_STREAMS"},
                                                {FILE_VOLUME_IS_COMPRESSED, "FILE_VOLUME_IS_COMPRESSED"},
                                                {FILE_SUPPORTS_OBJECT_IDS, "FILE_SUPPORTS_OBJECT_IDS"},
                                                {FILE_SUPPORTS_ENCRYPTION, "FILE_SUPPORTS_ENCRYPTION"},
                                                {FILE_NAMED_STREAMS, "FILE_NAMED_STREAMS"},
                                                {FILE_READ_ONLY_VOLUME, "FILE_READ_ONLY_VOLUME"},
                                                {FILE_SEQUENTIAL_WRITE_ONCE, "FILE_SEQUENTIAL_WRITE_ONCE"},
                                                {FILE_SUPPORTS_TRANSACTIONS, "FILE_SUPPORTS_TRANSACTIONS"},
                                                {FILE_SUPPORTS_HARD_LINKS, "FILE_SUPPORTS_HARD_LINKS"},
                                                {FILE_SUPPORTS_EXTENDED_ATTRIBUTES, "FILE_SUPPORTS_EXTENDED_ATTRIBUTES"},
                                                {FILE_SUPPORTS_OPEN_BY_FILE_ID, "FILE_SUPPORTS_OPEN_BY_FILE_ID"},
                                                {FILE_SUPPORTS_USN_JOURNAL, "FILE_SUPPORTS_USN_JOURNAL"},
                                                {FILE_SUPPORTS_BLOCK_REFCOUNTING, "FILE_SUPPORTS_BLOCK_REFCOUNTING"} };

    wchar_t drive[MAX_PATH],
        volumeNameBuffer[MAX_PATH], 
        fileSystemNameBuffer[MAX_PATH];
    DWORD volumeSerialNumber,
        maxComponentLength,
        fileSystemFlags,
        sectorsPerCluster,
        bytesPerSector,
        numberOfFreeClusters,
        totalNumberOfClusters;

    getListDrives();
    cout << "\nChoose drive: ";
    wcin >> drive;
    wcscat_s(drive, L":\\");
    system("cls");

    wcout << "Information About Drive: " << drive << endl << endl;
    cout << "Drive Type: " << typeTable[GetDriveType(drive)] << endl << endl;

    if (GetVolumeInformation(drive, volumeNameBuffer, sizeof(volumeNameBuffer),
        &volumeSerialNumber, &maxComponentLength, &fileSystemFlags,
        fileSystemNameBuffer, sizeof(fileSystemNameBuffer)))
    {
        wcout << "Drive Volume Information:" << endl;
        wcout << "  Volume Name Buffer: " << volumeNameBuffer << endl;
        wcout << "  Volume Serial Number: " << volumeSerialNumber << endl;
        wcout << "  Maximum Component Length: " << maxComponentLength << endl;

        wcout << "  File System Flag: " << fileSystemFlags << endl;
        cout << "  File System Flags: " << endl;
        for (auto const& f : fsFlagsTable)
            if (fileSystemFlags & f.first)
                cout << "    " << f.second << endl;

        wcout << "  File System Name Buffer: " << fileSystemNameBuffer << endl << endl;
    } 
    else cout << "Error get Drive Volume Information" << endl;

    if (GetDiskFreeSpace(drive, &sectorsPerCluster, &bytesPerSector, 
        &numberOfFreeClusters, &totalNumberOfClusters))
    {
        wcout << "Drive Memory Information:" << endl;
        wcout << "  Sectors Per Cluster: " << sectorsPerCluster << endl;
        wcout << "  Bytes Per Sector: " << bytesPerSector << endl;
        wcout << "  Number Of Free Clusters: " << numberOfFreeClusters << endl;
        wcout << "  Total Number Of Clusters: " << totalNumberOfClusters << endl;
        int freeSpace = numberOfFreeClusters / 1024 * sectorsPerCluster / 1024 *
            bytesPerSector / 1024;
        int totalSpace = totalNumberOfClusters / 1024 * sectorsPerCluster /1024 *
            bytesPerSector / 1024;
        wcout << "  Free space: " << freeSpace << " GB" << endl;
        wcout << "  Total space: " << totalSpace << " GB" << endl;
    }
    else cout << "Error get Drive Memory Information" << endl;
}

void createDirectory() {
    wchar_t path[MAX_PATH];

    cout << "Enter a path to create directory: ";
    wcin >> path;
    
    if (CreateDirectory(path, NULL))
        cout << "Directory create" << endl;
    else
        cout << "Error create directory" << endl;
}

void removeDirectory() {
    wchar_t path[MAX_PATH];

    cout << "Enter a path to remove directory: ";
    wcin >> path;

    if (RemoveDirectory(path))
        cout << "Directory remove" << endl;
    else
        cout << "Error remove directory" << endl;
}

void createFile() {
    wchar_t path[MAX_PATH];

    cout << "Enter a path to create file: ";
    wcin >> path;

    HANDLE hFile = CreateFile(path, FILE_ADD_FILE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
    
    if (hFile != INVALID_HANDLE_VALUE) {
        cout << "File create" << endl;
        CloseHandle(hFile);
    }
    else
        cout << "Error create file" << endl;
}

void copyFile() {
    wchar_t oldPath[MAX_PATH];
    wchar_t newPath[MAX_PATH];
    DWORD error;
    int item;

    cout << "Enter the current file path: ";
    wcin >> oldPath;
    cout << "Enter a new file path: ";
    wcin >> newPath;

    CopyFile(oldPath, newPath, true);
    error = GetLastError();

    if (!error)
        cout << "File copy";
    else {
        if (error == 80) {
            system("cls");
            do {
                cout << "The file exist" << endl << endl;
                cout << "Overwrite the file?" << endl;
                cout << "  1. Yes" << endl;
                cout << "  0. No" << endl << endl;
                cout << "Choose an action: ";

                cin >> item;

                if (item != 0 && item != 1) {
                    cout << "\nInput error. Repeat..." << endl;
                    system("pause");
                    system("cls");
                }
            } while (item != 0 && item != 1);

            if (item == 1) {
                CopyFile(oldPath, newPath, false);
                cout << "\nFile copy";
            }   
        }
        else
            cout << "\nError copy file";
    }
}

void moveFile() {
    wchar_t oldPath[MAX_PATH];
    wchar_t newPath[MAX_PATH];
    DWORD error;
    int item;

    cout << "Enter the current file path: ";
    wcin >> oldPath;
    cout << "Enter a new file path: ";
    wcin >> newPath;

    MoveFile(oldPath, newPath);
    error = GetLastError();
   
    if (!error)
        cout << "File move";
    else {
        if (error == 183) {
            system("cls");
            do {
                cout << "The file exist" << endl << endl;
                cout << "Overwrite the file?" << endl;
                cout << "  1. Yes" << endl;
                cout << "  0. No" << endl << endl;
                cout << "Choose an action: ";

                cin >> item;

                if (item != 0 && item != 1) {
                    cout << "\nInput error. Repeat..." << endl;
                    system("pause");
                    system("cls");
                }
            } while (item != 0 && item != 1);

            if (item == 1) {
                MoveFileEx(oldPath, newPath, MOVEFILE_REPLACE_EXISTING);
                cout << "\nFile move";
            }
        }
        else
            cout << "\nError move file";
    }
}

void getFileAttributes() {
    wchar_t path[MAX_PATH];
    map <unsigned long, string> attrs = {   {FILE_ATTRIBUTE_ARCHIVE, "ARCHIVE"}, // Архивный
                                                {FILE_ATTRIBUTE_COMPRESSED, "COMPRESSED"}, // Сжатый
                                                {FILE_ATTRIBUTE_DEVICE, "DEVICE"}, // Зарезервированный, не используется
                                                {FILE_ATTRIBUTE_DIRECTORY, "DIRECTORY"}, // Дескриптор индефицирует директорию
                                                {FILE_ATTRIBUTE_ENCRYPTED, "ENCRYPTED"}, // Зашифрованный
                                                {FILE_ATTRIBUTE_HIDDEN, "HIDDEN"}, // Скрытый
                                                {FILE_ATTRIBUTE_NORMAL, "NORMAL"}, // Не имеет других установленных атрибутов
                                                {FILE_ATTRIBUTE_NOT_CONTENT_INDEXED, "NOT CONTENT INDEXED"}, // Не индексирован 
                                                {FILE_ATTRIBUTE_OFFLINE, "OFFLINE"}, // Данные доступны не сразу
                                                {FILE_ATTRIBUTE_READONLY, "READONLY"}, // Только для чтения
                                                {FILE_ATTRIBUTE_REPARSE_POINT, "REPARSE_POINT"}, // Имеет связаную точку повторной обработки
                                                {FILE_ATTRIBUTE_SPARSE_FILE, "SPARSE FILE"}, // Разреженный
                                                {FILE_ATTRIBUTE_SYSTEM, "SYSTEM"}, // Частично или исключительно используется ОС
                                                {FILE_ATTRIBUTE_TEMPORARY, "TEMPORARY"} }; // Для временного хранения

    cout << "Enter a file path: ";
    wcin >> path;

    DWORD dwAttrs = GetFileAttributes(path);
    if (dwAttrs != INVALID_FILE_ATTRIBUTES)
        cout << "File Attribute: " << attrs[dwAttrs] << endl;
    else
        cout << "Error get file attributes" << endl;
}

void setFileAttributes() {
    wchar_t path[MAX_PATH];
    string attr;
    bool isAttr;
    map <string, unsigned long> attrs = { {"ARCHIVE", FILE_ATTRIBUTE_ARCHIVE},
                                                {"COMPRESSED", FILE_ATTRIBUTE_COMPRESSED},
                                                {"DEVICE", FILE_ATTRIBUTE_DEVICE},
                                                {"DIRECTORY", FILE_ATTRIBUTE_DIRECTORY},
                                                {"ENCRYPTED", FILE_ATTRIBUTE_ENCRYPTED},
                                                {"HIDDEN", FILE_ATTRIBUTE_HIDDEN},
                                                {"NORMAL", FILE_ATTRIBUTE_NORMAL},
                                                {"NOT CONTENT INDEXED", FILE_ATTRIBUTE_NOT_CONTENT_INDEXED},
                                                {"OFFLINE", FILE_ATTRIBUTE_OFFLINE},
                                                {"READONLY", FILE_ATTRIBUTE_READONLY},
                                                {"REPARSE_POINT", FILE_ATTRIBUTE_REPARSE_POINT},
                                                {"SPARSE FILE", FILE_ATTRIBUTE_SPARSE_FILE},
                                                {"SYSTEM", FILE_ATTRIBUTE_SYSTEM},
                                                {"TEMPORARY", FILE_ATTRIBUTE_TEMPORARY} }; 

    cout << "Enter a file path: ";
    wcin >> path;
    system("cls");

    
    map <string, unsigned long> :: iterator it = attrs.begin();

    // Вывод списка атрибутов и выбор одного из них
    do {
        cout << "File attributes: " << endl;

        for (it = attrs.begin(); it != attrs.end(); it++) {
            cout << "  " << it->first << endl;
        }

        cout << "\nEnter attribute: ";
        cin >> attr;

        isAttr = attrs.find(attr) != attrs.end();
        if (!isAttr) {
            cout << "Erorr in attribute selection. Repeat..." << endl;
            system("pause");
            system("cls");
        }   
    } while (!isAttr);

    if (SetFileAttributes(path, attrs[attr]))
        cout << "Set file attribute" << endl;
    else
        cout << "Error set file attributes" << endl;
}

void getFileInformationByHandle() {
    wchar_t path[MAX_PATH];
    HANDLE hFile;
    BY_HANDLE_FILE_INFORMATION fileInfo;
    SYSTEMTIME st;
    FILETIME ft;
    
    cout << "Enter a file path: ";
    wcin >> path;

    hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

    if (GetFileInformationByHandle(hFile, &fileInfo)) {
        cout << "\nFile information by Handle:" << endl;
        cout << "  File Attributes:" << fileInfo.dwFileAttributes << endl;
        cout << "  Volume Serial Number: " << fileInfo.dwVolumeSerialNumber << endl;

        ft = fileInfo.ftCreationTime;
        FileTimeToLocalFileTime(&ft, &ft);
        FileTimeToSystemTime(&ft, &st);
        printf("  Creation Time: %02d-%02d-%04d  %02d:%02d:%02d.%03d\n",
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        
        ft = fileInfo.ftLastAccessTime;
        FileTimeToLocalFileTime(&ft, &ft);
        FileTimeToSystemTime(&ft, &st);
        printf("  Last Access Time: %02d-%02d-%04d  %02d:%02d:%02d.%03d\n",
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        
        ft = fileInfo.ftLastWriteTime;
        FileTimeToLocalFileTime(&ft, &ft);
        FileTimeToSystemTime(&ft, &st);
        printf("  Last Write Time: %02d-%02d-%04d  %02d:%02d:%02d.%03d\n",
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
       
        cout << "  File Index High: " << fileInfo.nFileIndexHigh << endl;
        cout << "  File Index Low: " << fileInfo.nFileIndexLow << endl;
        cout << "  File Size High: " << fileInfo.nFileSizeHigh << endl;
        cout << "  File Size Low: " << fileInfo.nFileSizeLow << endl;
        cout << "  Number Of Links: " << fileInfo.nNumberOfLinks << endl;

    }
    else {
        cout << "\nError get file information" << endl;
    }
        

    CloseHandle(hFile);
}

void getFileTime() {
    wchar_t path[MAX_PATH];
    FILETIME cTime, aTime, wTime; // creation, last access, write
    SYSTEMTIME st;
    HANDLE hFile;

    cout << "Enter a file path: ";
    wcin >> path;

    hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (GetFileTime(hFile, &cTime, &aTime, &wTime)) {
        cout << "\nFile time:" << endl;
        FileTimeToLocalFileTime(&cTime, &cTime);
        FileTimeToSystemTime(&cTime, &st);
        printf("  Creation time: %02d-%02d-%04d  %02d:%02d:%02d.%03d\n", 
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

        FileTimeToLocalFileTime(&aTime, &aTime);
        FileTimeToSystemTime(&aTime, &st);
        printf("  Last access time: %02d-%02d-%04d  %02d:%02d:%02d.%03d\n",
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

        FileTimeToLocalFileTime(&wTime, &wTime);
        FileTimeToSystemTime(&wTime, &st);
        printf("  Write time: %02d-%02d-%04d  %02d:%02d:%02d.%03d\n", 
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    }
    else {
        cout << "\nError get file time" << endl;
    }
    CloseHandle(hFile);
}

void setFileTime() {
    int itemTime;
    wchar_t path[MAX_PATH];
    FILETIME  ft;
    SYSTEMTIME st, time;
    HANDLE hFile;
    BOOL toST, toFT;

    cout << "Enter a file path: ";
    wcin >> path;
    system("cls");

    // Выбор времени
    do {
        cout << "Set the time to:\n" <<
            "  1. Creation time\n" <<
            "  2. Last access time\n" <<
            "  3. Write time\n" <<
            "  4. All time\n" <<
            "Choose the time to set: ";

        cin >> itemTime;

        if (!(itemTime > 0 && itemTime < 5)) {
            cout << "Timing error" << endl;
            system("pause");
        }

        system("cls");
    } while (!(itemTime > 0 && itemTime < 5));

    hFile = CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

    // Ввод и установка времени
    cout << "Enter the time in the format dd mm yyyy hh mm ss ms:" << endl;
    cin >> time.wDay >> time.wMonth >> time.wYear >> time.wHour >> 
        time.wMinute >> time.wSecond >> time.wMilliseconds;

    toST = TzSpecificLocalTimeToSystemTime(NULL, &time, &st);
    toFT = SystemTimeToFileTime(&st, &ft);

    switch(itemTime) {
        case 1:
            if (toST && toFT && SetFileTime(hFile, &ft, (LPFILETIME)NULL, (LPFILETIME)NULL)) {
                cout << "The create time is set" << endl;
            }
            else {
                cout << "Error set file time" << endl;
            }
            break;
        case 2:
            if (toST && toFT && SetFileTime(hFile, (LPFILETIME)NULL, &ft, (LPFILETIME)NULL)) {
                cout << "Last access time is set" << endl;
            }
            else {
                cout << "Error set file time" << endl;
            }
            break;
        case 3:
            if (toST && toFT && SetFileTime(hFile, (LPFILETIME)NULL, (LPFILETIME)NULL, &ft)) {
                cout << "Write time is set" << endl;
            }
            else {
                cout << "Error set file time" << endl;
            }
            break;
        case 4:
            if (toST && toFT && SetFileTime(hFile, &ft, &ft, &ft)) {
                cout << "All times are set" << endl;
            }
            else {
                cout << "Error set file time" << endl;
            }
            break;
    }

    CloseHandle(hFile);
}