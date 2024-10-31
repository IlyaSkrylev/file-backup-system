#include <windows.h>

struct dataAboutFile {
    TCHAR fSource[MAX_PATH];
    TCHAR dDest[MAX_PATH];
    DWORD frequency;
    DWORD lastCopy;
};

int RecreateBinFile(const struct dataAboutFile* record, int numberOfRecord);
int WriteDataIntoBinFile(const struct dataAboutFile* data);
struct dataAboutFile* GetDataFromBinFile(int* recordCount);
int RecordCountInBinFile();
int DeleteRecordFromBinFile(int index);