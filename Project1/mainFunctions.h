#include <windows.h>

struct dataAboutFile {
    TCHAR fSource[MAX_PATH];
    TCHAR dDest[MAX_PATH];
    DWORD frequency;
    DWORD lastCopy;
};

void TakeBinFilePath();
void FreeMemoryBinFile();
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
void CopyingFile(const TCHAR* dest, const TCHAR* source, BOOL isNeedToSetTime);
void CopyDirectory(const LPTSTR dest, const LPTSTR dirPath, BOOL isNeedToSetTime);
void GetCountFilesAndFolders(const LPTSTR dirPath, const LPTSTR dest, int* countFiles, int* countFolders);
void ListFilesAndFolders(const LPTSTR dirPath, const LPTSTR dest, LPTSTR* fPaths, const int cFiles, LPTSTR* dPaths, const int cDirs);
TCHAR* CreatePathToFile(const TCHAR* dest, const TCHAR* file);
TCHAR* CreatePathToFolder(const TCHAR* dest, const TCHAR* folder);
TCHAR* NewFolderPath(const TCHAR* dest, const TCHAR* pathToFolder);
TCHAR* NewFilePath(const TCHAR* dest, const TCHAR* pathToFile);