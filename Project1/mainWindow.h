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
void OnClickButtonLang(HWND hwnd);
void OnClickSelectFile(HWND hwnd);
void OnClickSelectDir(HWND hwnd);
void OnClickButtonCreate(HWND hwnd);

int GetIndexOfComponent(int id);
char* OpenFileDialog(HWND hwnd);
char* SelectFolder(HWND hwnd);
LPTSTR ConcatenateStrings(LPTSTR lpszStr1, LPTSTR lpszStr2);