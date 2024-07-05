LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		switch (wp) {
		case StartGame:
			if (Words.size() > 0) {
				EnableWindow(LetterWritePanel, TRUE);
				EnableWindow(OneTipButton, TRUE);
				EnableWindow(StopButton, TRUE);
				EnableWindow(StartButton, FALSE);
				GameStarted = true;
				attempts = defAtts;
				SelectedWord = Words[randomNum(Words)];
				ClosedWord = L"";
				for (int i = 0; i < wcslen(SelectedWord.c_str()); i++) ClosedWord += CloseSymbol;
				SetWindowText(WordStatic, ClosedWord.c_str());
				if (On) MessageBox(hWnd, SelectedWord.c_str(), L"DevMode", MB_OK);
			}
			else MessageBox(hWnd, L"Список слов пуст! Вы можете загрузить свой\nНажмите Игра > Загрузить список слов...", MainWndName, MB_OK);
			break;
		case StopGame:
			if (GameStarted) {
				SetWindowText(AboutLetterStatic, L"Вы самостоятельно завершили игру.");
				OnGameStop();
				GameStarted = false;
			}
			break;
		case CheckWord:
			if (GameStarted && !WordWriteChance) {
				TCHAR buff[1024];
				GetWindowText(LetterWritePanel, buff, 1024);
				SetWindowText(LetterWritePanel, NULL);
				letter = buff;
				if (letter != L"") {
					neww = L"";
					if (std::wstring(SelectedWord).find(letter) <= std::wstring(SelectedWord).length()) {
						if (find(used_letters.begin(), used_letters.end(), letter) == used_letters.end()) {
							if (*find(ClosedWord.begin(), ClosedWord.end() - 1, letter[0]) != letter[0]) {
								for (int i = 0; i < wcslen(SelectedWord.c_str()); i++) {
									if (letter[0] == std::wstring(SelectedWord)[i]) { neww += letter[0]; newOnlyWord += letter[0]; }
									else neww += ClosedWord[i];
								}
								ClosedWord = neww;
								EnableWindow(OneTipButton, FALSE);
								SetWindowText(WordStatic, ClosedWord.c_str());
								SetWindowText(AboutLetterStatic, L"Буква есть в слове!");
								used_letters.push_back(letter);
							}
							else SetWindowText(AboutLetterStatic, L"Буква уже есть в слове.");
						}
						else SetWindowText(AboutLetterStatic, L"Буква уже была названа.");
					}
					else { 
						attempts--;
						SetWindowText(AboutLetterStatic, L"Буквы нет в слове."); 
						SetWindowText(AttemptsStatic, (L"Попытки: " + std::to_wstring(attempts)).c_str());
						used_letters.push_back(letter);
					}
					if (ClosedWord == std::wstring(SelectedWord)) {
						SetWindowText(AboutLetterStatic, L"Вы угадали слово!");
						OnGameStop();
					}
					else if (attempts <= 0) {
						SetWindowText(AboutLetterStatic, L"Попытки закончились, вы проиграли!");
						OnGameStop();
					}
					if (!WordWriteChance && newOnlyWord.length() == SelectedWord.length() / 2 && !No && GameStarted) {
						if (MessageBox(hWnd, L"Вам выпал шанс открыть слово целиком, но если ошибётесь, то проиграете.\nХотите рискнуть?", MainWndName, MB_YESNO) == IDYES) {
							WordWriteChance = true;
							SetWindowText(WordWritePanel, L"");
							SetWindowText(LetterWritePanel, L"");
							EnableWindow(WordWritePanel, true);
							EnableWindow(LetterWritePanel, false);
						}
						else No = true;
					}
				}
			}
			else if (GameStarted && WordWriteChance) {
				wchar_t TrueWord[256];
				GetWindowText(WordWritePanel, TrueWord, 256);
				if (TrueWord == SelectedWord) {
					SetWindowText(AboutLetterStatic, L"Вы угадали слово. Вы выиграли!");
					OnGameStop();
				}
				else {
					SetWindowText(AboutLetterStatic, L"Вы не угадали слово. Вы проиграли!");
					OnGameStop();
				}
			}
			break;
		case RandomTip:
			LoadTips();
			Num = randomNum(tips);
			MessageBox(hWnd, tips[Num].c_str(), MainWndName, MB_OK);
			EnableWindow(OneTipButton, FALSE);
			break;
		case LoadListOfWords:
			if (!GameStarted && GetOpenFileName(&ofn)) LoadListOfWordsFunction(PathToFile);
			MessageBox(hWnd, std::to_wstring(Words.size()).c_str(), L"a", MB_OK);
			break;
		case OpenSettings:
			if (!GameStarted) {
				EnableWindow(hWnd, FALSE);
				ShowWindow(hWndSettings, SW_NORMAL);
				UpdateWindow(hWndSettings);
			}
			else MessageBox(hWnd, L"Чтобы открыть настройки завершите игру.", MainWndName, MB_OK);
			break;
		case ExitFromSoftware:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}		
		break;
	case WM_KEYDOWN:
		switch(wp) {
		case VK_SPACE:
			if (!GameStarted) SendMessage(hWndMain, WM_COMMAND, (WPARAM)StartGame, (LPARAM)StartButton);
			else SendMessage(hWndMain, WM_COMMAND, (WPARAM)CheckWord, (LPARAM)CheckButton);
			break;
		}
		break;
	case WM_CREATE:
		FirstStartCheck();
		LoadListOfWordsFunction(PathToBaseList.c_str());
		SetOpenFileParams(hWnd);
		break;
	case WM_CLOSE:
		if (GameStarted) {
			if (MessageBox(hWnd, L"Игра не закончена, вы точно хотите выйти?", MainWndName, MB_YESNO) == IDYES) {
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}
		}
		else SendMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

LRESULT CALLBACK SettingsProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		if (HIWORD(wp) == CBN_SELCHANGE) {
			SendMessage((HWND)lp, (UINT)CB_GETLBTEXT, (WPARAM)SendMessage((HWND)lp, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0), (LPARAM)ListOfItems);
		}
		switch (wp) {
		case SetChanges:
			SetChangesSettings(hWnd);
			break;
		}
		break;
	case WM_CREATE:
		SettingsWidgets(hWnd);
		break;
	case WM_CLOSE:
		EnableWindow(hWndMain, TRUE);
		ShowWindow(hWnd, SW_HIDE);
		break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}