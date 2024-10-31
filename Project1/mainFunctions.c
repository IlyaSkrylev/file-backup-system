#include <Windows.h>
#include <tchar.h>
#include "mainFunctions.h"

#define BIN_FILE_PATH "datafile.bin"
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

    HANDLE hFile = CreateFile(_T(BIN_FILE_PATH), GENERIC_WRITE,
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
    HANDLE hFile = CreateFile(_T(BIN_FILE_PATH), GENERIC_WRITE,
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

    HANDLE hFile = CreateFile(_T(BIN_FILE_PATH), GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }

    DWORD bytesWritten;
    for (int i = 0; i < recordCount; i++) {
        if (i != index) {
            SetFilePointer(hFile, 0, NULL, FILE_END);
            if (!WriteFile(hFile, data, sizeof(struct dataAboutFile), &bytesWritten, NULL)) {
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

    HANDLE hFile = CreateFile(_T(BIN_FILE_PATH), GENERIC_WRITE,
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
    HANDLE hFile = CreateFile(_T(BIN_FILE_PATH), GENERIC_READ,
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
    HANDLE hFile = CreateFile(_T(BIN_FILE_PATH), GENERIC_READ,
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
        TCHAR fDestFile[MAX_PATH];
        TCHAR fileName[MAX_PATH];
        GetFileName(data[i].fSource, fileName);
        TCHAR fName[MAX_PATH], fType[MAX_PATH];
        DivideFileName(fileName, fName, fType);

        SYSTEMTIME st;
        GetLocalTime(&st);

        TCHAR* year = ConvertIntToTCHAR(st.wYear);
        TCHAR* month = ConvertIntToTCHAR(st.wMonth);
        TCHAR* day = ConvertIntToTCHAR(st.wDay);
        TCHAR* hour = ConvertIntToTCHAR(st.wHour);
        TCHAR* minute = ConvertIntToTCHAR(st.wMinute);

        _stprintf_s(fDestFile, MAX_PATH, _T("%s\\%s%s%s%s%s%s%s%s%s%s%s%s"), data[i].dDest, fName, year, "-", month, "-", day, "_", hour, "-", minute, ".", fType);

        CopyFile(data[i].fSource, fDestFile, FALSE);

        free(year); free(month); free(day); free(hour); free(minute);
    }
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