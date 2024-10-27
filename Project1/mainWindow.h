#include <windows.h>

int RunWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitializeCompnents(HWND hwnd);
void DrawDependsComponents();
void SetMinMaxSize(LPARAM lParam);
void CommandsOfComponents(HWND hwnd, WPARAM wParam);
LRESULT ChangeStaticBk(WPARAM wParam);

HWND CreateButton(HWND hwnd, struct component btn);
HWND CreateEdit(HWND hwnd, struct component edit);
HWND CreateNumericEdit(HWND hwnd, struct component nedit);
HWND CreateLabel(HWND hwnd, struct component lbl);
HWND CreateUpDown(HWND hwnd, struct component tmr);

LPTSTR GetStringFromResource(UINT id);
void SetApplicationLanguage(BOOL isEnglish);

void OnClickButtonMainMenu(int idBtnMainMenu, int idHideButton);
void HideComponentsMainMenu(int idBtnMainMenu, int idHideButton);
void ResetComponentsCaptionsToAdd();
void OnClickButtonLang(HWND hwnd);
void OnClickSelectFile(HWND hwnd);
void OnClickSelectDir(HWND hwnd);
void OnClickButtonCreate(HWND hwnd);

int GetIndexOfComponent(int id);
char* OpenFileDialog(HWND hwnd);
char* SelectFolder(HWND hwnd);
LPTSTR ConcatenateStrings(LPTSTR strings[], int len);
int GetEnteredNumber(LPTSTR number);
int WriteDataIntoBinFile(const struct dataAboutFile* data);
int RecordCountInBinFile();

void ErrorTextOnly(HWND hwnd, DWORD textInd);
int ErrorIncorrectFilePath(HWND hwnd, LPTSTR fSource);
int ErrorIncorrectDirPath(HWND hwnd, LPTSTR dDest);
int ErrorIncorrectFrequency(HWND hwnd, int freq);
void InformationSuccessCreating(HWND hwnd, LPTSTR fSource, LPTSTR dDest, int minFreq);
