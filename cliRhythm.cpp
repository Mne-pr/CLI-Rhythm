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
		// ��� ����
		string songName = songPath.substr(previous);
		if (!songName._Equal("Datas")) {
			// Datas.txt�� songList ���Ϳ� ����
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
void cliRhythm::ScreenRender(int x, int y, char ch) {// �޹�濡 �׸�
	DWORD whatIsThis; 
	COORD backBufferCursor = { x, y };
	SetConsoleCursorPosition(screen[_whatScreen], backBufferCursor);
	WriteFile(screen[_whatScreen], (char*)ch, 1, &whatIsThis, NULL);
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

void cliRhythm::PushKey() {
	int key = 0;
	ScreenClear(GboxLT.X + 1, GboxRD.Y - 1, GboxRD.X - 1, GboxRD.Y - 1);
	for (int j = 1; j <= 3; j++) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * j, GboxRD.Y - 1, "|");
	
	if (kbhit()) {
		// Ŭ���κ� �׸�
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
	ScreenRender(((COLS - 1) - 2) / 2, 13, "����"); ScreenRender(sLoc, 14, "����");
	if (mainSelect == STARTMENU) { ScreenRender(sLoc - 2, 13, ">"); ScreenRender(sLoc + 5, 13, "<"); }
	else if (mainSelect == EXITMENU) { ScreenRender(sLoc - 2, 14, ">"); ScreenRender(sLoc + 5, 14, "<"); }

	//ȭ�� ��ü
	if (noswitch == 0) ScreenSwitch();

}
void cliRhythm::RenderSongsUI(int noswitch) {
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
	for (int i = 0; i < SboxRD.Y - SboxLT.Y - 1; i++) {
		if (i + 1 > songList.size()) break;
		ScreenRender(SboxLT.X + 1, SboxLT.Y + 1 + i, (char*)songList[songBoxFirst + i].c_str());
	}

	//����� ���õǾ����� Ȯ�ο�
	ScreenRender(1, LINES - 2, (char*)songList[songSelect].c_str());


	if (noswitch == 0) ScreenSwitch();
}
void cliRhythm::RenderInGameUI(char* songName, int noswitch) {
	ScreenClear(0, 0, COLS, LINES);
	
	// �׵θ� �׸�
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, 0, "-");
	for (int i = 1; i < LINES - 1; i++) { ScreenRender(0, i, "|"); ScreenRender(COLS - 1, i, "|"); }
	for (int i = 1; i < COLS - 1; i++) ScreenRender(i, LINES - 1, "-");

	// Ÿ��Ʋ �׸�
	ScreenRender(1, 1, songName);

	// ����ȭ�� �׸�
	for (int i = GboxLT.Y; i < GboxRD.Y; i++) { ScreenRender(GboxLT.X - 1, i, "��"); ScreenRender(COLS - GboxLT.X-1, i, "��"); }
	for (int i = GboxLT.Y; i < GboxRD.Y; i++) { 
		for (int j = 1; j <= 3; j++) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * j, i, "|");
	}
	for (int i = GboxLT.X + 1; i < GboxRD.X - 1; i++) ScreenRender(i, GboxRD.Y - 2, "-");
	for (int i = GboxLT.X + 1; i < GboxRD.X - 1; i++) ScreenRender(i, GboxRD.Y, "-");


	ScreenRender(1, 2, score);

	if (noswitch == 0) ScreenSwitch();

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
int cliRhythm::SelectStart() {// �� ���� ȭ��
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
				//�������� ���͵� �ݾƾ����� ����
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
		//ä������ ����
		Gamefile.open("RhythmData\\" + songList[songSelect] + "\\" + songList[songSelect] + ".txt");
		//ä������ ������¿��� = ���翩�� Ȯ��
		RenderInGameUI(songName, 1);
		if (Gamefile.is_open()) {ScreenRender(statusX, statusY, "ä���ε��Ϸ� "); ScreenSwitch();}
		else { throw 1; }
		//�뷡���� �ִ��� Ȯ���ϰ� ������ 2������..
		
	}
	catch (int ex) {
		if (ex == 1) {
			//������ �������� ���� ���
			for (int i = 3; i > 0; i--) {
				char temp[50]=""; sprintf(temp, "���� ȭ������ ���ư��ϴ� %d", i);
				ScreenRender(statusX, statusY - 1, "ä���ε����� - ���Ͼ���"); ScreenRender(statusX, statusY, temp);
				ScreenSwitch(); Sleep(1000);
			}
			return(1);
		}
		if (ex == 2) {
			for (int i = 3; i > 0; i--) {
				char temp[50] = ""; sprintf(temp, "���� ȭ������ ���ư��ϴ� %d", i);
				ScreenRender(statusX, statusY - 1, "���Ƿε����� - ���Ͼ���"); ScreenRender(statusX, statusY, temp);
				ScreenSwitch(); Sleep(1000);
			}
			return(1);
		}
		else { return(1); }
	}
	
	//���� �ʱ�ȭ
	for (int y = 0; y < 38; y++) {
		for (int x = 0; x < 4; x++) Gbuffer[y][x] = 0;
	}

	//ī��Ʈ�ٿ�
	for (int i = 0; i < 3; i++) {
		char* list[] = { "'1'","'2'","'3'"};
		ScreenRender(COLS / 2 - 1, LINES / 2 + 1, "   ");
		ScreenRender(COLS / 2 - 1, LINES / 2 - 1, "   ");
		ScreenRender(COLS / 2 - 1, LINES / 2, list[2 - i]); ScreenSwitch(); Sleep(1000);
	}
	RenderInGameUI(songName); RenderInGameUI(songName);

	//txt������ start���� Ŀ���̵�
	char line[10] = "";
	while (strcmp(line,"start\0")!=0 ) {
		Gamefile.getline(line, 8);
	}

	//������� ����. Ŭ���� ������ �޼ҵ�� ������ ���� �� ����
	//�׳� c���ó�� �������� �߾�����..

	// �뷡 Ʋ�� - ���� �ȵǰ�����
	//try {
	//	char SongFile[100] = "";
	//	//sprintf(SongFile, "RhythmData\\%s\\%s.wav", songList[songSelect], songList[songSelect]);
	//	PlaySound(L"RhythmData\\ssibal\\ssibal.wav", NULL, SND_FILENAME | SND_ASYNC);
	//	//ScreenRender(0, 0, "�뷡 ����"); ScreenSwitch();
	//}
	//catch (exception ex) {
	//	char* why = (char*)ex.what();
	//	ScreenRender(0, 0, why); ScreenSwitch();
	//	Sleep(3000); return(1);
	//}

	while (1) {
		//Ŭ������
		ScreenClear(GboxLT.X + 1, GboxRD.Y - 1, GboxRD.X - 1, GboxRD.Y - 1);
		for (int j = 1; j <= 3; j++) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * j, GboxRD.Y - 1, "|");
		//��Ʈ����
		ScreenClear(GboxLT.X + 1, 1, GboxRD.X - 1, GboxRD.Y - 3);
		
		//�������� ������. ������ �Ʒ��� �����⸸ �ϸ��
		for (int y = GboxRD.Y - 3; y >= GboxLT.Y; y--) {
			//�� ���� ���� �׳� �������� ������ ������
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
		//ù°�� ������
		// start �Ʒ����� ���� �Է¹ް� ������ ������ �����Ŷ�� �׳��н�
		try {
			Gamefile.getline(line, 8);
			if (Gamefile.eof()) EoF = 1;
		}
		catch (exception) {}

		//�ִ� 4��Ʈ���� �Է¹��� �� �����Ƿ� 0,2,4,6 �ε����� �� �˻�

		for (int x = 0; x < 4; x++) { 
			if (line[0] == '\0') break;
			//�� ������ �ƴѰ��
			if (EoF != 1) LastNote = 0;
			int where = line[2 * x] - '1'; Gbuffer[0][where] = 1;
			//ScreenRender(GboxLT.X + 1 + (GboxRD.X - GboxLT.X) / 4 * where, GboxLT.Y, "#########");
			//�� �̻� �ߺ��� ��Ʈ�� ���� ��� ����
			if (line[2 * x + 1] == '\0') {/* for (int t = x+1; t < 4; t++) Gbuffer[0][t] = 0;*/ break; }
		}

		//������ ���μ��� �ٽ� �����
		for (int i = GboxLT.Y; i < GboxRD.Y - 2; i++) {
			for (int j = 1; j <= 3; j++) ScreenRender(GboxLT.X + (GboxRD.X - GboxLT.X) / 4 * j, i, "|");
		}

		PushKey();
		ScreenSwitch();

		Sleep(16);

	}
}