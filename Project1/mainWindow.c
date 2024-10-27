#include <windows.h>
#include <shlobj.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include "mainWindow.h"
#include "resource1.h"

#define MIN_WINDOW_SIZE_X 830
#define MIN_WINDOW_SIZE_Y 600
#define MAX_WINDOW_SIZE_X 830
#define MAX_WINDOW_SIZE_Y 600

#define MIN_NUMUPDOWN_VALUE 0
#define MAX_NUMUPDOWN_VALUE 100

#define MAX_RECORDS 100

#define ID_BTN_ADD 101
#define ID_BTN_HIDE_ADD 102
#define ID_BTN_LANG 103
#define ID_BTN_SELECT_FILE 104
#define ID_BTN_SELECT_DIR 105
#define ID_BTN_CREATE 106
#define ID_BTN_ALL_FILES 107
#define ID_BTN_HIDE_ALL_FILES 108

#define ID_EDIT_SOURCE 201
#define ID_EDIT_DEST 202
#define ID_EDIT_HOUR 203
#define ID_EDIT_MINUTE 204

#define ID_LBL_SOURCE 301
#define ID_LBL_DEST 302
#define ID_LBL_FREQUENCY 303
#define ID_LBL_HOUR 304
#define ID_LBL_MINUTE 305

#define ID_UPDOWN_HOUR 401
#define ID_UPDOWN_MINUTE 402

#define BUTTON_WIDTH 100 
#define BUTTON_HEIGHT 25

#define EDIT_WIDTH 400
#define EDIT_HEIGHT 20

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

struct dataAboutFile {
    LPTSTR fSource[MAX_PATH];
    LPTSTR dDest[MAX_PATH];
    DWORD frequency;
};

struct component cmpInfo[] = {
    {NULL, "button", BUTTON_ADD, ID_BTN_ADD, 10, 10, BUTTON_WIDTH, BUTTON_HEIGHT, SW_SHOW, TRUE},
    {NULL, "label", LABEL_SOURCE, ID_LBL_SOURCE, 120, 10, 150, 20, SW_HIDE, FALSE},
    {NULL, "label", LABEL_DEST, ID_LBL_DEST, 120, 40, 150, 20, SW_HIDE, FALSE},
    {NULL, "label", LABEL_FREQUENCY, ID_LBL_FREQUENCY, 120, 70, 150, 20, SW_HIDE, FALSE},
    {NULL, "label", LABEL_HOUR, ID_LBL_HOUR, 280, 70, 10, 20, SW_HIDE, FALSE},
    {NULL, "label", LABEL_MINUTE, ID_LBL_MINUTE, 360, 70, 25, 20, SW_HIDE, FALSE},
    {NULL, "edit", 0, ID_EDIT_SOURCE, 280, 10, EDIT_WIDTH, EDIT_HEIGHT, SW_HIDE, FALSE},
    {NULL, "edit", 0, ID_EDIT_DEST, 280, 40, EDIT_WIDTH, EDIT_HEIGHT, SW_HIDE, FALSE},
    {NULL, "nedit", 0, ID_EDIT_HOUR, 290, 70, 50, 20, SW_HIDE, FALSE},
    {NULL, "updown", 0, ID_UPDOWN_HOUR, 0, 0, 0, 0, SW_HIDE, FALSE},
    {NULL, "nedit", 0, ID_EDIT_MINUTE, 390, 70, 50, 20, SW_HIDE, FALSE},
    {NULL, "updown", 0, ID_UPDOWN_MINUTE, 0, 0, 0, 0, SW_HIDE, FALSE},
    {NULL, "button", BUTTON_SELECT_FILE, ID_BTN_SELECT_FILE, 690, 8, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE, FALSE},
    {NULL, "button", BUTTON_SELECT_DIR, ID_BTN_SELECT_DIR, 690, 38, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE, FALSE},
    {NULL, "button", BUTTON_CREATE, ID_BTN_CREATE, 690, 70, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE, FALSE},
    {NULL, "button", BUTTON_HIDE_ADD, ID_BTN_HIDE_ADD, 120, 100, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE, FALSE},

    {NULL, "button", BUTTON_ALL_FILES, ID_BTN_ALL_FILES, 10, 45, BUTTON_WIDTH, BUTTON_HEIGHT, SW_SHOW, TRUE},
    {NULL, "button", BUTTON_HIDE_ALL_FILES, ID_BTN_HIDE_ALL_FILES, 10, 100, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE, FALSE},

    {NULL, "button", BUTTON_LANG, ID_BTN_LANG, 0, 0, 40, BUTTON_HEIGHT, SW_SHOW, TRUE},

    {NULL, NULL, NULL, NULL, NULL, NULL}
};

RECT windowRect;

int RunWindow(HINSTANCE hInstance, int nCmdShow) {
    SetApplicationLanguage(TRUE);
    WNDCLASSEX wcex; HWND hWnd; MSG msg;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "BackUP";
    wcex.hIconSm = wcex.hIcon;
    RegisterClassEx(&wcex);
    hWnd = CreateWindow("BackUP", L"BackUP",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    switch (uMsg) {
    case WM_CREATE: {
        InitializeCompnents(hwnd);
        break;
    }
    case WM_SIZE: {
        GetWindowRect(hwnd, &windowRect);
        DrawDependsComponents();
        break;
    }
    case WM_CTLCOLORSTATIC: {
        return ChangeStaticBk(wParam);
        break;
    }
    case WM_GETMINMAXINFO: {
        SetMinMaxSize(lParam);
        break;
    }
    case WM_COMMAND: {
        CommandsOfComponents(hwnd, wParam);
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Components creating
void InitializeCompnents(HWND hwnd) {
    for (int i = 0; cmpInfo[i].id != NULL; i++) {
        if (cmpInfo[i].type == "button")
            cmpInfo[i].cmp = CreateButton(hwnd, cmpInfo[i]);
        else if (cmpInfo[i].type == "edit")
            cmpInfo[i].cmp = CreateEdit(hwnd, cmpInfo[i]);
        else if (cmpInfo[i].type == "nedit")
            cmpInfo[i].cmp = CreateNumericEdit(hwnd, cmpInfo[i]);
        else if (cmpInfo[i].type == "label")
            cmpInfo[i].cmp = CreateLabel(hwnd, cmpInfo[i]);
        else if (cmpInfo[i].type == "updown")
            cmpInfo[i].cmp = CreateUpDown(hwnd, cmpInfo[i]);

        ShowWindow(cmpInfo[i].cmp, cmpInfo[i].initShow);
    }
}

void DrawDependsComponents() {
    int index = GetIndexOfComponent(ID_BTN_LANG);
    int newX = windowRect.right - windowRect.left - 40 - 28;
    int newY = windowRect.bottom - windowRect.top - BUTTON_HEIGHT - 50;
    SetWindowPos(cmpInfo[index].cmp, NULL, newX, newY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    cmpInfo[index].x = newX;
    cmpInfo[index].y = newY;
}

// Set min and max size of form
void SetMinMaxSize(LPARAM lParam) {
    MINMAXINFO* mmi = (MINMAXINFO*)lParam;
    mmi->ptMinTrackSize.x = MIN_WINDOW_SIZE_X;
    mmi->ptMinTrackSize.y = MIN_WINDOW_SIZE_Y;

    mmi->ptMaxTrackSize.x = MAX_WINDOW_SIZE_X;
    mmi->ptMaxTrackSize.y = MAX_WINDOW_SIZE_Y;
}

LRESULT ChangeStaticBk(WPARAM wParam) {
    COLORREF backgroundColor = GetSysColor(COLOR_WINDOW);
    HDC hdcStatic = (HDC)wParam;
    SetTextColor(hdcStatic, RGB(0, 0, 0));
    SetBkColor(hdcStatic, backgroundColor);
    return (LRESULT)CreateSolidBrush(backgroundColor);
}

LPTSTR GetStringFromResource(UINT id) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    int len = 256;
    LPTSTR buffer = (LPTSTR)malloc(len * sizeof(TCHAR));

    if (LoadString(hInstance, id, buffer, len)) {
        return buffer;
    }
    else {
        free(buffer);
        return NULL;
    }
}

// Select a language
void SetApplicationLanguage(BOOL isEnglish) {
    if (isEnglish) {
        SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT));
    }
    else {
        SetThreadUILanguage(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT));
    }
}

HWND CreateButton(HWND hwnd, struct component btn) {
    LPTSTR btnName = GetStringFromResource(btn.name);
    HWND button =  CreateWindow(TEXT("BUTTON"), btnName,
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            btn.x, btn.y, btn.width, btn.height,
            hwnd, (HMENU)btn.id, NULL, NULL);
    free(btnName);
    return button;
}

HWND CreateEdit(HWND hwnd, struct component edit) {
    return CreateWindow(TEXT("EDIT"), "",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        edit.x, edit.y, edit.width, edit.height,
        hwnd, (HMENU)edit.id, NULL, NULL);
}

HWND CreateNumericEdit(HWND hwnd, struct component nedit) {
    return CreateWindow(TEXT("EDIT"), "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
        nedit.x, nedit.y, nedit.width, nedit.height,
        hwnd, (HMENU)nedit.id, NULL, NULL);
}

HWND CreateLabel(HWND hwnd, struct component lbl) {
    LPTSTR lName = GetStringFromResource(lbl.name);
    HWND label = CreateWindow(TEXT("static"), lName,
        WS_CHILD | WS_VISIBLE,
        lbl.x, lbl.y, lbl.width, lbl.height,
        hwnd, (HMENU)lbl.id, NULL, NULL);
    free(lName);
    return label;
}

HWND CreateUpDown(HWND hwnd, struct component tmr) {
    HWND hwndSpin = CreateWindow(UPDOWN_CLASS, "",
        WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS,
        0, 0, 0, 0,
        hwnd, (HMENU)500, NULL, NULL);
    
    int indexOfEdit = 0;
    switch (tmr.id) {
    case ID_UPDOWN_HOUR: {
        indexOfEdit = ID_EDIT_HOUR;
        break;
    }
    case ID_UPDOWN_MINUTE:
        indexOfEdit = ID_EDIT_MINUTE;
        break;
    }

    SendMessage(hwndSpin, UDM_SETBUDDY, (WPARAM)cmpInfo[GetIndexOfComponent(indexOfEdit)].cmp, 0);
    SendMessage(hwndSpin, UDM_SETRANGE32, MIN_NUMUPDOWN_VALUE, MAX_NUMUPDOWN_VALUE);
    SendMessage(hwndSpin, UDM_SETPOS32, 0, 0);
    return hwndSpin;
}

// Command processing 
void CommandsOfComponents(HWND hwnd, WPARAM wParam) {
    switch (LOWORD(wParam)) {
    case ID_BTN_ADD: {
        OnClickButtonMainMenu(ID_BTN_ADD, ID_BTN_HIDE_ADD);
        break;
    }
    case ID_BTN_HIDE_ADD: {
        HideComponentsMainMenu(ID_BTN_ADD, ID_BTN_HIDE_ADD);
        break;
    }
    case ID_BTN_LANG: {
        OnClickButtonLang(hwnd);
        break;
    }
    case ID_BTN_SELECT_FILE: {
        OnClickSelectFile(hwnd);
        break;
    }
    case ID_BTN_SELECT_DIR: {
        OnClickSelectDir(hwnd);
        break;
    }
    case ID_BTN_CREATE: {
        OnClickButtonCreate(hwnd);
        break;
    }
    case ID_BTN_ALL_FILES: {
        OnClickButtonMainMenu(ID_BTN_ALL_FILES, ID_BTN_HIDE_ALL_FILES);
        //ShowFilesFromBinFile(hwnd);
        break;
    }
    case ID_BTN_HIDE_ALL_FILES: {
        HideComponentsMainMenu(ID_BTN_ALL_FILES, ID_BTN_HIDE_ALL_FILES);
        break;
    }
    }
}

void OnClickButtonMainMenu(int idBtnMainMenu, int idHideButton) {
    int indexBtnMainMenu = GetIndexOfComponent(idBtnMainMenu);
    int indexHideButton = GetIndexOfComponent(idHideButton);

    for (int i = 0; cmpInfo[i].id != NULL; i++) {
        if (!cmpInfo[i].isAlwaysVisible && (i < indexBtnMainMenu || i > indexHideButton)) {
            ShowWindow(cmpInfo[i].cmp, SW_HIDE);
            cmpInfo[i].initShow = SW_HIDE;
        }
    }

    for (int i = indexBtnMainMenu + 1; i <= indexHideButton; i++) {
        ShowWindow(cmpInfo[i].cmp, SW_SHOW);
        cmpInfo[i].initShow = SW_SHOW;
    }
}

void HideComponentsMainMenu(int idBtnMainMenu, int idHideButton) {
    int indexHideButton = GetIndexOfComponent(idHideButton);
    for (int i = GetIndexOfComponent(idBtnMainMenu) + 1; i <= indexHideButton; i++) {
        ShowWindow(cmpInfo[i].cmp, SW_HIDE);
        cmpInfo[i].initShow = SW_HIDE;
    }
}


void ResetComponentsCaptionsToAdd(){
    SetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_SOURCE)].cmp, "");
    SetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_DEST)].cmp, "");
    SendMessage(cmpInfo[GetIndexOfComponent(ID_UPDOWN_HOUR)].cmp, UDM_SETPOS32, 0, 0);
    SendMessage(cmpInfo[GetIndexOfComponent(ID_UPDOWN_MINUTE)].cmp, UDM_SETPOS32, 0, 0);
}

void OnClickButtonLang(HWND hwnd) {
    char buttonText[100];
    GetWindowText(cmpInfo[GetIndexOfComponent(ID_BTN_LANG)].cmp, buttonText, sizeof(buttonText));
    if (buttonText[0] == 'e')
        SetApplicationLanguage(TRUE);
    else
        SetApplicationLanguage(FALSE);
    
    for (int i = 0; cmpInfo[i].id != NULL; i++) {
        if (cmpInfo[i].name != 0) {
            LPTSTR text = GetStringFromResource(cmpInfo[i].name);
            SetWindowText(cmpInfo[i].cmp, text);
            free(text);
        }
    }
}

void OnClickSelectFile(HWND hwnd) {
    char* filePath = OpenFileDialog(hwnd);
    if (filePath != NULL && filePath[0] != '\0')
        SetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_SOURCE)].cmp, filePath);
    free(filePath);
}

void OnClickSelectDir(HWND hwnd) {
    char* filePath = SelectFolder(hwnd);
    if (filePath != NULL && filePath[0] != '\0')
        SetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_DEST)].cmp, filePath);
    free(filePath);
}

void OnClickButtonCreate(HWND hwnd) {
    int lenS = GetWindowTextLength(cmpInfo[GetIndexOfComponent(ID_EDIT_SOURCE)].cmp);
    int lenD = GetWindowTextLength(cmpInfo[GetIndexOfComponent(ID_EDIT_DEST)].cmp);
    int lenH = GetWindowTextLength(cmpInfo[GetIndexOfComponent(ID_EDIT_HOUR)].cmp);
    int lenM = GetWindowTextLength(cmpInfo[GetIndexOfComponent(ID_EDIT_MINUTE)].cmp);

    if (lenS <= 0 || lenD <= 0 || lenH <= 0 || lenM <= 0) {
        ErrorTextOnly(hwnd, TEXT_NOT_FILLED);
        return;
    }
    else {
        LPTSTR fSource = (LPTSTR)malloc((lenS + 1) * sizeof(TCHAR));
        GetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_SOURCE)].cmp, fSource, lenS + 1);
        if (ErrorIncorrectFilePath(hwnd, fSource)) {
            free(fSource);
            return;
        }

        LPTSTR dDest = (LPTSTR)malloc((lenD + 1) * sizeof(TCHAR));
        GetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_DEST)].cmp, dDest, lenS + 1);
        if (ErrorIncorrectDirPath(hwnd, dDest)) {
            free(fSource); free(dDest);
            return;
        }

        LPTSTR hour = (LPTSTR)malloc((lenH + 1) * sizeof(TCHAR));
        GetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_HOUR)].cmp, hour, lenH + 1);
        int h = GetEnteredNumber(hour);

        LPTSTR minutes = (LPTSTR)malloc((lenM + 1) * sizeof(TCHAR));
        GetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_MINUTE)].cmp, minutes, lenM + 1);
        int m = GetEnteredNumber(minutes);

        int minFreq = h * 60 + m;
        if (ErrorIncorrectFrequency(hwnd, minFreq)) {
            free(fSource); free(dDest); free(hour); free(minutes);
            return;
        }

        struct dataAboutFile data = { fSource, dDest, minFreq };
        int res = WriteDataIntoBinFile(&data);
        if (res == -1) {
            free(fSource); free(dDest); free(hour); free(minutes);
            ErrorTextOnly(hwnd, TEXT_WRONG);
            return;
        }
        else if (res == 1) {
            free(fSource); free(dDest); free(hour); free(minutes);
            ErrorTextOnly(hwnd, TEXT_LIMIT_FILES);
            return;
        }

        InformationSuccessCreating(hwnd, fSource, dDest, minFreq);
        ResetComponentsCaptionsToAdd();
        HideComponentsMainMenu(ID_BTN_ADD, ID_BTN_HIDE_ADD);
        free(fSource); free(dDest); free(hour); free(minutes);
    }
}

int GetIndexOfComponent(int id) {
    for (int i = 0; cmpInfo[i].id != NULL; i++) {
        if (cmpInfo[i].id == id)
            return i;
    }
    return 0;
}

char* OpenFileDialog(HWND hwnd) {
    OPENFILENAME ofn;
    char* fileName = malloc(MAX_PATH);
    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(fileName, MAX_PATH);

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0Text Files (*.txt)\0*.txt\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    char currentDir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDir);
    if (GetOpenFileName(&ofn))
    {
        SetCurrentDirectoryA(currentDir);
        return fileName;
    }

    SetCurrentDirectoryA(currentDir);
    free(fileName);
    return NULL;
}

char* SelectFolder(HWND hwnd) {
    BROWSEINFO bi = { 0 };
    bi.hwndOwner = hwnd;
    LPTSTR title = GetStringFromResource(TEXT_BI_TITLE);
    bi.lpszTitle = title;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != NULL) {
        char* path = malloc(MAX_PATH);
        SHGetPathFromIDList(pidl, path);
        CoTaskMemFree(pidl);
        free(title);
        return path;
    }
    free(title);
    return NULL;
}

LPTSTR ConcatenateStrings(LPTSTR strings[], int len) {
    int totalLen = 0;
    for (int i = 0; i < len; i++) {
        totalLen += lstrlen(strings[i]);
    }

    LPTSTR lpszResult = (LPTSTR)malloc((totalLen + 1) * sizeof(TCHAR));
    lstrcpy(lpszResult, strings[0]);

    for (int i = 1; i < len; i++) {
        lstrcat(lpszResult, strings[i]);
    }

    return lpszResult;
}

void ErrorTextOnly(HWND hwnd, DWORD textInd) {
    LPTSTR errorTitle = GetStringFromResource(TEXT_ERROR);
    LPTSTR errorMessage = GetStringFromResource(textInd);
    MessageBox(hwnd, errorMessage, errorTitle, MB_OK | MB_ICONERROR);

    free(errorTitle);
    free(errorMessage);
}

int ErrorIncorrectFilePath(HWND hwnd, LPTSTR fSource) {
    DWORD attributes = GetFileAttributes(fSource);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        LPTSTR errorTitle = GetStringFromResource(TEXT_ERROR);
        LPTSTR errorMessage = GetStringFromResource(TEXT_FILE_NOT_EXIST);
        LPTSTR lArr[] = { "\"", fSource, "\"", errorMessage };
        LPTSTR data = ConcatenateStrings(lArr, 4);
        MessageBox(hwnd, data, errorTitle, MB_OK | MB_ICONERROR);

        free(errorTitle);
        free(errorMessage);
        free(data);
        return 1;
    }
    else if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        LPTSTR errorTitle = GetStringFromResource(TEXT_ERROR);
        LPTSTR errorMessage = GetStringFromResource(TEXT_NOT_FILE);
        LPTSTR lArr[] = { "\"", fSource, "\"", errorMessage };
        LPTSTR data = ConcatenateStrings(lArr, 4);
        MessageBox(hwnd, data, errorTitle, MB_OK | MB_ICONERROR);

        free(errorTitle);
        free(errorMessage);
        free(data);
        return 1;
    }
    return 0;
}

int ErrorIncorrectDirPath(HWND hwnd, LPTSTR dDest) {
    DWORD attributes = GetFileAttributes(dDest);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        LPTSTR errorTitle = GetStringFromResource(TEXT_ERROR);
        LPTSTR errorMessage = GetStringFromResource(TEXT_FILE_NOT_EXIST);
        LPTSTR lArr[] = { "\"", dDest, "\"", errorMessage };
        LPTSTR data = ConcatenateStrings(lArr, 4);
        MessageBox(hwnd, data, errorTitle, MB_OK | MB_ICONERROR);

        free(errorTitle);
        free(errorMessage);
        free(data);
        return 1;
    }
    else if (!(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        LPTSTR errorTitle = GetStringFromResource(TEXT_ERROR);
        LPTSTR errorMessage = GetStringFromResource(TEXT_NOT_DIR);
        LPTSTR lArr[] = { "\"", dDest, "\"", errorMessage };
        LPTSTR data = ConcatenateStrings(lArr, 4);
        MessageBox(hwnd, data, errorTitle, MB_OK | MB_ICONERROR);

        free(errorTitle);
        free(errorMessage);
        free(data);
        return 1;
    }
    return 0;
}

int ErrorIncorrectFrequency(HWND hwnd, int freq) {
    if (freq == 0) {
        LPTSTR errorTitle = GetStringFromResource(TEXT_ERROR);
        LPTSTR errorMessage = GetStringFromResource(TEXT_ICORRECT_FREQUENCY);
        LPTSTR lArr[] = { "", errorMessage };
        LPTSTR data = ConcatenateStrings(lArr, 2);
        MessageBox(hwnd, data, errorTitle, MB_OK | MB_ICONERROR);

        free(errorTitle);
        free(errorMessage);
        free(data);
        return 1;
    }
    return 0;
}

void InformationSuccessCreating(HWND hwnd, LPTSTR fSource, LPTSTR dDest, int minFreq) {
    LPTSTR lh = (LPTSTR)malloc((5) * sizeof(TCHAR));
    swprintf(lh, sizeof(lh) / sizeof(TCHAR), L"%d", minFreq / 60);

    LPTSTR lm = (LPTSTR)malloc((5) * sizeof(TCHAR));
    swprintf(lm, sizeof(lm) / sizeof(TCHAR), L"%d", minFreq % 60);

    LPTSTR file = GetStringFromResource(TEXT_FILE);
    LPTSTR to = GetStringFromResource(TEXT_TO);
    LPTSTR freq = GetStringFromResource(TEXT_FREQ);
    LPTSTR hour = GetStringFromResource(TEXT_HOUR);
    LPTSTR minute = GetStringFromResource(TEXT_MINUTE);
    LPTSTR lArr[] = { file, _T(" \""), fSource, _T("\" "), to, _T(" \""),
                        dDest, _T("\" "), freq, " ",  lh, hour, " ", lm, minute };
    LPTSTR data = ConcatenateStrings(lArr, 15);
    MessageBox(hwnd, data, GetStringFromResource(TEXT_INFO), MB_OK);
    free(data); free(lh); free(lm); free(file);
    free(to); free(freq); free(hour); free(minute);
}

int GetEnteredNumber(LPTSTR number) {
    int n = _tstoi(number);
    if (n > MAX_NUMUPDOWN_VALUE)
        return MAX_NUMUPDOWN_VALUE;
    else
        return n;
}

int WriteDataIntoBinFile(const struct dataAboutFile* data) {
    int recordCount;
    if ((recordCount = RecordCountInBinFile()) == -1)
        return -1;

    if (recordCount >= MAX_RECORDS)
        return 1;

    HANDLE hFile = CreateFile(_T("datafile.bin"), GENERIC_WRITE,            
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

int RecordCountInBinFile() {
    struct dataAboutFile* data = (struct dataAboutFile*)malloc(sizeof(struct dataAboutFile) * MAX_RECORDS);
    HANDLE hFile = CreateFile(_T("datafile.bin"), GENERIC_READ,
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