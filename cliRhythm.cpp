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
		// 경로 제거
		string songName = songPath.substr(previous);
		if (!songName._Equal("Datas")) {
			// Datas.txt와 songList 벡터에 저장
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
void cliRhythm::ScreenRender(int x, int y, char ch) {// 뒷배경에 그림
	DWORD whatIsThis; 
	COORD backBufferCursor = { x, y };
	SetConsoleCursorPosition(screen[_whatScreen], backBufferCursor);
	WriteFile(screen[_whatScreen], (char*)ch, 1, &whatIsThis, NULL);
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

void cliRhythm::PushKey() {
	int key = 0;
	ScreenClear(GboxLT.X + 1, GboxRD.Y - 1, GboxRD.X - 1, GboxRD.Y - 1);
	for (int j = 1; j <= 3; j++) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * j, GboxRD.Y - 1, "|");
	
	if (kbhit()) {
		// 클릭부분 그림
		key = getch();
		if (key == EXITKEY1 || key == EXITKEY2) {}
		if (key == DKEY || key == SDKEY) { ScreenRender(GboxLT.X + 1, GboxRD.Y - 1, "@@@@@@@@@"); Pan(DKEY); }
		if (key == FKEY || key == SFKEY) { ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * 1 + 1, GboxRD.Y - 1, "@@@@@@@@@"); Pan(FKEY); }
		if (key == JKEY || key == SJKEY) { ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * 2 + 1, GboxRD.Y - 1, "@@@@@@@@@");  Pan(JKEY); }
		if (key == KKEY || key == SKKEY) { ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * 3 + 1, GboxRD.Y - 1, "@@@@@@@@@"); Pan(KKEY); }
		else {}
	}
}
void cliRhythm::Pan(int key) {
	if (key == DKEY) {

	}
}

void cliRhythm::RenderMainUI(int noswitch) {
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
	ScreenRender(((COLS - 1) - 2) / 2, 13, "시작"); ScreenRender(sLoc, 14, "종료");
	if (mainSelect == STARTMENU) { ScreenRender(sLoc - 2, 13, ">"); ScreenRender(sLoc + 5, 13, "<"); }
	else if (mainSelect == EXITMENU) { ScreenRender(sLoc - 2, 14, ">"); ScreenRender(sLoc + 5, 14, "<"); }

	//화면 교체
	if (noswitch == 0) ScreenSwitch();

}
void cliRhythm::RenderSongsUI(int noswitch) {
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
	for (int i = 0; i < SboxRD.Y - SboxLT.Y - 1; i++) {
		if (i + 1 > songList.size()) break;
		ScreenRender(SboxLT.X + 1, SboxLT.Y + 1 + i, (char*)songList[songBoxFirst + i].c_str());
	}

	//제대로 선택되었는지 확인용
	ScreenRender(1, LINES - 2, (char*)songList[songSelect].c_str());


	if (noswitch == 0) ScreenSwitch();
}
void cliRhythm::RenderInGameUI(char* songName, int noswitch) {
	ScreenClear(0, 0, COLS, LINES);
	
	// 테두리 그림
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, 0, "-");
	for (int i = 1; i < LINES - 1; i++) { ScreenRender(0, i, "|"); ScreenRender(COLS - 1, i, "|"); }
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, LINES - 1, "-");

	// 타이틀 그림
	ScreenRender(1, 1, songName);

	// 리겜화면 그림
	for (int i = GboxLT.Y; i < GboxRD.Y; i++) { ScreenRender(GboxLT.X - 1, i, "□"); ScreenRender(COLS - GboxLT.X-1, i, "□"); }
	for (int i = GboxLT.Y; i < GboxRD.Y; i++) { 
		for (int j = 1; j <= 3; j++) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * j, i, "|");
	}
	for (int i = GboxLT.X + 1; i < GboxRD.X - 1; i++) ScreenRender(i, GboxRD.Y - 2, "-");
	for (int i = GboxLT.X + 1; i < GboxRD.X - 1; i++) ScreenRender(i, GboxRD.Y, "-");


	ScreenRender(1, 2, score);

	if (noswitch == 0) ScreenSwitch();

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
		case EXITKEY1:
		case EXITKEY2:
			exit(0); break;
		case ENTERKEY:
			if (mainSelect == EXITMENU) exit(0);
			else if (SelectStart() == 1) ScreenClear(0, 0, COLS, LINES);
			break;
		case OVERASCII:
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
	}
}
int cliRhythm::SelectStart() {// 곡 선택 화면
	int key = 0;

	while (1) {
		RenderSongsUI();
		key = _getch();

		switch (key) {
		case EXITKEY1:
		case EXITKEY2:
			return(1); break;
		case ENTERKEY:
			if (InGameStart() == 1) { 
				Gamefile.close();
				//음악파일 연것도 닫아야할지 생각
				continue; 
			}
			break;
		case OVERASCII:
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
			default: break;
			}
		default: break;
		}
	}
}
int cliRhythm::InGameStart() {
	int key = 0; int statusX = 1, statusY = LINES - 2;
	char* songName = (char*)songList[songSelect].c_str();
	RenderInGameUI(songName);

	try {
		//채보파일 오픈
		Gamefile.open("RhythmData\\" + songList[songSelect] + "\\" + songList[songSelect] + ".txt");
		//채보파일 정상오픈여부 = 존재여부 확인
		RenderInGameUI(songName, 1);
		if (Gamefile.is_open()) {ScreenRender(statusX, statusY, "채보로딩완료 "); ScreenSwitch();}
		else { throw 1; }
		//노래파일 있는지 확인하고 없으면 2던지자..
		
	}
	catch (int ex) {
		if (ex == 1) {
			//파일이 존재하지 않은 경우
			for (int i = 3; i > 0; i--) {
				char temp[50]=""; sprintf(temp, "이전 화면으로 돌아갑니다 %d", i);
				ScreenRender(statusX, statusY - 1, "채보로딩실패 - 파일없음"); ScreenRender(statusX, statusY, temp);
				ScreenSwitch(); Sleep(1000);
			}
			return(1);
		}
		if (ex == 2) {
			for (int i = 3; i > 0; i--) {
				char temp[50] = ""; sprintf(temp, "이전 화면으로 돌아갑니다 %d", i);
				ScreenRender(statusX, statusY - 1, "음악로딩실패 - 파일없음"); ScreenRender(statusX, statusY, temp);
				ScreenSwitch(); Sleep(1000);
			}
			return(1);
		}
		else { return(1); }
	}
	
	//버퍼 초기화
	for (int y = 0; y < 38; y++) {
		for (int x = 0; x < 4; x++) Gbuffer[y][x] = 0;
	}

	//카운트다운
	for (int i = 0; i < 3; i++) {
		char* list[] = { "'1'","'2'","'3'"};
		ScreenRender(COLS / 2 - 1, LINES / 2 + 1, "   ");
		ScreenRender(COLS / 2 - 1, LINES / 2 - 1, "   ");
		ScreenRender(COLS / 2 - 1, LINES / 2, list[2 - i]); ScreenSwitch(); Sleep(1000);
	}
	RenderInGameUI(songName); RenderInGameUI(songName);

	//txt파일의 start까지 커서이동
	char line[10] = "";
	while (strcmp(line,"start\0")!=0 ) {
		Gamefile.getline(line, 8);
	}

	//스레드는 포기. 클래스 내부의 메소드로 스레드 돌릴 수 없음
	//그냥 c언어처럼 만들었어야 했었나봄..

	// 노래 틀기 - 지금 안되고있음
	//try {
	//	char SongFile[100] = "";
	//	//sprintf(SongFile, "RhythmData\\%s\\%s.wav", songList[songSelect], songList[songSelect]);
	//	PlaySound(L"RhythmData\\ssibal\\ssibal.wav", NULL, SND_FILENAME | SND_ASYNC);
	//	//ScreenRender(0, 0, "노래 시작"); ScreenSwitch();
	//}
	//catch (exception ex) {
	//	char* why = (char*)ex.what();
	//	ScreenRender(0, 0, why); ScreenSwitch();
	//	Sleep(3000); return(1);
	//}

	while (1) {
		//클릭삭제
		ScreenClear(GboxLT.X + 1, GboxRD.Y - 1, GboxRD.X - 1, GboxRD.Y - 1);
		for (int j = 1; j <= 3; j++) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * j, GboxRD.Y - 1, "|");
		//노트삭제
		ScreenClear(GboxLT.X + 1, 1, GboxRD.X - 1, GboxRD.Y - 3);
		
		//나머지줄 렌더링. 위에를 아래로 내리기만 하면됨
		for (int y = GboxRD.Y - 3; y >= GboxLT.Y; y--) {
			//그 외의 줄은 그냥 다음으로 내리고 렌더링
			for (int x = 0; x < 4; x++) {
				int temp = Gbuffer[y - 1][x];
				Gbuffer[y][x] = temp;
				Gbuffer[y - 1][x] = 0;
				if (Gbuffer[y][x] == 1) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * x, y, "#########");
			}
		}

		LastNote++;
		if (LastNote >= 100 && EoF == 1) {
			EoF = 0;
			Gamefile.close();
			return(1);
		}
		//첫째줄 렌더링
		// start 아래부터 라인 입력받고 파일의 끝까지 읽은거라면 그냥패스
		try {
			Gamefile.getline(line, 8);
			if (Gamefile.eof()) EoF = 1;
		}
		catch (exception) {}

		//최대 4노트까지 입력받을 수 있으므로 0,2,4,6 인덱스의 값 검사

		for (int x = 0; x < 4; x++) { 
			if (line[0] == '\0') break;
			//빈 라인이 아닌경우
			if (EoF != 1) LastNote = 0;
			int where = line[2 * x] - '1'; Gbuffer[0][where] = 1;
			//ScreenRender(GboxLT.X + 1 + (GboxRD.X - GboxLT.X) / 4 * where, GboxLT.Y, "#########");
			//더 이상 중복된 노트가 없는 경우 나가
			if (line[2 * x + 1] == '\0') {/* for (int t = x+1; t < 4; t++) Gbuffer[0][t] = 0;*/ break; }
		}

		//집나간 라인선들 다시 만들어
		for (int i = GboxLT.Y; i < GboxRD.Y - 2; i++) {
			for (int j = 1; j <= 3; j++) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * j, i, "|");
		}

		PushKey();
		ScreenSwitch();

		Sleep(16);

	}
}