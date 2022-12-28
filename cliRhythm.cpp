#include "cliRhythm.h"

cliRhythm::cliRhythm() {}
cliRhythm::~cliRhythm() {
	Outfile.close();
	Infile.close();
}
cliRhythm::cliRhythm(string username) {// ������ ���� ������
	_username = username;
	char mode[100];
	sprintf(mode,"mode con: cols=%d lines=%d",COLS,LINES);
	system(mode);
	system("title cli_�������_0.0.1v");

	// �� �̸��� �����´�.
	// ���߿� ������ �ҷ����ų� �ؾ��Ѵٸ� ���� �����ľ��ҵ�. ���� ���� ���� �ʱ�ȭ�ؼ� ���⶧��(trunc)
	// Ȥ�ø��� �о� �Ѵ� ���������
	Outfile.open("RhythmData\\Datas.txt",std::ios::trunc | std::ios::out);
	if (Outfile.fail()) {
		cerr << "Error!" << endl;
	}
	Infile.open("RhythmData\\Datas.txt", std::ios::in);
	if (Infile.fail()) {
		cerr << "Error!" << endl;
	}
	// ���丮 �� ��������(*.txt) ����� �ҷ���
	fs::directory_iterator itr(fs::current_path() / "RhythmData");
	// ���丮 �� ���� ��ü�� ���� �ݺ���
	while (itr != fs::end(itr)) {
		// ��������
		const fs::directory_entry& entry = *itr;
		string songPath = entry.path().generic_string();		
		// path���� ���� �޺κ��� ���ϴ� �����̸��̴ϱ� �װ͸� ���� 
		size_t previous = 0, current;
		current = songPath.find('//');
		while (current != string::npos) {
			previous = current + 1;
			current = songPath.find('//', previous);
		}
		// Ȯ���� �κб��� ����
		string songName = songPath.substr(previous);
		songName = songName.substr(0, songName.length() - 4);
		if (!songName._Equal("Datas")) {
			// Datas.txt�� songList ���Ϳ� ������
			// songList�� �����ϴ� ������ �� ��� ����� �� ������. �׷� Datas�� �����ǹ̰� �ճ� �ͱ��ѵ� Ȥ�ø���
			Infile << songName << endl;
			songList.push_back(songName);
		}
		
		itr++;
	}
	
	// ���� ���۵� ����
	ScreenInit();
}

//�������� �Լ�
void cliRhythm::ScreenInit() {// ���۵��� ����.
	screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//Ŀ�� ũ�� ���̰�, �Ⱥ��̰� �Ӽ� ����
	CURSOR.dwSize = 1;
	CURSOR.bVisible = false;
	//���ۿ� Ŀ�� �Ӽ��� ����
	SetConsoleCursorInfo(screen[0], &CURSOR);
	SetConsoleCursorInfo(screen[1], &CURSOR);
} 
void cliRhythm::ScreenRender(int x, int y, char* text) {// �޹�濡 �׸�
	DWORD whatIsThis; // ��������Ǹ� ����ٰ� ���δٴ°Ű����� �� �� �׳� ����
	COORD backBufferCursor = { x, y };
	SetConsoleCursorPosition(screen[_whatScreen], backBufferCursor);
	WriteFile(screen[_whatScreen], text, strlen(text), &whatIsThis, NULL);// �������� ��¥ ���Ͽ� ���°� �ƴ϶� NULL�ε�
}
void cliRhythm::ScreenSwitch() {// ������ ���۸� ����Ʈ�� ������, �ٸ� ������ �ε����� �̸� �����س���
	//0 1 �� �߿� �ϳ��ϱ� �׳� �� not���� �� ��
	SetConsoleActiveScreenBuffer(screen[_whatScreen]);
	_whatScreen = !_whatScreen;
}
void cliRhythm::ScreenClear(int x, int y, int x2, int y2) {//������ Ư�� �κ��� ����(��� �������� ���ű���)
	//���α��̸�ŭ �ݺ��ؼ� ������ ������ �ʱ�ȭ�Ѱ�
	for (int i = y; i <= y + y2 - y; i++) {
		COORD Coor = { x,i };
		DWORD dw;
		FillConsoleOutputCharacter(screen[_whatScreen], ' ', x2 - x, Coor, &dw);
		//��� ����, ������ڸ�, �󸶳� ����, ��� ��ǥ����, ������ ���ڸ� �������� �������ִ� ����
	}
}


//���ε��� �Լ�
void cliRhythm::Start() {//Main ȭ��
	//Ű ������ �޴� ģ��
	int key = 0;
	// ������ ȭ�鿡�� ������ Ű�Է��� ��ٸ� - kbhit()�� ����� �ʿ� ���ٰ� �Ǵ�
	while (1) {
		RenderMainUI();
		key = _getch();

		switch (key) {
		case 13:// ����
			if (mainSelect == EXITMENU) exit(0);
			else if (SelectStart() == 1) ScreenClear(0, 0, COLS, LINES);
			break;
		case 224:// �ƽ�Ű�ڵ� �� �̻�
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

		//if (key == 13) { // ����
		//	if (mainSelect == EXITMENU) exit(0);
		//	else { 
		//		if (SelectStart() == 1) continue;
		//}}
		//else if (key == 224) {
		//	key = _getch();

		//	switch (key) {
		//	case UPKEY: // ��
		//		if (mainSelect == STARTMENU) break;
		//		mainSelect = STARTMENU; break;
		//	case DOWNKEY: // �Ʒ�
		//		if (mainSelect == EXITMENU) break;
		//		mainSelect = EXITMENU; break;
		//	default:
		//		break;
		//}}
		//else continue;
	}
}

int cliRhythm::SelectStart() {// �� ���� ȭ��
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
	char* title = "CLI �������";
	int sLoc = ((COLS - 1) - 2) / 2; // ����ĭ ��ġ(selectLocation)
	ScreenClear(0, 0, COLS, LINES);

	//�׵θ� �׸�
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, 0, "-");
	for (int i = 1; i < LINES - 1; i++) { ScreenRender(0, i, "|"); ScreenRender(COLS - 1, i, "|"); }
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, LINES - 1, "-");

	//Ÿ��Ʋ �׸�
	ScreenRender(((COLS - 1) - strlen(title)) / 2, 5, title);

	//ARROW_KEY�� ���� arrow �׸�
	ScreenRender(((COLS - 1) - 2)/2, 13, "����"); ScreenRender(sLoc, 14, "����");
	if (mainSelect == STARTMENU) { ScreenRender(sLoc - 2, 13, ">"); ScreenRender(sLoc + 5, 13, "<"); }
	else if(mainSelect == EXITMENU) { ScreenRender(sLoc - 2, 14, ">"); ScreenRender(sLoc + 5, 14, "<"); }
	
	//ȭ�� ��ü
	ScreenSwitch();

}

void cliRhythm::RenderSongsUI() {
	char* title = "���� �����ϼ���";
	ScreenClear(0, 0, COLS, LINES);

	//�׵θ� �׸�
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, 0, "-");
	for (int i = 1; i < LINES - 1; i++) { ScreenRender(0, i, "|"); ScreenRender(COLS - 1, i, "|"); }
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, LINES - 1, "-");

	//Ÿ��Ʋ �׸�
	ScreenRender(((COLS - 1) - strlen(title)) / 2, 5, title);

	//�� ��� �ڽ� �׸�
	for (int i = SboxLT.X; i < SboxRD.X; i++) ScreenRender(i, SboxLT.Y, "-");
	for (int i = SboxLT.Y + 1; i < SboxRD.Y; i++) { ScreenRender(SboxLT.X - 1, i, "|"); ScreenRender(COLS - SboxLT.X, i, "|"); }
	for (int i = SboxLT.X; i < SboxRD.X; i++) ScreenRender(i, SboxRD.Y, "-");

	//ARROW_KEY�� ���� arrow �׸�
	ScreenRender(SboxLT.X - 3, (SboxLT.Y + 1) + songBoxArrow, ">");
	
	//�� ��� ���
	for (int i = 0; i < SboxRD.Y - SboxLT.Y - 1; i++) 
		ScreenRender(SboxLT.X + 1, SboxLT.Y + 1 + i, (char*)songList[songBoxFirst + i].c_str());

	//����� ���õǾ����� Ȯ�ο�
	ScreenRender(1, LINES - 2, (char*)songList[songSelect].c_str());


	ScreenSwitch();
}

void cliRhythm::RenderInGameUI() {
	system("cls");
	std::cout.imbue(std::locale("kor"));

	//std::cout << L"��" << setw(COLS - 2) << setfill(L'��') << L"��" << endl; std::cout << setfill(L' ');

	//for (int i = 0; i < 39; i++) std::cout << L"��" << setw(19) << L"��" << setw(COLS - 40) << L"��" << setw(19) << L"��" << endl;

	//std::cout << L"��                  ����������������������������������������������������������������������������������                    ��" << endl;
	//std::cout << L"��" << setw(19) << L"�� \u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2 \u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2 \u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2 \u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2\u25A2 ��"<< endl;

	//for (int i = 0; i < 2; i++) std::cout << L"��" << setw(20) << L"��" << setw(COLS - 42) << L"��" << setw(20) << L"��" << endl;

	//std::cout << L"��" << setw(COLS - 2) << setfill(L'��') << L"��" << endl; std::cout << setfill(L' ');

}