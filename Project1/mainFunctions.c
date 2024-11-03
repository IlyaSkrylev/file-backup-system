#include <Windows.h>
#include <tchar.h>
#include "mainFunctions.h"

#define BIN_FILE_PATH "datafile.bin"
#define SAVE_FILE_PATH "BackupFiles";
#define MAX_RECORDS 100

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
    for (int i = 0; i < count; i++) {
        DWORD attributes = GetFileAttributes(data[i].fSource);
        if (attributes & FILE_ATTRIBUTE_DIRECTORY)
            CopyDirectory(&data[i].dDest, &data[i].fSource, TRUE);
        else {
            CopyingFile(&data[i].dDest, &data[i].fSource, TRUE);
        }
    }
}

void CopyingFile(const TCHAR* dest, const TCHAR* source, BOOL isNeedToSetTime) {
    TCHAR fDestFile[MAX_PATH];
    TCHAR* path = CreatePathToFile(dest, source);
    if (!isNeedToSetTime) {
        free(path);
        path = NewFilePath(dest, source);
    }
    _stprintf_s(fDestFile, MAX_PATH, _T("%s"), path);
    CopyFile(source, fDestFile, FALSE);
    free(path);
}

void CopyDirectory(const LPTSTR dest, const LPTSTR dirPath, BOOL isNeedToSetTime) {
    TCHAR* path = CreatePathToFolder(dest, dirPath);
    if (!isNeedToSetTime) {
        free(path);
        path = NewFolderPath(dest, dirPath);
    }
    CreateDirectory(path, NULL);

    int cFiles = 0, cDirs = 0;
    GetCountFilesAndFolders(dirPath, &cFiles, &cDirs);
    LPTSTR* fPaths = (LPTSTR*)malloc(sizeof(LPTSTR*) * cFiles);
    LPTSTR* dPaths = (LPTSTR*)malloc(sizeof(LPTSTR*) * cDirs);

    ListFilesAndFolders(dirPath, fPaths, cFiles, dPaths, cDirs);
    
    for (int i = 0; i < cFiles; i++) {
        CopyingFile(path, fPaths[i], FALSE);
    }

    for (int i = 0; i < cDirs; i++) {
        CopyDirectory(path, dPaths[i], FALSE);
    }
    free(path);
}

void GetCountFilesAndFolders(const LPTSTR dirPath, int* countFiles, int* countFolders) {
    LPTSTR tmp = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
    _stprintf_s(tmp, MAX_PATH, _T("%s\\*"), dirPath);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(tmp, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) return;

    int cFiles = 0, cFolders = 0;
    do {
        if (strcmp(findFileData.cFileName, _T(".")) != 0 && strcmp(findFileData.cFileName, _T("..")) != 0) {
            LPTSTR path = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
            _stprintf_s(path, MAX_PATH, _T("%s\\%s"), dirPath, findFileData.cFileName);
            DWORD attributes = GetFileAttributes(path);
            if (attributes & FILE_ATTRIBUTE_DIRECTORY)
                cFolders++;
            else
                cFiles++;
            free(path);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    *countFiles = cFiles;
    *countFolders = cFolders;
    free(tmp);
}

void ListFilesAndFolders(const LPTSTR dirPath, LPTSTR* fPaths, const int cFiles, LPTSTR* dPaths, const int cDirs) {
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
        if (strcmp(findFileData.cFileName, _T(".")) != 0 && strcmp(findFileData.cFileName, _T("..")) != 0) {
            LPTSTR path = (LPTSTR)malloc(MAX_PATH * sizeof(TCHAR));
            _stprintf_s(path, MAX_PATH, _T("%s\\%s"), dirPath, findFileData.cFileName);
            DWORD attributes = GetFileAttributes(path);
            if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
                dPaths[iDirs] = path;
                iDirs++;
            }
            else {
                fPaths[iFiles] = path;
                iFiles++;
            }
        }
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

/*BOOL CTRLHandler(DWORD CtrlType, LPTSTR unsavedFiles) {
    if (CtrlType == CTRL_CLOSE_EVENT || CtrlType == CTRL_LOGOFF_EVENT || CtrlType == CTRL_SHUTDOWN_EVENT) {
        TCHAR saveFilePath = _T(SAVE_FILE_PATH);
        CreateDirectory(&saveFilePath, NULL);
                          
    }

    return TRUE;
}*/