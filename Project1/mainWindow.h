#include <windows.h>

struct component {
    HWND cmp;
    char* type;
    UINT name;
    int id;
    int x, y;
    int width, height;
    int initShow;
    BOOL isAlwaysVisible;
};

int RunWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitializeCompnents(HWND hwnd);
void DrawDependsComponents();
void SetMinMaxSize(LPARAM lParam);
void CommandsOfComponents(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT ChangeStaticBk(WPARAM wParam);

HWND CreateButton(HWND hwnd, struct component btn);
HWND CreateEdit(HWND hwnd, struct component edit);
HWND CreateNumericEdit(HWND hwnd, struct component nedit);
HWND CreateLabel(HWND hwnd, struct component lbl);
HWND CreateUpDown(HWND hwnd, struct component tmr);
HWND CreateListBox(HWND hwnd, struct component tb);

LPTSTR GetStringFromResource(UINT id);
void SetApplicationLanguage(BOOL isEnglish);

void ShowAuxiliaryComponents(int idBtnMainMenu, int idHideButton);
void HideComponentsMainMenu(int idBtnMainMenu, int idHideButton);
void ResetComponentsCaptionsToAdd();
void ResetComponentsCaptionsToAllFiles();
int ChangeComponentsCoords(HWND hwnd);
int ShowFilesFromBinFile(hwnd);
void OnClickButtonLang(HWND hwnd);
void OnClickSelectFile(HWND hwnd);
void OnClickSelectDir(HWND hwnd, int idEditDest);
void OnClickButtonCreate(HWND hwnd);
void OnClickButtonChange(HWND hwnd, LPARAM lParam);
void OnClickDeleteAllFiles(HWND hwnd, LPARAM lParam);
void ShowInfoAboutFile(LPARAM lParam);
void ChangeYOfComponent(int idCmp, int idDepCmp);
void ChangePositionUpDown(HWND hwnd, int idUD, int idEdit);

int GetIndexOfComponent(int id);
char* OpenFileDialog(HWND hwnd);
char* SelectFolder(HWND hwnd);
LPTSTR ConcatenateStrings(LPTSTR strings[], int len);
int GetEnteredNumber(LPTSTR number);
LPTSTR ConvertIntToLPTSTR(int n);
void ChangePath(LPTSTR path);

void ErrorTextOnly(HWND hwnd, DWORD textInd);
int ErrorIncorrectFilePath(HWND hwnd, LPTSTR fSource);
int ErrorIncorrectDirPath(HWND hwnd, LPTSTR dDest, LPTSTR fSource);
int ErrorIncorrectFrequency(HWND hwnd, int freq);
void InformationSuccessCreating(HWND hwnd, LPTSTR fSource, LPTSTR dDest, int minFreq);
void InformationSuccessDeleting(HWND hwnd, LPTSTR fSource);
