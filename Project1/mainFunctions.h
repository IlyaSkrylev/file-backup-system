#include <windows.h>

struct dataAboutFile {
    TCHAR fSource[MAX_PATH];
    TCHAR dDest[MAX_PATH];
    DWORD frequency;
    DWORD lastCopy;
};

struct logsInfo {
    TCHAR fSource[MAX_PATH];
    TCHAR fDest[MAX_PATH];
    TCHAR copyTime[MAX_PATH];
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

void TakeLogFile();
void FreeMemoryLogFile();
void AddRecordToLogFile(const TCHAR* filePath, const TCHAR* destPath);
struct logsInfo* GetDataFromLogFile(int* recordCount);

DWORD GetNowTime();
struct dataAboutFile* GetFilesToCopy(int* count);
void CopyFiles(const struct dataAboutFile* data, int count);
void GetFileName(const TCHAR* filePath, TCHAR* fName);
void DivideFileName(const TCHAR* fileName, TCHAR* fName, TCHAR* fType);
TCHAR* ConvertIntToTCHAR(int n);
TCHAR* CopyingFile(const TCHAR* dest, const TCHAR* source, BOOL isNeedToSetTime);
TCHAR* CopyDirectory(const LPTSTR dest, const LPTSTR dirPath, BOOL isNeedToSetTime);
void GetCountFilesAndFolders(const LPTSTR dirPath, const LPTSTR dest, int* countFiles, int* countFolders);
void ListFilesAndFolders(const LPTSTR dirPath, const LPTSTR dest, LPTSTR* fPaths, const int cFiles, LPTSTR* dPaths, const int cDirs);
TCHAR* CreatePathToFile(const TCHAR* dest, const TCHAR* file);
TCHAR* CreatePathToFolder(const TCHAR* dest, const TCHAR* folder);
TCHAR* NewFolderPath(const TCHAR* dest, const TCHAR* pathToFolder);
TCHAR* NewFilePath(const TCHAR* dest, const TCHAR* pathToFile);
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
void EnableBackupRestorePrivileges();