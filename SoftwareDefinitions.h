#define ExitFromSoftware 1161
#define StartGame 1
#define StopGame 2
#define CheckWord 3
#define LoadListOfWords 4
#define RandomTip 5
#define OpenSettings 6
#define SetChanges 7
#define DevMode 8

HWND hWndMain;
HWND hWndSettings;

HWND StartButton;
HWND StopButton;
HWND WordStatic;
HWND AboutLetterStatic;
HWND AttemptsStatic;
HWND LetterWritePanel;
HWND WordWritePanel;
HWND CheckButton;
HWND OneTipButton;

HWND SaveChanges;
HWND SetAttemptsEdit;
HWND ComboBox;
HWND CheckBox;
HWND DevModeCB;

DWORD BuffSiz = 128;
TCHAR WinUserName[sizeof(BuffSiz)];

wchar_t AttsBuffer[128];

int defAtts = 999, attempts = defAtts;
int Num;

TCHAR A[16];
TCHAR ListOfItems[256];
TCHAR CloseSymbolsList[7][10] = {
	TEXT("&"), TEXT("*"), TEXT("@"), 
	TEXT("!"), TEXT("%"), TEXT("$"), TEXT("?"), 
};

LPCWSTR MainWndName = L"Угадай слово";
LPCWSTR SettingsWndName = L"Настройки игры";

std::vector <std::wstring> WhatChangesSave = {};
std::vector <std::wstring> tips = {};
std::vector <std::wstring> used_letters = {};
std::vector <std::wstring> Words = {};
std::wstring PathToJson = L".\\state.json";
std::wstring PathToBaseList = L".\\list.txt";
std::wstring SelectedWord;
std::wstring ClosedWord;
std::wstring CloseSymbol = CloseSymbolsList[1];
std::wstring neww, letter, newOnlyWord;

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> str_wstr_conv;

std::string JKsymb = "symbol";
std::string JKatts = "attempts";
std::string JKcl = "cl";

bool GameStarted = false;
bool ClearListBeforeLoad = true;
bool WordWriteChance = false;
bool No = false;
bool On = false;

wchar_t PathToFile[260];
OPENFILENAME ofn;

INT Width = 405;
INT Height = 200;

WNDCLASS NewWindowClass(LPCWSTR ClassName, WNDPROC Procedure, UINT Styles, HBRUSH BGColor, HCURSOR Cursor, HICON Icon, HINSTANCE hInst, LPCWSTR MenuName, int callbackClsExtra, int callbackWndExtra);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK SettingsProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
int randomNum(std::vector<std::wstring> vec);
void LoadListOfWordsFunction(LPCWSTR path);
void SetOpenFileParams(HWND hWnd);
void SettingsWidgets(HWND hWnd);
void ParentCenter(HWND hWndParent, HWND hWndChild);
void SetChangesSettings(HWND hWnd);
void OnSoftwareLoad();
void FirstStartCheck();
void OnGameStop();
void LoadTips();