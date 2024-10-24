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

#define ID_BTN_ADD 101
#define ID_BTN_HIDE_ADD 102
#define ID_BTN_LANG 103
#define ID_BTN_SELECT_FILE 104
#define ID_BTN_SELECT_DIR 105
#define ID_BTN_CREATE 106

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
};

struct component cmpInfo[] = {
    {NULL, "button", BUTTON_ADD, ID_BTN_ADD, 10, 10, BUTTON_WIDTH, BUTTON_HEIGHT, SW_SHOW},
    {NULL, "label", LABEL_SOURCE, ID_LBL_SOURCE, 120, 10, 150, 20, SW_HIDE},
    {NULL, "label", LABEL_DEST, ID_LBL_DEST, 120, 40, 150, 20, SW_HIDE},
    {NULL, "label", LABEL_FREQUENCY, ID_LBL_FREQUENCY, 120, 70, 150, 20, SW_HIDE},
    {NULL, "label", LABEL_HOUR, ID_LBL_HOUR, 280, 70, 10, 20, SW_HIDE},
    {NULL, "label", LABEL_MINUTE, ID_LBL_MINUTE, 360, 70, 25, 20, SW_HIDE},
    {NULL, "edit", 0, ID_EDIT_SOURCE, 280, 10, EDIT_WIDTH, EDIT_HEIGHT, SW_HIDE},
    {NULL, "edit", 0, ID_EDIT_DEST, 280, 40, EDIT_WIDTH, EDIT_HEIGHT, SW_HIDE},
    {NULL, "nedit", 0, ID_EDIT_HOUR, 290, 70, 50, 20, SW_HIDE},
    {NULL, "updown", 0, ID_UPDOWN_HOUR, 0, 0, 0, 0, SW_HIDE},
    {NULL, "nedit", 0, ID_EDIT_MINUTE, 390, 70, 50, 20, SW_HIDE},
    {NULL, "updown", 0, ID_UPDOWN_MINUTE, 0, 0, 0, 0, SW_HIDE},
    {NULL, "button", BUTTON_SELECT_FILE, ID_BTN_SELECT_FILE, 690, 8, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE},
    {NULL, "button", BUTTON_SELECT_DIR, ID_BTN_SELECT_DIR, 690, 38, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE},
    {NULL, "button", BUTTON_CREATE, ID_BTN_CREATE, 690, 70, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE},
    {NULL, "button", BUTTON_HIDE_ADD, ID_BTN_HIDE_ADD, 120, 100, BUTTON_WIDTH, BUTTON_HEIGHT, SW_HIDE},

    {NULL, "button", BUTTON_LANG, ID_BTN_LANG, 0, 0, 40, BUTTON_HEIGHT, SW_SHOW},

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

// Get a string in the selected language
LPTSTR GetStringFromResource(UINT id) {
    HINSTANCE hInstance = GetModuleHandle(NULL); 
    static TCHAR buffer[256];

    if (LoadString(hInstance, id, buffer, sizeof(buffer) / sizeof(TCHAR))) {
        return buffer; 
    }
    else {
        return TEXT("Œ¯Ë·Í‡"); 
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
    return CreateWindow(TEXT("BUTTON"), GetStringFromResource(btn.name),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            btn.x, btn.y, btn.width, btn.height,
            hwnd, (HMENU)btn.id, NULL, NULL);
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
    return CreateWindow(TEXT("static"), GetStringFromResource(lbl.name),
        WS_CHILD | WS_VISIBLE,
        lbl.x, lbl.y, lbl.width, lbl.height,
        hwnd, (HMENU)lbl.id, NULL, NULL);
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
        OnClickButtonAdd();
        break;
    }
    case ID_BTN_HIDE_ADD: {
        HideComponentsToAdd();
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
    }
}

void OnClickButtonAdd() {
    for (int i = 1; i <= GetIndexOfComponent(ID_BTN_HIDE_ADD); i++) {
        ShowWindow(cmpInfo[i].cmp, SW_SHOW);
        cmpInfo[i].initShow = SW_SHOW;
    }
}

void HideComponentsToAdd() {
    for (int i = 1; i <= GetIndexOfComponent(ID_BTN_HIDE_ADD); i++) {
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
            SetWindowText(cmpInfo[i].cmp, GetStringFromResource(cmpInfo[i].name));
        }
    }
}

void OnClickSelectFile(HWND hwnd) {
    char* filePath = OpenFileDialog(hwnd);
    if (filePath != NULL && filePath[0] != '\0')
        SetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_SOURCE)].cmp, filePath);
}

void OnClickSelectDir(HWND hwnd) {
    char* filePath = SelectFolder(hwnd);
    if (filePath != NULL && filePath[0] != '\0')
        SetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_DEST)].cmp, filePath);
}

void OnClickButtonCreate(HWND hwnd) {
    int lenS = GetWindowTextLength(cmpInfo[GetIndexOfComponent(ID_EDIT_SOURCE)].cmp);
    int lenD = GetWindowTextLength(cmpInfo[GetIndexOfComponent(ID_EDIT_DEST)].cmp);
    int lenH = GetWindowTextLength(cmpInfo[GetIndexOfComponent(ID_EDIT_HOUR)].cmp);
    int lenM = GetWindowTextLength(cmpInfo[GetIndexOfComponent(ID_EDIT_MINUTE)].cmp);

    if (lenS <= 0 || lenD <= 0 || lenH <= 0 || lenM <= 0) {
        ErrorFieldsNotFilled(hwnd);
        return;
    }
    else {
        LPTSTR fSource = (LPTSTR)malloc((lenS + 1) * sizeof(TCHAR));
        GetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_SOURCE)].cmp, fSource, lenS + 1);
        if (ErrorIncorrectFilePath(hwnd, fSource))
            return;

        LPTSTR dDest = (LPTSTR)malloc((lenD + 1) * sizeof(TCHAR));
        GetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_DEST)].cmp, dDest, lenS + 1);
        if (ErrorIncorrectDirPath(hwnd, dDest))
            return;

        LPTSTR hour = (LPTSTR)malloc((lenH + 1) * sizeof(TCHAR));
        GetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_HOUR)].cmp, hour, lenH + 1);
        int h = GetEnteredNumber(hour);

        LPTSTR minutes = (LPTSTR)malloc((lenM + 1) * sizeof(TCHAR));
        GetWindowText(cmpInfo[GetIndexOfComponent(ID_EDIT_MINUTE)].cmp, minutes, lenM + 1);
        int m = GetEnteredNumber(minutes);

        int minFreq = h * 60 + m;
        if (ErrorIncorrectFrequency(hwnd, minFreq))
            return;

        InformationSuccessCreating(hwnd, fSource, dDest, minFreq);
        ResetComponentsCaptionsToAdd();
        HideComponentsToAdd();
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

    if (GetOpenFileName(&ofn))
    {
        return fileName;
    }

    free(fileName);
    return NULL;
}

char* SelectFolder(HWND hwnd) {
    BROWSEINFO bi = { 0 };
    bi.hwndOwner = hwnd;
    bi.lpszTitle = "Select a folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != NULL) {
        char* path = malloc(MAX_PATH);
        SHGetPathFromIDList(pidl, path);
        CoTaskMemFree(pidl);
        return path;
    }
    return NULL;
}

LPTSTR ConcatenateStrings(LPTSTR lpszStr1, LPTSTR lpszStr2)
{
    LPTSTR lpszResult = (LPTSTR)malloc((lstrlen(lpszStr1) + lstrlen(lpszStr2) + 1) * sizeof(TCHAR));
    if (lpszResult == NULL)
        return NULL;

    lstrcpy(lpszResult, lpszStr1);
    lstrcat(lpszResult, lpszStr2);

    return lpszResult;
}

void ErrorFieldsNotFilled(HWND hwnd) {
    LPTSTR data = ConcatenateStrings("", GetStringFromResource(TEXT_NOT_FILLED));
    MessageBox(hwnd, data, GetStringFromResource(TEXT_ERROR), MB_OK | MB_ICONERROR);
}

int ErrorIncorrectFilePath(HWND hwnd, LPTSTR fSource) {
    DWORD attributes = GetFileAttributes(fSource);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        LPTSTR data = ConcatenateStrings("\"", fSource);
        data = ConcatenateStrings(data, "\"");
        data = ConcatenateStrings(data, GetStringFromResource(TEXT_FILE_NOT_EXIST));
        MessageBox(hwnd, data, GetStringFromResource(TEXT_ERROR), MB_OK | MB_ICONERROR);
        return 1;
    }
    else if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        LPTSTR data = ConcatenateStrings("\"", fSource);
        data = ConcatenateStrings(data, "\"");
        data = ConcatenateStrings(data, GetStringFromResource(TEXT_NOT_FILE));
        MessageBox(hwnd, data, GetStringFromResource(TEXT_ERROR), MB_OK | MB_ICONERROR);
        return 1;
    }
    return 0;
}

int ErrorIncorrectDirPath(HWND hwnd, LPTSTR dDest) {
    DWORD attributes = GetFileAttributes(dDest);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        LPTSTR data = ConcatenateStrings("\"", dDest);
        data = ConcatenateStrings(data, "\"");
        data = ConcatenateStrings(data, GetStringFromResource(TEXT_FILE_NOT_EXIST));
        MessageBox(hwnd, data, GetStringFromResource(TEXT_ERROR), MB_OK | MB_ICONERROR);
        return 1;
    }
    else if (!(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        LPTSTR data = ConcatenateStrings("\"", dDest);
        data = ConcatenateStrings(data, "\"");
        data = ConcatenateStrings(data, GetStringFromResource(TEXT_NOT_DIR));
        MessageBox(hwnd, data, GetStringFromResource(TEXT_ERROR), MB_OK | MB_ICONERROR);
        return 1;
    }
    return 0;
}

int ErrorIncorrectFrequency(HWND hwnd, int freq) {
    if (freq == 0) {
        LPTSTR data = ConcatenateStrings("", GetStringFromResource(TEXT_ICORRECT_FREQUENCY));
        MessageBox(hwnd, data, GetStringFromResource(TEXT_ERROR), MB_OK | MB_ICONERROR);
        return 1;
    }
    return 0;
}

void InformationSuccessCreating(HWND hwnd, LPTSTR fSource, LPTSTR dDest, int minFreq) {
    LPTSTR data = ConcatenateStrings(GetStringFromResource(TEXT_FILE), " ");
    data = ConcatenateStrings(data, "\"");
    data = ConcatenateStrings(data, fSource);
    data = ConcatenateStrings(data, "\"");
    data = ConcatenateStrings(data, " ");
    data = ConcatenateStrings(data, GetStringFromResource(TEXT_TO));
    data = ConcatenateStrings(data, " ");
    data = ConcatenateStrings(data, "\"");
    data = ConcatenateStrings(data, dDest);
    data = ConcatenateStrings(data, "\"");
    data = ConcatenateStrings(data, " ");
    data = ConcatenateStrings(data, GetStringFromResource(TEXT_FREQ));
    data = ConcatenateStrings(data, " ");

    LPTSTR str = (LPTSTR)malloc((5) * sizeof(TCHAR));
    swprintf(str, sizeof(str) / sizeof(wchar_t), L"%d", minFreq / 60);
    data = ConcatenateStrings(data, str);
    data = ConcatenateStrings(data, GetStringFromResource(TEXT_HOUR));
    data = ConcatenateStrings(data, " ");
    swprintf(str, sizeof(str) / sizeof(wchar_t), L"%d", minFreq % 60);
    data = ConcatenateStrings(data, str);
    data = ConcatenateStrings(data, GetStringFromResource(TEXT_MINUTE));
    MessageBox(hwnd, data, GetStringFromResource(TEXT_INFO), MB_OK);
}

int GetEnteredNumber(LPTSTR number) {
    return _tstoi(number) % (MAX_NUMUPDOWN_VALUE + 1);
}