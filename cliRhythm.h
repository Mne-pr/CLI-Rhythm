#pragma once
#include <iostream>// 기본
#include <stdio.h>
#include <cstdio>
#include <iomanip>
#include <conio.h>
#include <Windows.h>// system, Sleep
#include <fstream>// file
#include <vector>// vector
#include <filesystem>// all files at directory
#include <string>// substr
#include <sstream> 
#include <stdarg.h>
#include <math.h>

using namespace std;
namespace fs = std::filesystem;

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define STARTMENU 100
#define EXITMENU 200

#define OVERASCII 224
#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77
#define EXITKEY1 81
#define EXITKEY2 113
#define ENTERKEY 13

#define DKEY 68
#define SDKEY 100
#define FKEY 70
#define SFKEY 102
#define JKEY 74
#define SJKEY 106
#define KKEY 75
#define SKKEY 107

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
	void ScreenRender(int x, int y, char ch);
	void ScreenSwitch();
	void ScreenClear(int x, int y, int x2, int y2);
public:
	void RenderMainUI(int noswitch = 0);
	void RenderSongsUI(int noswitch = 0);
	void RenderInGameUI(char* songName, int noswitch = 0);
public:
	void PushKey();
	void Pan(int key);

private:
	HANDLE screen[2];
	CONSOLE_CURSOR_INFO CURSOR;
	int _whatScreen = 0;
	char bufs[COLS];
	COORD SboxLT = { 10,10 };
	COORD SboxRD = { COLS - SboxLT.X,LINES - SboxLT.Y };
	COORD GboxLT = { 20,1 };
	COORD GboxRD = { COLS - GboxLT.X,LINES - 3 };
	int DKey = 0, FKey = 0, JKey = 0, KKey = 0;
	int LastNote = 0;
	int EoF = 0;

private:
	string _username = "default";// 아직 쓸모없음
	// 곡 목록 출력
	int mainSelect = STARTMENU; // 시작은 0, 종료는 1
	int songSelect = 0;
	int songBoxFirst = 0;
	int songBoxArrow = 0;
	// 구동에 필요한 파일 관련
	ofstream Outfile;
	ofstream Infile;
	ifstream Gamefile;
	vector<string> songList;
	// 판정 관련
	char* score = "0";
	int Perfect = 0;
	int Good = 0;
	int Miss = 0;
	int Gbuffer[38][4];

};
