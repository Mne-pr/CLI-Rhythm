#pragma once
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>
#include <sstream>
using namespace std;
namespace fs = std::filesystem;

#define STARTMENU 100
#define EXITMENU 200

#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77

#define COLS 80
#define LINES 47

class cliRhythm {
public:
	cliRhythm();
	~cliRhythm();
	cliRhythm(string username);

	void Start();
	int SelectStart(int key);
	void PaintMainUI();
	void PaintSongsUI();
	
	
private:
	string _username = "default";
	int mainSelect = STARTMENU; // 시작은 0, 종료는 1
	ofstream Outfile;
	ofstream Infile;
	vector<string> songList;
};
