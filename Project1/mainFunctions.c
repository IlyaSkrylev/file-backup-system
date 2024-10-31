#include <Windows.h>
#include <tchar.h>
#include "mainFunctions.h"

#define BIN_FILE_PATH "datafile.bin"
#define MAX_RECORDS 100

int RecreateBinFile(const struct dataAboutFile* record, int numberOfRecord) {
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