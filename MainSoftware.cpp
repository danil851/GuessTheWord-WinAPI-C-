#include "includes.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	LPCWSTR MainClassName = L"MainWndClass",
		SettingsClassName = L"SettingsWndClass";

	GetUserName(WinUserName, &BuffSiz);

	WNDCLASS SettingsClass = NewWindowClass(SettingsClassName, SettingsProcedure, CS_HREDRAW | CS_VREDRAW, (HBRUSH)COLOR_WINDOW,
		LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_APPLICATION), hInst, NULL, 0, 0);
	RegisterClass(&SettingsClass);
	WNDCLASS SoftwareMainClass = NewWindowClass(MainClassName, SoftwareMainProcedure, CS_HREDRAW | CS_VREDRAW, (HBRUSH)COLOR_WINDOW,
		LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_APPLICATION), hInst, NULL, 0, 0);
	if (!RegisterClass(&SoftwareMainClass)) { return -1; }

	hWndMain = CreateWindow(MainClassName, MainWndName, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, hInst, NULL);
	hWndSettings = CreateWindow(SettingsClassName, SettingsWndName, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, hInst, NULL);

	ParentCenter(hWndMain, hWndMain);
	ParentCenter(hWndSettings, hWndSettings);

	HMENU RootMenu = CreateMenu();
	HMENU GameSubMenu = CreateMenu();

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)GameSubMenu, L"Игра");
	AppendMenu(GameSubMenu, MF_STRING, LoadListOfWords, L"Загрузить список слов...");
	AppendMenu(GameSubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(GameSubMenu, MF_STRING, OpenSettings, L"Настройки");
	AppendMenu(GameSubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(GameSubMenu, MF_STRING, ExitFromSoftware, L"Выйти");

	SetMenu(hWndMain, RootMenu);

	int LetterEditAndButtonWidth = 35;

	StartButton = CreateWindow(L"button", L"Загадать", BS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 80, 20, hWndMain, (HMENU)StartGame, hInst, NULL); // 1
	StopButton = CreateWindow(L"button", L"Стоп", BS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 35, 80, 20, hWndMain, (HMENU)StopGame, hInst, NULL); // 2
	OneTipButton = CreateWindow(L"button", L"Подсказка", WS_CHILD | WS_VISIBLE | BS_CENTER | WS_BORDER, 10, 60, 80, 20, hWndMain, (HMENU)RandomTip, hInst, NULL); // 3
	WordStatic = CreateWindow(L"static", L"Здесь будет слово", WS_CHILD | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTERIMAGE | BS_MULTILINE, 95, 10, 285, 45, hWndMain, NULL, hInst, NULL); //1-2
	AboutLetterStatic = CreateWindow(L"static", L"Состояние игры", SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 95, 60, 285, 20, hWndMain, NULL, hInst, NULL); // 3
	CreateWindow(L"static", L"Ваша буква: ", WS_CHILD | WS_VISIBLE, 10, 85, 90, 20, hWndMain, NULL, hInst, NULL); // 4
	LetterWritePanel = CreateWindow(L"edit", NULL, ES_CENTER | WS_VISIBLE | WS_BORDER | WS_CHILD, 105, 85, LetterEditAndButtonWidth, 20, hWndMain, NULL, hInst, NULL); // 4
	CheckButton = CreateWindow(L"button", L"?", BS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_BORDER, 105, 110, LetterEditAndButtonWidth, 20, hWndMain, (HMENU)CheckWord, hInst, NULL); // 5
	AttemptsStatic = CreateWindow(L"static", (L"Попытки: " + std::to_wstring(defAtts)).c_str(), WS_CHILD | WS_VISIBLE, 10, 110, 90, 20, hWndMain, NULL, hInst, NULL); // 5
	WordWritePanel = CreateWindow(L"edit", L"Здесь писать слово, если выпадет шанс", ES_CENTER | WS_VISIBLE | WS_BORDER | WS_CHILD | ES_MULTILINE, 145, 85, 235, 45, hWndMain, NULL, hInst, NULL); // 4
	EnableWindow(StopButton, FALSE);
	EnableWindow(LetterWritePanel, FALSE);
	EnableWindow(OneTipButton, FALSE);
	EnableWindow(WordWritePanel, FALSE);
	SendMessage(LetterWritePanel, EM_SETLIMITTEXT, 1, 0);

	SetFocus(hWndMain);

	ShowWindow(hWndMain, ncmdshow);
	UpdateWindow(hWndMain);

	MSG SoftwareMainMessage = { 0 };
	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}
	return 0;
}

void SettingsWidgets(HWND hWnd) {
	int EditPos = 260;
	SaveChanges = CreateWindow(L"button", L"Применить", WS_CHILD | WS_BORDER | BS_CENTER | WS_VISIBLE, Width - 100, Height - 64, 80, 20, hWnd, (HMENU)SetChanges, NULL, NULL);
	DevModeCB = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 380, -10, 20, 20, hWnd, (HMENU)DevMode, NULL, NULL);
	CreateWindow(L"static", L"Закрывающий символ:", WS_CHILD | WS_VISIBLE, 10, 10, 160, 20, hWnd, NULL, NULL, NULL);
	CreateWindow(L"static", L"Кол-во попыток:", WS_CHILD | WS_VISIBLE, 10, 40, 115, 20, hWnd, NULL, NULL, NULL);
	CreateWindow(L"static", L"Очищать при загрузке списка", WS_CHILD | WS_VISIBLE, 10, 70, 220, 20, hWnd, NULL, NULL, NULL);
	ComboBox = CreateWindow(L"combobox", NULL, CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, EditPos, 10, 40, 200, hWnd, NULL, NULL, NULL);
	SetAttemptsEdit = CreateWindow(L"edit", std::to_wstring(defAtts).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, EditPos, 40, 35, 20, hWnd, NULL, NULL, NULL);
	CheckBox = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, EditPos, 70, 20, 20, hWnd, NULL, NULL, NULL);
	if (ClearListBeforeLoad) SendMessage(CheckBox, BM_SETCHECK, 1, 0);
	else SendMessage(CheckBox, BM_SETCHECK, 0, 0);
	SendMessage(ComboBox, EM_SETLIMITTEXT, 1, 0);
	memset(&A, 0, sizeof(A));
	for (int i = 0; i <= 6; i++) {
		wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)CloseSymbolsList[i]);
		SendMessage(ComboBox, (UINT)CB_ADDSTRING, 0, (LPARAM)A);
	}
	SendMessage(ComboBox, CB_SETCURSEL, (WPARAM)2, 0);
}

void OnGameStop() {
	SetWindowText(WordStatic, SelectedWord.c_str());
	SetWindowText(WordWritePanel, NULL);
	SetWindowText(AttemptsStatic, (L"Попытки: " + std::to_wstring(attempts)).c_str());
	GameStarted = false;
	No = false;
	if (WordWriteChance) { WordWriteChance = false; newOnlyWord = L""; }
	used_letters.clear();
	EnableWindow(StartButton, TRUE);
	EnableWindow(LetterWritePanel, FALSE);
	EnableWindow(OneTipButton, FALSE);
	EnableWindow(StopButton, FALSE);
	EnableWindow(WordWritePanel, false);
	EnableWindow(LetterWritePanel, true);
}

void LoadListOfWordsFunction(LPCWSTR path) {
	std::string PathSTR(WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr), 0);
	WideCharToMultiByte(CP_UTF8, 0, path, -1, &PathSTR[0], WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr), nullptr, nullptr);
	std::ifstream in(PathSTR);
	std::string line;
	if(ClearListBeforeLoad)Words.clear();
	in.imbue(std::locale(in.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));
	while (std::getline(in, line)) {
		Words.push_back(str_wstr_conv.from_bytes(line));
	}
	in.close();
}

void LoadTips() {
	bool EnglishInWord = false;
	std::wstring englAlph = L"qwertyuiopasdfghjklzxcvbnm";
	for (int i = 0; i < englAlph.length(); i++) {
		if (SelectedWord.find(englAlph[i]) != std::wstring::npos) {
			EnglishInWord = true;
			break;
		}
	}
	for (int i = 0; i < SelectedWord.length(); i++) {
		if (isupper(SelectedWord[i])) {
			tips.push_back(L"Слово содержит заглавные буквы");
			break;
		}
	}
	if (EnglishInWord) tips.push_back(L"Слово содержит английские буквы");
	else if (!EnglishInWord) tips.push_back(L"Слово НЕ содержит английские буквы");
}

void ParentCenter(HWND hWndParent, HWND hWndChild) {
	RECT prc;
	RECT crc;
	GetWindowRect(hWndParent, &prc);
	GetWindowRect(hWndChild, &crc);
	prc.right -= prc.left;
	prc.bottom -= prc.top;
	crc.right -= crc.left;
	crc.bottom -= crc.top;
	int x = prc.left + (prc.right - crc.right) / 2;
	int y = prc.top + (prc.bottom - crc.bottom) / 2;
	SetWindowPos(hWndChild, HWND_NOTOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
}

int randomNum(std::vector<std::wstring> vec) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, vec.size() - 1);
	return dist6(rng);
}

void OnSoftwareLoad() {
	std::ifstream j(PathToJson);
	json js = json::parse(j);
	j.close();
	std::string symb = js[JKsymb].get<std::string>();
	for (int i = 0; i < 8; i++) {
		if (!std::find(std::begin(CloseSymbolsList), std::end(CloseSymbolsList), (TCHAR*)symb[0])) { symb = "*"; break; }
	}
	int atts = js[JKatts].get<int>();
	if (atts > 999 || atts <= 0) atts = defAtts;
	ClearListBeforeLoad = js[JKcl].get<bool>();
	CloseSymbol = str_wstr_conv.from_bytes(symb);
	defAtts = atts;
	SetWindowText(AttemptsStatic, (L"Попытки: " + std::to_wstring(defAtts)).c_str());
}

void SetChangesSettings(HWND hWnd) {
	std::ifstream ifs(PathToJson);
	json js;
	ifs >> js;
	if (ifs.is_open()) {
		GetWindowText(SetAttemptsEdit, AttsBuffer, 128);
		if (ListOfItems != CloseSymbol && ListOfItems[0] != '\0') {
			CloseSymbol = ListOfItems;
			js[JKsymb] = str_wstr_conv.to_bytes(CloseSymbol);
		}
		if (_wtoi(AttsBuffer) != defAtts) {
			defAtts = _wtoi(AttsBuffer);
			js[JKatts] = defAtts;
			SetWindowText(AttemptsStatic, (L"Попытки: " + std::to_wstring(attempts)).c_str());
		}
		if (SendMessage(CheckBox, BM_GETCHECK, 0, 0) != BST_CHECKED) ClearListBeforeLoad = false;
		else ClearListBeforeLoad = true;
		if (SendMessage(DevModeCB, BM_GETCHECK, 0, 0) != BST_CHECKED) { On = false; SetWindowText(CheckButton, L"?"); }
		else { On = true; SetWindowText(CheckButton, L"!");  }

		js[JKcl] = bool(ClearListBeforeLoad);
		std::ofstream ofs(PathToJson);
		ofs << js;
		ifs.close();
		ofs.close();
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
	else {
		FirstStartCheck();
		MessageBox(hWnd, L"Файл state.json не удалось найти и программа создала новый. Измените значение настроек заново.", SettingsWndName, MB_OK);
	}
}

void FirstStartCheck() {
	std::ifstream ifs(PathToJson);
	if (!ifs.is_open()) {
		json obj = {
			{JKcl, ClearListBeforeLoad},
			{JKatts, defAtts},
			{JKsymb, str_wstr_conv.to_bytes(CloseSymbol)}
		};
		std::ofstream ofs(PathToJson);
		ofs << obj;
		ofs.close();
	}
	else { OnSoftwareLoad(); }
	ifs.close();
}

void SetOpenFileParams(HWND hWnd) {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = PathToFile;
	ofn.nMaxFile = sizeof(PathToFile);
	ofn.lpstrFilter = L"TXT (*.txt)\0\0";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L".\\Lists\\";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

WNDCLASS NewWindowClass(LPCWSTR ClassName, WNDPROC Procedure, UINT Styles, HBRUSH BGColor, HCURSOR Cursor, HICON Icon, HINSTANCE hInst, LPCWSTR MenuName,
	int callbackClsExtra, int callbackWndExtra) {

	WNDCLASS NWC = { 0 };

	NWC.lpszClassName = ClassName;
	NWC.lpfnWndProc = Procedure;
	NWC.style = Styles;
	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.hbrBackground = BGColor;
	NWC.lpszMenuName = MenuName;
	NWC.cbClsExtra = callbackClsExtra;
	NWC.cbWndExtra = callbackWndExtra;

	return NWC;
}