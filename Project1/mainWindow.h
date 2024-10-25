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

void OnClickButtonAdd();
void HideComponentsToAdd();
void ResetComponentsCaptionsToAdd();
void OnClickButtonLang(HWND hwnd);
void OnClickSelectFile(HWND hwnd);
void OnClickSelectDir(HWND hwnd);
void OnClickButtonCreate(HWND hwnd);

int GetIndexOfComponent(int id);
char* OpenFileDialog(HWND hwnd);
char* SelectFolder(HWND hwnd);
LPTSTR ConcatenateStrings(LPTSTR lpszStr1, LPTSTR lpszStr2);
int GetEnteredNumber(LPTSTR number);
int WriteDataIntoFile(const struct dataAboutFile* data);

void ErrorFieldsNotFilled(HWND hwnd);
int ErrorIncorrectFilePath(HWND hwnd, LPTSTR fSource);
int ErrorIncorrectDirPath(HWND hwnd, LPTSTR dDest);
int ErrorIncorrectFrequency(HWND hwnd, int freq);
void ErrorWrong(HWND hwnd);
void InformationSuccessCreating(HWND hwnd, LPTSTR fSource, LPTSTR dDest, int minFreq);