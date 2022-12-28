#pragma once
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <thread>
using namespace std;
namespace fs = std::filesystem;

#define STARTMENU 100
#define EXITMENU 200

#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77

#define COLS 81
#define LINES 47

class cliRhythm {
public://생성자
	cliRhythm();
	~cliRhythm();
	cliRhythm(string username);
public:
	void Start();
	int SelectStart();
	int InGameStart();
public:
	void ScreenInit();
	void ScreenRender(int x, int y, char* text);
	void ScreenSwitch();
	void ScreenClear(int x, int y, int x2, int y2);
	void RenderMainUI();
	void RenderSongsUI();
	void RenderInGameUI();
	
private:
	HANDLE screen[2];
	CONSOLE_CURSOR_INFO CURSOR;
	int _whatScreen = 0;
	char bufs[COLS];
	COORD SboxLT = { 10,10 };
	COORD SboxRD = { COLS - 10,LINES - 5 };

private:
	string _username = "default";
	int mainSelect = STARTMENU; // 시작은 0, 종료는 1
	int songSelect = 0;
	int songBoxFirst = 0;
	int songBoxArrow = 0;
	ofstream Outfile;
	ofstream Infile;
	ofstream Gamefile;
	vector<string> songList;

};
