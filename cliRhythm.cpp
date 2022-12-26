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
	system("title cli_�������_0.0.1v");

	//���� ���⼭ �� �̸��� ã��.. ���߿� ������ �ҷ����ų� �ؾ��Ѵٸ� ���� �����ľ��ҵ� 
	Outfile.open("RhythmData\\Datas.txt",std::ios::trunc | std::ios::out);
	if (Outfile.fail()) {
		cerr << "Error!" << endl;
	}
	Infile.open("RhythmData\\Datas.txt", std::ios::in);
	if (Infile.fail()) {
		cerr << "Error!" << endl;
	}
	
	//���丮 �� ��������(*.txt) ����� �ҷ���
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

void cliRhythm::Start() {//Main ȭ��
	int key = 0;
	PaintMainUI();
	// ������ ȭ�鿡�� ������ Ű�Է��� ��ٸ� - kbhit()�� ����� �ʿ� ���ٰ� �Ǵ�
	while (1) {
		key = _getch();

		if (key == 13) { // ����
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
			case UPKEY: // ��
				if (mainSelect == STARTMENU) break;
				mainSelect = STARTMENU;
				PaintMainUI();
				break;
			case DOWNKEY: // �Ʒ�
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

	std::wcout << L"��" << setw(COLS - 2) << setfill(L'��') << L"��" << endl; std::wcout << setfill(L' ');

	for (int i = 0; i < 5; i++) std::wcout <<  L"��" << setw(COLS - 2) << L"��" << endl;
	
	std::wcout << L"�� " << setw((COLS - 3) / 2 + strlen("RhythmGame") / 2 + 1);
	std::wcout << L"RhythmGame" << setw((COLS - 3) / 2 - strlen("RhythmGame") / 2) << L"��" << endl;

	for (int i = 0; i < 5; i++) std::wcout << L"��" << setw(COLS - 2) << L"��" << endl;

	if (mainSelect == STARTMENU) {
		std::wcout << L"�� " << setw((COLS - 3) / 2 + strlen("> ����") / 2 - 3);
		std::wcout << L"> ����" << setw((COLS - 3) / 2 - strlen("> ����") / 2 + 2) << L"��" << endl;
		std::wcout << L"�� " << setw((COLS - 3) / 2 + strlen("  ����") / 2 - 3);
		std::wcout << L"  ����" << setw((COLS - 3) / 2 - strlen("  ����") / 2 + 2) << L"��" << endl;
	}
	else {
		std::wcout << L"�� " << setw((COLS - 3) / 2 + strlen("  ����") / 2 - 3);
		std::wcout << L"  ����" << setw((COLS - 3) / 2 - strlen("  ����") / 2 + 2) << L"��" << endl;
		std::wcout << L"�� " << setw((COLS - 3) / 2 + strlen("> ����") / 2 - 3);
		std::wcout << L"> ����" << setw((COLS - 3) / 2 - strlen("> ����") / 2 + 2) << L"��" << endl;
	}

	for (int i = 0; i < 30 ; i++) std::wcout << L"��" << setw(COLS - 2) << L"��" << endl;
	std::wcout << L"��" << setw(COLS - 2) << setfill(L'��') << L"��" << endl; std::wcout << setfill(L' ');
}

void cliRhythm::PaintSongsUI() {
	system("cls");

	std::wcout << L"��" << setw(COLS - 2) << setfill(L'��') << L"��" << endl; std::wcout << setfill(L' ');

	for (int i = 0; i < 5; i++) std::wcout << L"��" << setw(COLS - 2) << L"��" << endl;

	std::wcout << L"�� " << setw((COLS - 3) / 2 + strlen("Select Song") / 2 + 1);
	std::wcout << L"Select Song" << setw((COLS - 3) / 2 - strlen("Select Song") / 2) << L"��" << endl;

	for (int i = 0; i < 5; i++) std::wcout << L"��" << setw(COLS - 2) << L"��" << endl;

	

	
}
