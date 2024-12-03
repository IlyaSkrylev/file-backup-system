#include <Windows.h>
#include <tchar.h>
#include "mainFunctions.h"

#define BIN_FILE_PATH "datafile.bin"
#define LOG_FILE_PATH "logfile.bin"
#define MAX_RECORDS 500

LPTSTR binFilePath;
LPTSTR logFilePath;

void TakeBinFilePath() {
    LPTSTR currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    binFilePath = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
    _stprintf_s(binFilePath, MAX_PATH, _T("%s\\%s"), currentDir, _T(BIN_FILE_PATH));
}

void FreeMemoryBinFile() {
    free(binFilePath);
}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    int count = 0;
    struct dataAboutFile* filesToCopy = GetFilesToCopy(&count);
    CopyFiles(filesToCopy, count);

    free(filesToCopy);
}

int ChangeRecordInBinFile(const struct dataAboutFile* record, int numberOfRecord) {
    int recordCount = 0;
    struct dataAboutFile* data = GetDataFromBinFile(&recordCount);

    if (data == NULL)
        return -1;

    data[numberOfRecord] = *record;

    HANDLE hFile = CreateFile(binFilePath, GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    SetFilePointer(hFile, 0, NULL, FILE_END);
    DWORD bytesWritten;
    if (!WriteFile(hFile, data, sizeof(struct dataAboutFile) * recordCount, &bytesWritten, NULL)) {
        return -1;
    }
    CloseHandle(hFile);
    free(data);
    return 0;
}

int RecreateBinFile(struct dataAboutFile* data, int count) {
    HANDLE hFile = CreateFile(binFilePath, GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    SetFilePointer(hFile, 0, NULL, FILE_END);
    DWORD bytesWritten;
    if (!WriteFile(hFile, data, sizeof(struct dataAboutFile) * count, &bytesWritten, NULL)) {
        return -1;
    }
    CloseHandle(hFile);
    return 0;
}

int DeleteRecordFromBinFile(int index) {
    int recordCount = 0;
    struct dataAboutFile* data = GetDataFromBinFile(&recordCount);

    HANDLE hFile = CreateFile(binFilePath, GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }

    DWORD bytesWritten;
    for (int i = 0; i < recordCount; i++) {
        if (i != index) {
            SetFilePointer(hFile, 0, NULL, FILE_END);
            if (!WriteFile(hFile, &data[i], sizeof(struct dataAboutFile), &bytesWritten, NULL)) {
                return -1;
            }
        }
    }
    CloseHandle(hFile);
    free(data);
    return 0;
}

int WriteDataIntoBinFile(const struct dataAboutFile* data) {
    int recordCount;
    if ((recordCount = RecordCountInBinFile()) == -1)
        return -1;

    if (recordCount >= MAX_RECORDS)
        return 1;

    HANDLE hFile = CreateFile(binFilePath, GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return -1;

    SetFilePointer(hFile, 0, NULL, FILE_END);
    DWORD bytesWritten;
    if (!WriteFile(hFile, data, sizeof(struct dataAboutFile), &bytesWritten, NULL))
        return -1;

    CloseHandle(hFile);
    return 0;
}

struct dataAboutFile* GetDataFromBinFile(int* recordCount) {
    struct dataAboutFile* data = (struct dataAboutFile*)malloc(sizeof(struct dataAboutFile) * MAX_RECORDS);
    HANDLE hFile = CreateFile(binFilePath, GENERIC_READ,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    DWORD bytesRead;
    BOOL result = ReadFile(hFile, data,
        sizeof(struct dataAboutFile) * MAX_RECORDS, &bytesRead, NULL);

    if (!result || bytesRead % sizeof(struct dataAboutFile) != 0) {
        CloseHandle(hFile);
        free(data);
        return NULL;
    }

    *recordCount = bytesRead / sizeof(struct dataAboutFile);
    CloseHandle(hFile);
    return data;
}

int RecordCountInBinFile() {
    struct dataAboutFile* data = (struct dataAboutFile*)malloc(sizeof(struct dataAboutFile) * MAX_RECORDS);
    HANDLE hFile = CreateFile(binFilePath, GENERIC_READ,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }

    DWORD bytesRead;
    BOOL result = ReadFile(hFile, data,
        sizeof(struct dataAboutFile) * MAX_RECORDS, &bytesRead, NULL);

    if (!result || bytesRead % sizeof(struct dataAboutFile) != 0) {
        CloseHandle(hFile);
        return -1;
    }

    size_t recordCount = bytesRead / sizeof(struct dataAboutFile);

    CloseHandle(hFile);
    free(data);
    return recordCount;
}

void TakeLogFile() {
    LPTSTR currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    logFilePath = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
    _stprintf_s(logFilePath, MAX_PATH, _T("%s\\%s"), currentDir, _T(LOG_FILE_PATH));
}

void FreeMemoryLogFile() {
    free(logFilePath);
}

void AddRecordToLogFile(const TCHAR* filePath, const TCHAR* destPath) {
    HANDLE hFile = CreateFile(logFilePath, GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return -1;

    struct logsInfo data;
    _tcscpy_s(data.fSource, MAX_PATH, filePath);
    _tcscpy_s(data.fDest, MAX_PATH, destPath);

    SYSTEMTIME st;
    GetLocalTime(&st);
    LPTSTR time = malloc(sizeof(TCHAR) * 50);

    _stprintf_s(time, 50, _T("%d-%02d-%02d   %02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
    _tcscpy_s(data.copyTime, MAX_PATH, time);
    SetFilePointer(hFile, 0, NULL, FILE_END);
    DWORD bytesWritten;
    if (!WriteFile(hFile, &data, sizeof(struct logsInfo), &bytesWritten, NULL)) {
        free(time);
        return -1;
    }

    free(time);
    CloseHandle(hFile);
    return 0;
}

struct logsInfo* GetDataFromLogFile(int* recordCount) {
    struct logsInfo* data = (struct logsInfo*)malloc(sizeof(struct logsInfo) * MAX_RECORDS);
    HANDLE hFile = CreateFile(logFilePath, GENERIC_READ,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    DWORD bytesRead;
    BOOL result = ReadFile(hFile, data,
        sizeof(struct logsInfo) * MAX_RECORDS, &bytesRead, NULL);

    if (!result || bytesRead % sizeof(struct logsInfo) != 0) {
        CloseHandle(hFile);
        free(data);
        return NULL;
    }

    *recordCount = bytesRead / sizeof(struct logsInfo);
    CloseHandle(hFile);
    return data;
}

DWORD GetNowTime() {
    SYSTEMTIME lst = { 2024, 1, 0, 1, 0, 0, 0, 0 };
    SYSTEMTIME st;
    GetLocalTime(&st);

    FILETIME fNow, fLast;
    SystemTimeToFileTime(&st, &fNow);
    SystemTimeToFileTime(&lst, &fLast);

    ULARGE_INTEGER uNow, uLast;

    uNow.LowPart = fNow.dwLowDateTime;
    uNow.HighPart = fNow.dwHighDateTime;
    uLast.LowPart = fLast.dwLowDateTime;
    uLast.HighPart = fLast.dwHighDateTime;

    ULONGLONG dif = uNow.QuadPart - uLast.QuadPart;

    DWORD nowTimeInMinutes = dif / 600000000;

    return nowTimeInMinutes;
}

struct dataAboutFile* GetFilesToCopy(int* count) {
    int recCount = 0;
    struct dataAboutFile* data = GetDataFromBinFile(&recCount);
    DWORD nowTime = GetNowTime();

    int indexes[MAX_RECORDS];
    int pointer = 0;

    for (int i = 0; i < recCount; i++) {
        if (data[i].lastCopy + data[i].frequency <= nowTime) {
            indexes[pointer] = i;
            pointer++;
            data[i].lastCopy = nowTime;
        }
    }

    struct dataAboutFile* filesToCopy = (struct dataAboutFile*)malloc(sizeof(struct dataAboutFile) * pointer);
    for (int i = 0; i < pointer; i++) {
        filesToCopy[i] = data[indexes[i]];
    }

    RecreateBinFile(data, recCount);
    free(data);

    *count = pointer;
    return filesToCopy;
}

void CopyFiles(const struct dataAboutFile* data, int count) {
    int deletedFilesCount = 0;
    for (int i = 0; i < count; i++) {
        DWORD attributes = GetFileAttributes(data[i].fSource);
        if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
            TCHAR* destPath= CopyDirectory(&data[i].dDest, &data[i].fSource, TRUE);
            if (destPath != NULL)
                AddRecordToLogFile(&data[i].fSource, destPath);
            else {
                DeleteRecordFromBinFile(i - deletedFilesCount);
                deletedFilesCount++;
            }
            free(destPath);
        }
        else {
            TCHAR* destPath = CopyingFile(&data[i].dDest, &data[i].fSource, TRUE);
            if (destPath != NULL) 
                AddRecordToLogFile(&data[i].fSource, destPath);
            else {
                DeleteRecordFromBinFile(i - deletedFilesCount);
                deletedFilesCount++;
            }
            free(destPath);;
        }
    }
}

TCHAR* CopyingFile(const TCHAR* dest, const TCHAR* source, BOOL isNeedToSetTime) {
    DWORD attributes = GetFileAttributes(source);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return NULL;
    }

    attributes = GetFileAttributes(dest);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return NULL;
    }

    TCHAR fDestFile[MAX_PATH];
    TCHAR* path = CreatePathToFile(dest, source);
    if (!isNeedToSetTime) {
        free(path);
        path = NewFilePath(dest, source);
    }
    _stprintf_s(fDestFile, MAX_PATH, _T("%s"), path);
    CopyFile(source, fDestFile, FALSE);
    return path;
}

TCHAR* CopyDirectory(const LPTSTR dest, const LPTSTR dirPath, BOOL isNeedToSetTime) {
    DWORD attributes = GetFileAttributes(dirPath);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return NULL;
    }

    attributes = GetFileAttributes(dest);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return NULL;
    }

    TCHAR* path = CreatePathToFolder(dest, dirPath);
    if (!isNeedToSetTime) {
        free(path);
        path = NewFolderPath(dest, dirPath);
    }
    CreateDirectory(path, NULL);

    int cFiles = 0, cDirs = 0;
    GetCountFilesAndFolders(dirPath, dest, &cFiles, &cDirs);
    LPTSTR* fPaths = (LPTSTR*)malloc(sizeof(LPTSTR*) * cFiles);
    LPTSTR* dPaths = (LPTSTR*)malloc(sizeof(LPTSTR*) * cDirs);

    ListFilesAndFolders(dirPath, dest, fPaths, cFiles, dPaths, cDirs);
    
    for (int i = 0; i < cFiles; i++) {
        CopyingFile(path, fPaths[i], FALSE);
    }

    for (int i = 0; i < cDirs; i++) {
        CopyDirectory(path, dPaths[i], FALSE);
    }
    return path;
}

void GetCountFilesAndFolders(const LPTSTR dirPath, const LPTSTR dest, int* countFiles, int* countFolders) {
    LPTSTR tmp = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
    _stprintf_s(tmp, MAX_PATH, _T("%s\\*"), dirPath);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(tmp, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) return;

    int cFiles = 0, cFolders = 0;
    do {
        LPTSTR path = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
        _stprintf_s(path, MAX_PATH, _T("%s\\%s"), dirPath, findFileData.cFileName);
        if (strcmp(findFileData.cFileName, _T(".")) != 0 && strcmp(findFileData.cFileName, _T("..")) != 0 && _tcscmp(dest, path) != 0) {
            DWORD attributes = GetFileAttributes(path);
            if (attributes & FILE_ATTRIBUTE_DIRECTORY)
                cFolders++;
            else
                cFiles++;
        }
        free(path);
    } while (FindNextFile(hFind, &findFileData) != 0);

    *countFiles = cFiles;
    *countFolders = cFolders;
    free(tmp);
}

void ListFilesAndFolders(const LPTSTR dirPath, const LPTSTR dest, LPTSTR* fPaths, const int cFiles, LPTSTR* dPaths, const int cDirs) {
    LPTSTR tmp = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
    _stprintf_s(tmp, MAX_PATH, _T("%s\\*"), dirPath);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(tmp, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) return;

    hFind = FindFirstFile(tmp, &findFileData);

    for (int i = 0; i < cFiles; i++) {
        fPaths[i] = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
    }
    for (int i = 0; i < cDirs; i++) {
        dPaths[i] = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
    }

    int iFiles = 0;
    int iDirs = 0;
    do {
        LPTSTR path = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
        _stprintf_s(path, MAX_PATH, _T("%s\\%s"), dirPath, findFileData.cFileName);
        if (strcmp(findFileData.cFileName, _T(".")) != 0 && strcmp(findFileData.cFileName, _T("..")) != 0 && _tcscmp(dest, path) != 0) {
            DWORD attributes = GetFileAttributes(path);
            if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
                _stprintf_s(dPaths[iDirs], MAX_PATH, _T("%s"), path);
                iDirs++;
            }
            else {
                _stprintf_s(fPaths[iFiles], MAX_PATH, _T("%s"), path);
                iFiles++;
            }
        }
        free(path);
    } while (FindNextFile(hFind, &findFileData) != 0);

    free(tmp);
}

void GetFileName(const TCHAR* filePath, TCHAR* fName) {
    TCHAR* lastSlash = _tcsrchr(filePath, '\\');

    if (lastSlash) {
        _tcscpy_s(fName, MAX_PATH, lastSlash + 1);
    }
    else {
        _tcscpy_s(fName, MAX_PATH, filePath);
    }
}

void DivideFileName(const TCHAR* fileName, TCHAR* fName, TCHAR* fType) {
    const TCHAR* dot = _tcsrchr(fileName, '.'); 

    if (dot) {
        size_t length = dot - fileName; 
        _tcsncpy_s(fName, MAX_PATH, fileName, length);
        fName[length] = '\0'; 

        _tcscpy_s(fType, MAX_PATH, dot + 1);
    }
    else {
        _stprintf_s(fName, MAX_PATH, _T("%s"), fileName);
        _stprintf_s(fType, MAX_PATH, _T("%s"), _T(""));
    }
}

TCHAR* ConvertIntToTCHAR(int n) {
    TCHAR* buffer = (TCHAR*)malloc(20 * sizeof(TCHAR));
    if (buffer != NULL) {
        _stprintf_s(buffer, 20, _T("%d"), n);
    }
    return buffer;
}

TCHAR* CreatePathToFile(const TCHAR* dest, const TCHAR* pathToFile) {
    TCHAR* fDestFile = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
    TCHAR fileName[MAX_PATH];
    GetFileName(pathToFile, fileName);

    TCHAR fName[MAX_PATH], fType[MAX_PATH];
    DivideFileName(fileName, fName, fType);

    SYSTEMTIME st;
    GetLocalTime(&st);

    TCHAR* year = ConvertIntToTCHAR(st.wYear);
    TCHAR* month = ConvertIntToTCHAR(st.wMonth);
    TCHAR* day = ConvertIntToTCHAR(st.wDay);
    TCHAR* hour = ConvertIntToTCHAR(st.wHour);
    TCHAR* minute = ConvertIntToTCHAR(st.wMinute);

    _stprintf_s(fDestFile, MAX_PATH, _T("%s\\%s%s%s%s%s%s%s%s%s%s%s%s"), dest, fName, year, "-", month, "-", day, "_", hour, "-", minute, ".", fType);
    free(year); free(month); free(day); free(hour); free(minute);
    return fDestFile;
}

TCHAR* CreatePathToFolder(const TCHAR* dest, const TCHAR* pathToFolder) {
    TCHAR* fDestFile = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
    TCHAR folderName[MAX_PATH];
    GetFileName(pathToFolder, folderName);

    SYSTEMTIME st;
    GetLocalTime(&st);

    TCHAR* year = ConvertIntToTCHAR(st.wYear);
    TCHAR* month = ConvertIntToTCHAR(st.wMonth);
    TCHAR* day = ConvertIntToTCHAR(st.wDay);
    TCHAR* hour = ConvertIntToTCHAR(st.wHour);
    TCHAR* minute = ConvertIntToTCHAR(st.wMinute);

    _stprintf_s(fDestFile, MAX_PATH, _T("%s\\%s%s%s%s%s%s%s%s%s%s"), dest, folderName, year, "-", month, "-", day, "_", hour, "-", minute);
    free(year); free(month); free(day); free(hour); free(minute);
    return fDestFile;
}

TCHAR* NewFolderPath(const TCHAR* dest, const TCHAR* pathToFolder){
    TCHAR* fDestFile = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
    TCHAR folderName[MAX_PATH];
    GetFileName(pathToFolder, folderName);

    _stprintf_s(fDestFile, MAX_PATH, _T("%s\\%s"), dest, folderName);
    return fDestFile;
}

TCHAR* NewFilePath(const TCHAR* dest, const TCHAR* pathToFile) {
    TCHAR* fDestFile = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
    TCHAR fileName[MAX_PATH];
    GetFileName(pathToFile, fileName);

    TCHAR fName[MAX_PATH], fType[MAX_PATH];
    DivideFileName(fileName, fName, fType);

    _stprintf_s(fDestFile, MAX_PATH, _T("%s\\%s%s%s"), dest, fName, ".", fType);
    return fDestFile;
}

TCHAR* RemoveLastPathElement(LPTSTR path) {
    TCHAR* newPath = NULL;
    int len = lstrlen(path);

    LPTSTR lastSlash = NULL;
    for (int i = len - 1; i >= 0; i--) {
        if (path[i] == '\\') {
            lastSlash = &path[i];
            break;
        }
    }

    if (lastSlash == NULL) {
        newPath = (TCHAR*)malloc(3 * sizeof(TCHAR));
        lstrcpy(newPath, _T(":\\"));
    }
    else {
        newPath = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
        lstrcpyn(newPath, path, lastSlash - path + 1);
    }
    return newPath;
}