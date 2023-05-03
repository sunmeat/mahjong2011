#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <io.h> // функции для поиска файлов
using namespace std;

HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
COORD c;

void text(char* mes, int sec = 0) {
	int len = strlen(mes);
	char* str = new char[len + 1];
	strcpy(str, mes);
	AnsiToOem(str, str);
	for (int i = 0; str[i] != '\0'; i++) {
		cout << str[i];
		Sleep(sec);
	}
	delete[]str;
}

void Window(int opx, int opy, int owi, int ohe, int oco) {
	int** optr = new int* [owi];
	for (int i = 0; i < owi; i++) optr[i] = new int[ohe];
	c.Y = opy;
	for (int j = 0; j < ohe; j++) {
		c.X = opx;
		for (int i = 0; i < owi; i++) {
			if (i == 0 && j == 0) optr[i][j] = 201;
			else if (i == owi - 1 && j == 0) optr[i][j] = 187;
			else if (i == 0 && j == ohe - 1) optr[i][j] = 200;
			else if (i == owi - 1 && j == ohe - 1) optr[i][j] = 188;
			else if ((j == 0 || j == ohe - 1) && i != 0 && i != owi - 1 || j == ohe - 1 && i != 0 && i != owi - 1) optr[i][j] = 205;
			else if ((i == 0 || i == owi - 1) && j != 0 && j != ohe - 1) optr[i][j] = 186;
			else optr[i][j] = 0;
			SetConsoleCursorPosition(H, c);
			cout << (char)optr[i][j];
			//FillConsoleOutputAttribute(H,oco,1,c,0);
			c.X++;
		}
		c.Y++;
	}
}

void Window_OL(int opx, int opy, int owi, int ohe, int oco) {
	int** optr = new int* [owi];
	for (int i = 0; i < owi; i++) optr[i] = new int[ohe];
	c.Y = opy;
	for (int j = 0; j < ohe; j++) {
		c.X = opx;
		for (int i = 0; i < owi; i++) {
			if (i == 0 && j == 0) optr[i][j] = 218;
			else if (i == owi - 1 && j == 0) optr[i][j] = 191;
			else if (i == 0 && j == ohe - 1) optr[i][j] = 192;
			else if (i == owi - 1 && j == ohe - 1) optr[i][j] = 217;
			else if ((j == 0 || j == ohe - 1) && i != 0 && i != owi - 1 || j == ohe - 1 && i != 0 && i != owi - 1) optr[i][j] = 196;
			else if ((i == 0 || i == owi - 1) && j != 0 && j != ohe - 1) optr[i][j] = 179;
			else optr[i][j] = 0;
			SetConsoleCursorPosition(H, c);
			cout << (char)optr[i][j];
			//FillConsoleOutputAttribute(H,oco,1,c,0);
			c.X++;
		}
		c.Y++;
	}
}

void Window_CL(int opx, int opy, int owi, int ohe, int oco) {
	int** optr = new int* [owi];
	for (int i = 0; i < owi; i++) optr[i] = new int[ohe];
	c.Y = opy;
	for (int j = 0; j < ohe; j++) {
		c.X = opx;
		for (int i = 0; i < owi; i++) {
			if (i == 0 && j == 0) optr[i][j] = 0;
			else if (i == owi - 1 && j == 0) optr[i][j] = 0;
			else if (i == 0 && j == ohe - 1) optr[i][j] = 0;
			else if (i == owi - 1 && j == ohe - 1) optr[i][j] = 0;
			else if ((j == 0 || j == ohe - 1) && i != 0 && i != owi - 1 || j == ohe - 1 && i != 0 && i != owi - 1) optr[i][j] = 0;
			else if ((i == 0 || i == owi - 1) && j != 0 && j != ohe - 1) optr[i][j] = 0;
			else optr[i][j] = 0;
			SetConsoleCursorPosition(H, c);
			cout << (char)optr[i][j];
			//FillConsoleOutputAttribute(H,oco,1,c,0);
			c.X++;
		}
		c.Y++;
	}
}

void Stand(int x, int y, int color) {
	c.X = x; c.Y = y;
	SetConsoleCursorPosition(H, c);
	SetConsoleTextAttribute(H, color);
}

int Razer(int digit) {
	int k = 1;
	while (digit / 10) {
		digit /= 10;
		k++;
	}
	return k;
}

class Wind { // создание и показ кнопок меню - намного удобнее, чем многократный вызов аналогичных функций
public:
	int xpoz, ypoz;
	int widz, heits;
	char* mess;
	int linkw, hoverw;
	int linkt, hovert;
	char** pArr;
	Wind(char* _mess, int _xpoz = 0, int _ypoz = 0, int _linkw = 1, int _hoverw = 7, int _linkt = 9, int _hovert = 15) {
		xpoz = _xpoz;
		ypoz = _ypoz;
		widz = strlen(_mess) + 4;
		heits = 3;
		mess = new char[strlen(_mess)]; strcpy(mess, _mess);
		linkw = _linkw;
		hoverw = _hoverw;
		linkt = _linkt;
		hovert = _hovert;
		pArr = new char* [3];
		for (int i = 0; i < 3; i++) pArr[i] = new char[widz];
		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < widz; i++) {
				if (i == 0 && j == 0) pArr[j][i] = (char)201;
				else if (i == widz - 1 && j == 0) pArr[j][i] = (char)187;
				else if (i == 0 && j == 3 - 1) pArr[j][i] = (char)200;
				else if (i == widz - 1 && j == 3 - 1) pArr[j][i] = (char)188;
				else if ((j == 0 || j == 3 - 1) && i != 0 && i != widz - 1 || j == 3 - 1 && i != 0 && i != widz - 1) pArr[j][i] = (char)205;
				else if ((i == 0 || i == widz - 1) && j != 0 && j != 3 - 1) pArr[j][i] = (char)186;
				else pArr[j][i] = (char)0;
			}
		}
		AnsiToOem(mess, mess);
		for (int i = 0; i < strlen(mess); i++) pArr[1][2 + i] = mess[i];
		OemToAnsi(mess, mess);
	}
	~Wind() {
		for (int i = 0; i < 3; i++) delete[]pArr[i];
		delete[]pArr;
		delete[]mess;
	}
	void SW(int mode = 1) {
		if (mode) {
			Window(xpoz, ypoz, widz, heits, linkw);
			Stand(xpoz + 2, ypoz + 1, linkt);
			text(mess);
		}
		else {
			Window(xpoz, ypoz, widz, heits, hoverw);
			Stand(xpoz + 2, ypoz + 1, hovert);
			text(mess);
		}
	}
	void Right(int xp, int yp, int way, int pause) {
		for (int w = 0; w < way; w++) {
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < widz; i++) {
					if (i + xp + w >= 0 && i + xp + w < 80) {
						Stand(i + xp + w, yp + j, 0);
						if (i == 0 || i == widz - 1 || j == 0 || j == 2) SetConsoleTextAttribute(H, linkw);
						else SetConsoleTextAttribute(H, linkt);
						cout << pArr[j][i];
					}
				}
				if (w) { Stand(xp + w - 1, yp + j, 7); cout << " "; }
			}
			Sleep(pause);
		}
	}
	void Left(int xp, int yp, int way, int pause) {
		for (int w = 0; w < way; w++) {
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < widz; i++) {
					if (i + xp - w >= 0 && i + xp - w < 80) {
						Stand(i + xp - w, yp + j, 0);
						if (i == 0 || i == widz - 1 || j == 0 || j == 2) SetConsoleTextAttribute(H, linkw);
						else SetConsoleTextAttribute(H, linkt);
						cout << pArr[j][i];
					}
				}
				if (w) { Stand(xp + widz - w, yp + j, 7); cout << " "; }
			}
			Sleep(pause);
		}
	}
};