#include <windows.h>

struct dataAboutFile {
    TCHAR fSource[MAX_PATH];
    TCHAR dDest[MAX_PATH];
    DWORD frequency;
    DWORD lastCopy;
};

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

int RecreateBinFile(struct dataAboutFile* data, int count);
int ChangeRecordInBinFile(const struct dataAboutFile* record, int numberOfRecord);
int WriteDataIntoBinFile(const struct dataAboutFile* data);
struct dataAboutFile* GetDataFromBinFile(int* recordCount);
int RecordCountInBinFile();
int DeleteRecordFromBinFile(int index);

DWORD GetNowTime();
struct dataAboutFile* GetFilesToCopy(int* count);
void CopyFiles(const struct dataAboutFile* data, int count);
void GetFileName(const TCHAR* filePath, TCHAR* fName);
void DivideFileName(const TCHAR* fileName, TCHAR* fName, TCHAR* fType);
TCHAR* ConvertIntToTCHAR(int n);