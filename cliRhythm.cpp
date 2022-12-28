#include "cliRhythm.h"

cliRhythm::cliRhythm() {}
cliRhythm::~cliRhythm() {
	Outfile.close();
	Infile.close();
}
cliRhythm::cliRhythm(string username) {// 실제로 사용될 생성자
	_username = username;
	char mode[100];
	sprintf(mode,"mode con: cols=%d lines=%d",COLS,LINES);
	system(mode);
	system("title cli_리듬게임_0.0.1v");

	// 곡 이름만 가져온다.
	// 나중에 점수를 불러오거나 해야한다면 구조 뜯어고쳐야할듯. 현재 파일 열때 초기화해서 열기때문(trunc)
	// 혹시몰라서 읽쓰 둘다 열어놓긴함
	Outfile.open("RhythmData\\Datas.txt",std::ios::trunc | std::ios::out);
	if (Outfile.fail()) {
		cerr << "Error!" << endl;
	}
	Infile.open("RhythmData\\Datas.txt", std::ios::in);
	if (Infile.fail()) {
		cerr << "Error!" << endl;
	}
	// 디렉토리 속 리듬파일(*.txt) 목록을 불러옴
	fs::directory_iterator itr(fs::current_path() / "RhythmData");
	// 디렉토리 속 파일 전체를 도는 반복문
	while (itr != fs::end(itr)) {
		// 변수선언
		const fs::directory_entry& entry = *itr;
		string songPath = entry.path().generic_string();		
		// path에서 제일 뒷부분이 원하는 파일이름이니까 그것만 추출 
		size_t previous = 0, current;
		current = songPath.find('//');
		while (current != string::npos) {
			previous = current + 1;
			current = songPath.find('//', previous);
		}
		// 확장자 부분까지 제거
		string songName = songPath.substr(previous);
		songName = songName.substr(0, songName.length() - 4);
		if (!songName._Equal("Datas")) {
			// Datas.txt와 songList 벡터에 저장함
			// songList에 저장하는 이유는 곡 목록 출력할 때 쓰려고. 그럼 Datas의 존재의미가 잇나 싶긴한데 혹시몰라서
			Infile << songName << endl;
			songList.push_back(songName);
		}
		
		itr++;
	}
	
	// 이제 버퍼들 생성
	ScreenInit();
}

//렌더관련 함수
void cliRhythm::ScreenInit() {// 버퍼들을 생성.
	screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//커서 크기 줄이고, 안보이게 속성 설정
	CURSOR.dwSize = 1;
	CURSOR.bVisible = false;
	//버퍼에 커서 속성을 지정
	SetConsoleCursorInfo(screen[0], &CURSOR);
	SetConsoleCursorInfo(screen[1], &CURSOR);
} 
void cliRhythm::ScreenRender(int x, int y, char* text) {// 뒷배경에 그림
	DWORD whatIsThis; // 오류검출되면 여기다가 쓰인다는거같은데 잘 모름 그냥 넣음
	COORD backBufferCursor = { x, y };
	SetConsoleCursorPosition(screen[_whatScreen], backBufferCursor);
	WriteFile(screen[_whatScreen], text, strlen(text), &whatIsThis, NULL);// 마지막은 진짜 파일에 쓰는게 아니라서 NULL인듯
}
void cliRhythm::ScreenSwitch() {// 저장한 버퍼를 프론트로 돌리고, 다른 버퍼의 인덱스를 미리 저장해놓음
	//0 1 둘 중에 하나니까 그냥 논리 not으로 한 것
	SetConsoleActiveScreenBuffer(screen[_whatScreen]);
	_whatScreen = !_whatScreen;
}
void cliRhythm::ScreenClear(int x, int y, int x2, int y2) {//버퍼의 특정 부분을 삭제(사실 공백으로 쓴거긴함)
	//세로길이만큼 반복해서 지정된 라인을 초기화한것
	for (int i = y; i <= y + y2 - y; i++) {
		COORD Coor = { x,i };
		DWORD dw;
		FillConsoleOutputCharacter(screen[_whatScreen], ' ', x2 - x, Coor, &dw);
		//어느 버퍼, 어느문자를, 얼마나 쓸거, 어느 좌표부터, 실제로 문자를 몇번썼는지 받을수있는 변수
	}
}


//내부동작 함수
void cliRhythm::Start() {//Main 화면
	//키 누른거 받는 친구
	int key = 0;
	// 정적인 화면에서 유저의 키입력을 기다림 - kbhit()를 사용할 필요 없다고 판단
	while (1) {
		RenderMainUI();
		key = _getch();

		switch (key) {
		case 13:// 엔터
			if (mainSelect == EXITMENU) exit(0);
			else if (SelectStart() == 1) ScreenClear(0, 0, COLS, LINES);
			break;
		case 224:// 아스키코드 그 이상
			key = _getch();
			
			switch (key) {
			case UPKEY:
			case LEFTKEY:
				if (mainSelect == STARTMENU) break;
				mainSelect = STARTMENU; break;
			case DOWNKEY:
			case RIGHTKEY:
				if (mainSelect == EXITMENU) break;
				mainSelect = EXITMENU; break;
			default:
				break;
			}

			break;
		default:
			break;
		}

		//if (key == 13) { // 엔터
		//	if (mainSelect == EXITMENU) exit(0);
		//	else { 
		//		if (SelectStart() == 1) continue;
		//}}
		//else if (key == 224) {
		//	key = _getch();

		//	switch (key) {
		//	case UPKEY: // 위
		//		if (mainSelect == STARTMENU) break;
		//		mainSelect = STARTMENU; break;
		//	case DOWNKEY: // 아래
		//		if (mainSelect == EXITMENU) break;
		//		mainSelect = EXITMENU; break;
		//	default:
		//		break;
		//}}
		//else continue;
	}
}

int cliRhythm::SelectStart() {// 곡 선택 화면
	int key = 0;

	while (1) {
		RenderSongsUI();
		key = _getch();

		switch (key) {
		case 113:
		case 81:
			return(1);
			break;
		case 13:
			if (InGameStart() == 1) continue;
			break;
		case 224:
			key = _getch();
			switch (key) {
			case UPKEY:
			case LEFTKEY:
				if (songSelect == 0) break;
				songBoxArrow--;
				if (songBoxArrow < 0) { songBoxArrow = 0; songBoxFirst--; }
				songSelect--; break;
			case DOWNKEY:
			case RIGHTKEY:
				if (songSelect == songList.size() - 1) break;
				songBoxArrow++;
				if (songBoxArrow > SboxRD.Y - SboxLT.Y - 2) { songBoxArrow = SboxRD.Y - SboxLT.Y - 2; songBoxFirst++; }
				songSelect++; break;
			default:
				break;
			}
		default:
			break;
		}
	}
}

int cliRhythm::InGameStart() {
	int key = 0;
	RenderInGameUI();
	//Gamefile.open("RhythmData\\"+ songList[songSelect]);
	//while (1) {
	//	
	//}

	while (1) {
		if (kbhit()) {
			key = getch();
			switch (key) {
			case 113:
			case 81:
				return(1);
				break;
			default:
				break;
			}
		}
	}
}


void cliRhythm::RenderMainUI() {
	char* title = "CLI 리듬게임";
	int sLoc = ((COLS - 1) - 2) / 2; // 선택칸 위치(selectLocation)
	ScreenClear(0, 0, COLS, LINES);

	//테두리 그림
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, 0, "-");
	for (int i = 1; i < LINES - 1; i++) { ScreenRender(0, i, "|"); ScreenRender(COLS - 1, i, "|"); }
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, LINES - 1, "-");

	//타이틀 그림
	ScreenRender(((COLS - 1) - strlen(title)) / 2, 5, title);

	//ARROW_KEY에 따른 arrow 그림
	ScreenRender(((COLS - 1) - 2)/2, 13, "시작"); ScreenRender(sLoc, 14, "종료");
	if (mainSelect == STARTMENU) { ScreenRender(sLoc - 2, 13, ">"); ScreenRender(sLoc + 5, 13, "<"); }
	else if(mainSelect == EXITMENU) { ScreenRender(sLoc - 2, 14, ">"); ScreenRender(sLoc + 5, 14, "<"); }
	
	//화면 교체
	ScreenSwitch();

}

void cliRhythm::RenderSongsUI() {
	char* title = "곡을 선택하세요";
	ScreenClear(0, 0, COLS, LINES);

	//테두리 그림
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, 0, "-");
	for (int i = 1; i < LINES - 1; i++) { ScreenRender(0, i, "|"); ScreenRender(COLS - 1, i, "|"); }
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, LINES - 1, "-");

	//타이틀 그림
	ScreenRender(((COLS - 1) - strlen(title)) / 2, 5, title);

	//곡 목록 박스 그림
	for (int i = SboxLT.X; i < SboxRD.X; i++) ScreenRender(i, SboxLT.Y, "-");
	for (int i = SboxLT.Y + 1; i < SboxRD.Y; i++) { ScreenRender(SboxLT.X - 1, i, "|"); ScreenRender(COLS - SboxLT.X, i, "|"); }
	for (int i = SboxLT.X; i < SboxRD.X; i++) ScreenRender(i, SboxRD.Y, "-");

	//ARROW_KEY에 따른 arrow 그림
	ScreenRender(SboxLT.X - 3, (SboxLT.Y + 1) + songBoxArrow, ">");
	
	//곡 목록 출력
	for (int i = 0; i < SboxRD.Y - SboxLT.Y - 1; i++) 
		ScreenRender(SboxLT.X + 1, SboxLT.Y + 1 + i, (char*)songList[songBoxFirst + i].c_str());

	//제대로 선택되었는지 확인용
	ScreenRender(1, LINES - 2, (char*)songList[songSelect].c_str());


	ScreenSwitch();
}

void cliRhythm::RenderInGameUI() {
	system("cls");
	std::cout.imbue(std::locale("kor"));

	//std::cout << L"┌" << setw(COLS - 2) << setfill(L'─') << L"┐" << endl; std::cout << setfill(L' ');

	//for (int i = 0; i < 39; i++) std::cout << L"│" << setw(19) << L"│" << setw(COLS - 40) << L"│" << setw(19) << L"│" << endl;

	//std::cout << L"│                  ├───────────────────────────────────────┤                    │" << endl;
	//std::cout << L"│" << setw(19) << L"│ \u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2 \u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2 \u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2 \u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2 │"<< endl;

	//for (int i = 0; i < 2; i++) std::cout << L"│" << setw(20) << L"│" << setw(COLS - 42) << L"│" << setw(20) << L"│" << endl;

	//std::cout << L"└" << setw(COLS - 2) << setfill(L'─') << L"┘" << endl; std::cout << setfill(L' ');

}