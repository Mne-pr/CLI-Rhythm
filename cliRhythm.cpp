#include "cliRhythm.h"

cliRhythm::cliRhythm() {}
cliRhythm::~cliRhythm() {
	Outfile.close();
	Infile.close();
}

cliRhythm::cliRhythm(string username) {
	_username = username;
	char mode[100];
	sprintf(mode,"mode con: cols=%d lines=%d",COLS,LINES);
	system(mode);
	system("title cli_리듬게임_0.0.1v");

	//이제 여기서 곡 이름만 찾자.. 나중에 점수를 불러오거나 해야한다면 구조 뜯어고쳐야할듯 
	Outfile.open("RhythmData\\Datas.txt",std::ios::trunc | std::ios::out);
	if (Outfile.fail()) {
		cerr << "Error!" << endl;
	}
	Infile.open("RhythmData\\Datas.txt", std::ios::in);
	if (Infile.fail()) {
		cerr << "Error!" << endl;
	}
	
	//디렉토리 속 리듬파일(*.txt) 목록을 불러옴
	fs::directory_iterator itr(fs::current_path() / "RhythmData");
	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		string a = entry.path().generic_string();		
		
		size_t previous = 0, current;
		current = a.find('//');
		while (current != string::npos) {
			previous = current + 1;
			current = a.find('//', previous);
		}
		string sub = a.substr(previous);

		sub = sub.substr(0, sub.size() - 4);
		if (!sub._Equal("Datas")) 
			Infile << sub << endl;
		
		itr++;
	}
}

void cliRhythm::Start() {//Main 화면
	int key = 0;
	PaintMainUI();
	// 정적인 화면에서 유저의 키입력을 기다림 - kbhit()를 사용할 필요 없다고 판단
	while (1) {
		key = _getch();

		if (key == 13) { // 엔터
			if (mainSelect == EXITMENU) exit(0);
			else {
				if (SelectStart(key) == 1) {
					PaintMainUI();
				}
			}
		}
		else if (key == 224) {
			key = _getch();
			switch (key) {
			case UPKEY: // 위
				if (mainSelect == STARTMENU) break;
				mainSelect = STARTMENU;
				PaintMainUI();
				break;
			case DOWNKEY: // 아래
				if (mainSelect == EXITMENU) break;
				mainSelect = EXITMENU;
				PaintMainUI();
				break;
			default:
				break;
			}
		}
		else 
			continue;
	}
}

int cliRhythm::SelectStart(int key) {
	key = 0;
	PaintSongsUI();

	while (1) {
		key = _getch();

		if (key == 113 || key == 81) { // q key
			return(1);
		}
		else if (key == 13) {

		}
		else if (key == 224) {

		}
		else
			continue;


	}
}

void cliRhythm::PaintMainUI() {
	system("cls");
	std::wcout.imbue(std::locale("kor"));

	std::wcout << L"┌" << setw(COLS - 2) << setfill(L'─') << L"┐" << endl; std::wcout << setfill(L' ');

	for (int i = 0; i < 5; i++) std::wcout <<  L"│" << setw(COLS - 2) << L"│" << endl;
	
	std::wcout << L"│ " << setw((COLS - 3) / 2 + strlen("RhythmGame") / 2 + 1);
	std::wcout << L"RhythmGame" << setw((COLS - 3) / 2 - strlen("RhythmGame") / 2) << L"│" << endl;

	for (int i = 0; i < 5; i++) std::wcout << L"│" << setw(COLS - 2) << L"│" << endl;

	if (mainSelect == STARTMENU) {
		std::wcout << L"│ " << setw((COLS - 3) / 2 + strlen("> 시작") / 2 - 3);
		std::wcout << L"> 시작" << setw((COLS - 3) / 2 - strlen("> 시작") / 2 + 2) << L"│" << endl;
		std::wcout << L"│ " << setw((COLS - 3) / 2 + strlen("  종료") / 2 - 3);
		std::wcout << L"  종료" << setw((COLS - 3) / 2 - strlen("  종료") / 2 + 2) << L"│" << endl;
	}
	else {
		std::wcout << L"│ " << setw((COLS - 3) / 2 + strlen("  시작") / 2 - 3);
		std::wcout << L"  시작" << setw((COLS - 3) / 2 - strlen("  시작") / 2 + 2) << L"│" << endl;
		std::wcout << L"│ " << setw((COLS - 3) / 2 + strlen("> 종료") / 2 - 3);
		std::wcout << L"> 종료" << setw((COLS - 3) / 2 - strlen("> 종료") / 2 + 2) << L"│" << endl;
	}

	for (int i = 0; i < 30 ; i++) std::wcout << L"│" << setw(COLS - 2) << L"│" << endl;
	std::wcout << L"└" << setw(COLS - 2) << setfill(L'─') << L"┘" << endl; std::wcout << setfill(L' ');
}

void cliRhythm::PaintSongsUI() {
	system("cls");

	std::wcout << L"┌" << setw(COLS - 2) << setfill(L'─') << L"┐" << endl; std::wcout << setfill(L' ');

	for (int i = 0; i < 5; i++) std::wcout << L"│" << setw(COLS - 2) << L"│" << endl;

	std::wcout << L"│ " << setw((COLS - 3) / 2 + strlen("Select Song") / 2 + 1);
	std::wcout << L"Select Song" << setw((COLS - 3) / 2 - strlen("Select Song") / 2) << L"│" << endl;

	for (int i = 0; i < 5; i++) std::wcout << L"│" << setw(COLS - 2) << L"│" << endl;

	

	
}
