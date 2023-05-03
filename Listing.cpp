#include "rustext.h"
//////////////////////////////////////////////////////////////////////////
// ПРОТОТИПЫ
//////////////////////////////////////////////////////////////////////////
void Main_Menu();
void Menu_Choice();
void main();
void Edditor();
//////////////////////////////////////////////////////////////////////////
// ФЛАГИ
//////////////////////////////////////////////////////////////////////////
bool menu_flags[6]; // наведён курсор на кнопку или нет
bool menu_activ[5]; // например, затенены уже фишки или ещё нет, и др.
int press; // нажали на фишку или нет
int prices[3] = { 750,500,250 }; // стоимость перетасовки, подсказки, отмены хода
int sl = 500, sl2 = 20, sl3 = 15;
char* global_address;
//////////////////////////////////////////////////////////////////////////
// ВСТРОЕННЫЙ РЕДАКТОР 
//////////////////////////////////////////////////////////////////////////
bool flags[4]; // наведён курсор на кнопки меню или нет
int glx, gly;  // запоминается положение курсора над фишками, для того, чтобы можно было правильно добавить фишку
bool first;    // вначале на поле нет фишек, используется для появления курсора в виде фишки
bool sorry_for_memory[10][50][80]; // просто неудобно высчитывать, где есть фишка, а где нет, приходится использовать такой большой массив
struct Layout { // координаты фишки
	int _x, _y, _z;
};
class Edit { // класс самого редактора, включены функции показа 'открываемых кусков' фишек, проверка на наличие фишки в определённом месте, получение номера слоя, добавление фишек, сохранение, отмена, новая раскладка
	char* ln; // название раскладки
	int lcount; // количество фишек
	Layout lm[840]; // массив координат фишек
public:
	~Edit() {
		delete[]ln;
	}
	Edit() {
		lcount = 0;
	}
	void Piece_Of_Fishka(int quart, int xx, int yy);
	void Edit_Presence(int x, int y);
	void Edit_Add(int x, int y);
	void Get_Level(int x, int y);
	void TSave();
	void TCancel();
	void TNew();
	void ex() {
		for (int i = 0; i < lcount; i++) lm[i]._x = lm[i]._y = lm[i]._z = 0;
		lcount = 0;
		for (int z = 9; z >= 0; z--)
			for (int j = 0; j < 50; j++)
				for (int i = 0; i < 80; i++)
					sorry_for_memory[z][j][i] = 0;
	}
};
void Edit::Piece_Of_Fishka(int quart, int xx, int yy) {
	if (quart == 1) {
		Stand(xx, yy, 14); cout << (char)218 << (char)196 << (char)196;
		Stand(xx, yy + 1, 14); cout << (char)179 << "  ";
		Stand(xx, yy + 2, 14); cout << (char)179 << "  ";
	}
	else if (quart == 2) {
		Stand(xx + 3, yy, 14); cout << (char)196 << (char)196 << (char)191;
		Stand(xx + 3, yy + 1, 14); cout << "  " << (char)179;
		Stand(xx + 3, yy + 2, 14); cout << "  " << (char)179;
	}
	else if (quart == 3) {
		Stand(xx, yy + 3, 14); cout << (char)179 << "  ";
		Stand(xx, yy + 4, 14); cout << (char)179 << "  ";
		Stand(xx, yy + 5, 14); cout << (char)192 << (char)196 << (char)196;
	}
	else if (quart == 4) {
		Stand(xx + 3, yy + 3, 14); cout << "  " << (char)179;
		Stand(xx + 3, yy + 4, 14); cout << "  " << (char)179;
		Stand(xx + 3, yy + 5, 14); cout << (char)196 << (char)196 << (char)217;
	}
}
void Edit::Edit_Presence(int x, int y) {
	if (glx == x && gly == y) return;
	bool password = 0;
	if (glx == x && gly == y - 3 || glx == x && gly == y + 3 || glx == x - 3 && gly == y || glx == x + 3 && gly == y || first == 0) password = 1;
	if (password == 0) {
		Stand(glx, gly, 8); cout << (char)250 << (char)250 << (char)250;
		Stand(glx, gly + 1, 8); cout << (char)250 << (char)250 << (char)250;
		Stand(glx, gly + 2, 8); cout << (char)250 << (char)250 << (char)250;
		for (int i = 0; i < lcount; i++) {
			for (int z = 1; z < 11; z++) {
				if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly - 3) {
					Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly - 3) {
					Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly) {
					Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly) {
					Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
				}
			}
		}
		Stand(glx + 3, gly, 8); cout << (char)250 << (char)250 << (char)250;
		Stand(glx + 3, gly + 1, 8); cout << (char)250 << (char)250 << (char)250;
		Stand(glx + 3, gly + 2, 8); cout << (char)250 << (char)250 << (char)250;
		for (int i = 0; i < lcount; i++) {
			for (int z = 1; z < 11; z++) {
				if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly - 3) {
					Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly - 3) {
					Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly) {
					Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly) {
					Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
				}
			}
		}
		Stand(glx, gly + 3, 8); cout << (char)250 << (char)250 << (char)250;
		Stand(glx, gly + 4, 8); cout << (char)250 << (char)250 << (char)250;
		Stand(glx, gly + 5, 8); cout << (char)250 << (char)250 << (char)250;
		for (int i = 0; i < lcount; i++) {
			for (int z = 1; z < 11; z++) {
				if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly) {
					Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly) {
					Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly + 3) {
					Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly + 3) {
					Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
				}
			}
		}
		Stand(glx + 3, gly + 3, 8); cout << (char)250 << (char)250 << (char)250;
		Stand(glx + 3, gly + 4, 8); cout << (char)250 << (char)250 << (char)250;
		Stand(glx + 3, gly + 5, 8); cout << (char)250 << (char)250 << (char)250;
		for (int i = 0; i < lcount; i++) {
			for (int z = 1; z < 11; z++) {
				if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly) {
					Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly) {
					Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly + 3) {
					Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
				}
				else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly + 3) {
					Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
				}
			}
		}
		glx = x; gly = y;
		return;
	}
	else {
		int color;
		bool flag = 1;
		for (int z = 0; z < 10 && flag; z++) {
			if (z == 0 && sorry_for_memory[z][y][x] == 0 || sorry_for_memory[z - 1][y][x - 3] == 1 && sorry_for_memory[z - 1][y][x + 3] == 1 || sorry_for_memory[z - 1][y - 3][x] == 1 && sorry_for_memory[z - 1][y + 3][x] == 1 || sorry_for_memory[z - 1][y][x] == 1 || sorry_for_memory[z - 1][y - 3][x - 3] == 1 && sorry_for_memory[z - 1][y - 3][x + 3] == 1 && sorry_for_memory[z - 1][y + 3][x - 3] == 1 && sorry_for_memory[z - 1][y + 3][x + 3] == 1 || sorry_for_memory[z - 1][y - 3][x - 3] == 1 && sorry_for_memory[z - 1][y - 3][x + 3] == 1 && sorry_for_memory[z - 1][y + 3][x] == 1 || sorry_for_memory[z - 1][y - 3][x] == 1 && sorry_for_memory[z - 1][y + 3][x - 3] == 1 && sorry_for_memory[z - 1][y + 3][x + 3] == 1 || sorry_for_memory[z - 1][y - 3][x - 3] == 1 && sorry_for_memory[z - 1][y + 3][x - 3] == 1 && sorry_for_memory[z - 1][y][x + 3] == 1 || sorry_for_memory[z - 1][y][x - 3] == 1 && sorry_for_memory[z - 1][y - 3][x + 3] == 1 && sorry_for_memory[z - 1][y + 3][x + 3] == 1) {
				if (sorry_for_memory[z][y - 3][x - 3] == 0 && sorry_for_memory[z][y - 3][x] == 0 && sorry_for_memory[z][y - 3][x + 3] == 0 && sorry_for_memory[z][y][x - 3] == 0 && sorry_for_memory[z][y][x] == 0 && sorry_for_memory[z][y][x + 3] == 0 && sorry_for_memory[z][y + 3][x - 3] == 0 && sorry_for_memory[z][y + 3][x] == 0 && sorry_for_memory[z][y + 3][x + 3] == 0) {
					color = 10; flag = 0; break;
				}
			}
		}
		if (flag == 1) color = 12;
		Window_OL(x, y, 6, 6, color);
		first = 1;
		if (glx == x && gly == y + 3) {
			Stand(x, y + 6, 8); cout << (char)250 << (char)250 << (char)250 << (char)250 << (char)250 << (char)250;
			Stand(x, y + 7, 8); cout << (char)250 << (char)250 << (char)250 << (char)250 << (char)250 << (char)250;
			Stand(x, y + 8, 8); cout << (char)250 << (char)250 << (char)250 << (char)250 << (char)250 << (char)250;
			for (int i = 0; i < lcount; i++) {
				for (int z = 1; z < 11; z++) {
					if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly) {
						Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
						Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly + 3) {
						Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
						Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
					}

					else if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly) {
						Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly + 3) {
						Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
					}

					else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly) {
						Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly + 3) {
						Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
					}
				}
			}
		}
		else if (glx == x && gly == y - 3) {
			Stand(x, y - 3, 8); cout << (char)250 << (char)250 << (char)250 << (char)250 << (char)250 << (char)250;
			Stand(x, y - 2, 8); cout << (char)250 << (char)250 << (char)250 << (char)250 << (char)250 << (char)250;
			Stand(x, y - 1, 8); cout << (char)250 << (char)250 << (char)250 << (char)250 << (char)250 << (char)250;
			for (int i = 0; i < lcount; i++) {
				for (int z = 1; z < 11; z++) {
					if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly) {
						Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
						Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly - 3) {
						Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
						Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
					}

					else if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly - 3) {
						Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly) {
						Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
					}

					else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly - 3) {
						Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly) {
						Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
					}
				}
			}
		}
		else if (glx == x + 3 && gly == y) {
			Stand(x + 6, y, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x + 6, y + 1, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x + 6, y + 2, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x + 6, y + 3, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x + 6, y + 4, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x + 6, y + 5, 8); cout << (char)250 << (char)250 << (char)250;
			for (int i = 0; i < lcount; i++) {
				for (int z = 1; z < 11; z++) {
					if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly) {
						Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
						Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly) {
						Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
						Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
					}

					else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly - 3) {
						Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly - 3) {
						Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
					}

					else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly + 3) {
						Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx + 3 && lm[i]._y == gly + 3) {
						Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
					}
				}
			}
		}
		else if (glx == x - 3 && gly == y) {
			Stand(x - 3, y, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x - 3, y + 1, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x - 3, y + 2, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x - 3, y + 3, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x - 3, y + 4, 8); cout << (char)250 << (char)250 << (char)250;
			Stand(x - 3, y + 5, 8); cout << (char)250 << (char)250 << (char)250;
			for (int i = 0; i < lcount; i++) {
				for (int z = 1; z < 11; z++) {
					if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly) {
						Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
						Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly) {
						Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
						Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
					}

					else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly + 3) {
						Piece_Of_Fishka(1, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly + 3) {
						Piece_Of_Fishka(2, lm[i]._x, lm[i]._y);
					}

					else if (lm[i]._z == z && lm[i]._x == glx && lm[i]._y == gly - 3) {
						Piece_Of_Fishka(3, lm[i]._x, lm[i]._y);
					}
					else if (lm[i]._z == z && lm[i]._x == glx - 3 && lm[i]._y == gly - 3) {
						Piece_Of_Fishka(4, lm[i]._x, lm[i]._y);
					}
				}
			}
		}
		glx = x; gly = y;
	}
}
void Edit::Edit_Add(int x, int y) {
	bool flag = 1;
	for (int z = 9; z >= 0 && flag; z--) {
		if (z == 0 && sorry_for_memory[z][y][x] == 0 || sorry_for_memory[z - 1][y][x - 3] == 1 && sorry_for_memory[z - 1][y][x + 3] == 1 || sorry_for_memory[z - 1][y - 3][x] == 1 && sorry_for_memory[z - 1][y + 3][x] == 1 || sorry_for_memory[z - 1][y][x] == 1 || sorry_for_memory[z - 1][y - 3][x - 3] == 1 && sorry_for_memory[z - 1][y - 3][x + 3] == 1 && sorry_for_memory[z - 1][y + 3][x - 3] == 1 && sorry_for_memory[z - 1][y + 3][x + 3] == 1 || sorry_for_memory[z - 1][y - 3][x - 3] == 1 && sorry_for_memory[z - 1][y - 3][x + 3] == 1 && sorry_for_memory[z - 1][y + 3][x] == 1 || sorry_for_memory[z - 1][y - 3][x] == 1 && sorry_for_memory[z - 1][y + 3][x - 3] == 1 && sorry_for_memory[z - 1][y + 3][x + 3] == 1 || sorry_for_memory[z - 1][y - 3][x - 3] == 1 && sorry_for_memory[z - 1][y + 3][x - 3] == 1 && sorry_for_memory[z - 1][y][x + 3] == 1 || sorry_for_memory[z - 1][y][x - 3] == 1 && sorry_for_memory[z - 1][y - 3][x + 3] == 1 && sorry_for_memory[z - 1][y + 3][x + 3] == 1) {
			if (sorry_for_memory[z][y - 3][x - 3] == 0 && sorry_for_memory[z][y - 3][x] == 0 && sorry_for_memory[z][y - 3][x + 3] == 0 && sorry_for_memory[z][y][x - 3] == 0 && sorry_for_memory[z][y][x] == 0 && sorry_for_memory[z][y][x + 3] == 0 && sorry_for_memory[z][y + 3][x - 3] == 0 && sorry_for_memory[z][y + 3][x] == 0 && sorry_for_memory[z][y + 3][x + 3] == 0) {
				sorry_for_memory[z][y][x] = 1;
				Window_OL(x, y, 6, 6, 14);
				flag = 0;
				lm[lcount]._x = x;
				lm[lcount]._y = y;
				lm[lcount]._z = z + 1;
				lcount++;
				break;
			}
		}
	}
}
void Edit::Get_Level(int x, int y) {
	for (int z = 9; z >= 0; z--) {
		for (int j = 1; j < 43; j += 3) {
			for (int i = 4; i < 76; i += 3) {
				if (i == x && j == y && sorry_for_memory[z][j][i] == 1) {
					z++;
					Stand(46, 47, 15);
					if (z < 10) cout << ' '; cout << z;
					return;
				}
			}
		}
	}
	Stand(46, 47, 7); cout << "  ";
}
void Edit::TSave() {
	if (lcount < 20 || lcount % 2 == 1) return;
	else {
		system("cls");
		Stand(1, 1, 12); text("Раскладка создана. Количество фишек - ", 15); cout << lcount;
		Stand(1, 3, 12); text("Укажите название раскладки: ", 15);
		ln = new char[20];
		cin.getline(ln, 20);
		Stand(1, 5, 12); text("Укажите название файла: ", 15);
		char* fn = new char[20];
		cin.getline(fn, 20);
		Stand(1, 7, 12); text("Cохранение...", 15);
		char* address = new char[40];
		OemToAnsi(fn, fn);
		strcpy(address, "User_Maps\\"); strcat(address, fn);
		FILE* f = fopen(address, "w+");
		OemToAnsi(ln, ln);
		fputs(ln, f); fputc('\n', f);
		fprintf(f, "%d %c", lcount, ' ');
		for (int i = 0; i < lcount; i++) fprintf(f, "%d %d %d %c", lm[i]._x, lm[i]._y, lm[i]._z, ' ');
		fclose(f);
		Stand(1, 9, 12); text("Готово!", 15);
		getch();
		system("cls"); Main_Menu();
	}
}
void Edit::TCancel() {
	if (lcount == 0) return;
	else {
		lcount--;
		for (int z = 9; z >= 0; z--) {
			for (int j = 1; j < 43; j += 3) {
				for (int i = 4; i < 76; i += 3) {
					if (i == lm[lcount]._x && j == lm[lcount]._y && z + 1 == lm[lcount]._z) {
						sorry_for_memory[z][j][i] = 0;
						Edit_Presence(lm[lcount]._x, lm[lcount]._y);
						lm[lcount]._x = lm[lcount]._y = lm[lcount]._z = 0;
					}
				}
			}
		}
	}
}

void Edit::TNew() {
	ex();
	Edditor();
}
void Edditor() {
	// интерфейс редактора
	for (int j = 1; j < 43; j++) {
		for (int i = 4; i < 76; i++) {
			Stand(i, j, 8); cout << (char)250;
		}
	}
	int xp = 2;
	Stand(0, 44, 1); for (int i = 0; i < 80; i++) cout << (char)205;
	Wind Save("СОХРАНИТЬ", xp, 46, 2, 7, 10, 15); Save.SW();
	Wind Cancel("ОТМЕНИТЬ", xp + 14, 46, 3, 7, 11, 15); Cancel.SW();
	Wind Level("УРОВЕНЬ ФИШКИ:   ", xp + 27, 46, 4, 7, 12, 15); Level.SW();
	Wind New("СОЗДАТЬ НОВУЮ", xp + 49, 46, 5, 7, 13, 15); New.SW();
	Wind Exit("ВЫЙТИ", xp + 67, 46, 6, 7, 14, 15); Exit.SW();
	//////////////////////////////////////////////////////////////////////////
	Edit Test; // сразу доступна новая раскладка
	//////////////////////////////////////////////////////////////////////////
	DWORD fdwMode, cNumRead;
	INPUT_RECORD irInBuf[128];
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(hin, fdwMode);
	while (1) {
		ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
		for (int i = 0; i < cNumRead; i++) {
			if (irInBuf[i].EventType == MOUSE_EVENT) {
				// наведение и нажатие на кнопки меню
				if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 3 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 76 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < 43) {
					int xpos = irInBuf[i].Event.MouseEvent.dwMousePosition.X - (irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1) % 3;
					int ypos = irInBuf[i].Event.MouseEvent.dwMousePosition.Y - (irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) % 3;
					if (xpos > 70) xpos -= 3;
					if (ypos > 39) ypos -= 3;
					Test.Get_Level(xpos, ypos);
					Test.Edit_Presence(xpos, ypos);
					if (irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) Test.Edit_Add(xpos, ypos);
				}
				else if (flags[0] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Save.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Save.xpoz + Save.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Save.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Save.ypoz + Save.heits) {
					Save.SW(0); flags[0] = 1;
					Test.Edit_Presence(4, 1);
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Save.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Save.xpoz + Save.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Save.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Save.ypoz + Save.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					Test.TSave();
				}
				else if (flags[0] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Save.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Save.xpoz + Save.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Save.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Save.ypoz + Save.heits))) {
					Save.SW(); flags[0] = 0;
				}
				else if (flags[1] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Cancel.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Cancel.xpoz + Cancel.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Cancel.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Cancel.ypoz + Cancel.heits) {
					Cancel.SW(0); flags[1] = 1;
					Test.Edit_Presence(4, 1);
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Cancel.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Cancel.xpoz + Cancel.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Cancel.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Cancel.ypoz + Cancel.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Test.TCancel();
				}
				else if (flags[1] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Cancel.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Cancel.xpoz + Cancel.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Cancel.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Cancel.ypoz + Cancel.heits))) {
					Cancel.SW(); flags[1] = 0;
				}
				else if (flags[2] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > New.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<New.xpoz + New.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>New.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < New.ypoz + New.heits) {
					New.SW(0); flags[2] = 1;
					Test.Edit_Presence(4, 1);
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > New.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<New.xpoz + New.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>New.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < New.ypoz + New.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					Test.TNew();
				}
				else if (flags[2] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= New.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= New.xpoz + New.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= New.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= New.ypoz + New.heits))) {
					New.SW(); flags[2] = 0;
				}
				else if (flags[3] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Exit.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Exit.xpoz + Exit.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Exit.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Exit.ypoz + Exit.heits) {
					Exit.SW(0); flags[3] = 1;
					Test.Edit_Presence(4, 1);
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Exit.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Exit.xpoz + Exit.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Exit.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Exit.ypoz + Exit.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					Test.ex();
					system("cls");
					Main_Menu();
				}
				else if (flags[3] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Exit.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Exit.xpoz + Exit.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Exit.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Exit.ypoz + Exit.heits))) {
					Exit.SW(); flags[3] = 0;
				}
				else Test.Edit_Presence(4, 1);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
class Fishka {
	friend class Mahjong;
	bool can_see; // заперта или нет
	bool presence; // есть или нет
	int pos_x, pos_y, pos_z; // координаты фишки
	char symb; // простой рисунок фишки
	bool is_visited, is_choosed; // были ли мы только что на этой фишке, выбрана ли эта фишка
public:
	Fishka();
	void Show();
	void Ghost();
	void Special_Show(int f = 0);
	void Set(int px, int py, int pz, bool pres, bool visib);
	void Set_Symb(char s) { symb = s; }
	void Set_Viz(bool cs) { can_see = cs; }
};
Fishka::Fishka() {
	is_choosed = is_visited = 0;
	pos_x = pos_y = pos_z = 0;
	can_see = presence = true;
	symb = ' ';
}
void Fishka::Special_Show(int f) {
	int c1, c2, c3, c4, c5;
	if (f == 0) {
		c1 = 12; c2 = 12; c3 = 10; c4 = 9; c5 = 12;
	}
	else if (f == 1) {
		Stand(pos_x, pos_y, 0);
		for (int i = pos_y; i < pos_y + 6; i++) {
			Stand(pos_x, i, 7);
			cout << "      ";
		}
		return;
	}
	if (f == 2) {
		if (can_see) {
			c1 = 14; c2 = 12; c3 = 10; c4 = 9; c5 = 6;
		}
		else {
			c1 = 15; c2 = 7; c3 = 7; c4 = 7; c5 = 8;
		}
	}
	Window_OL(pos_x, pos_y, 6, 6, c5);
	if (symb == '!') {
		Stand(pos_x + 2, pos_y + 2, c2); cout << (char)1 << (char)1;
		Stand(pos_x + 2, pos_y + 3, c2); cout << (char)1 << (char)1;
	}
	else if (symb == '_') {
		Stand(pos_x + 2, pos_y + 2, c4); cout << (char)1 << (char)1;
		Stand(pos_x + 2, pos_y + 3, c4); cout << (char)1 << (char)1;
	}
	else if (symb == '^') {
		Stand(pos_x + 2, pos_y + 2, c3); cout << (char)1 << (char)1;
		Stand(pos_x + 2, pos_y + 3, c3); cout << (char)1 << (char)1;
	}
	else {
		Stand(pos_x + 2, pos_y + 2, c1); cout << symb << symb;
		Stand(pos_x + 2, pos_y + 3, c1); cout << symb << symb;
	}
}
void Fishka::Show() {
	int c1, c2, c3, c4, c5;
	if (can_see) {
		c1 = 14; c2 = 12; c3 = 10; c4 = 9; c5 = 6;
	}
	else {
		c1 = 7; c2 = 7; c3 = 7; c4 = 7; c5 = 8;
	}
	if (is_choosed) {
		c1 = 13; c2 = 13; c3 = 13; c4 = 13; c5 = 13;
	}
	Window_OL(pos_x, pos_y, 6, 6, c5);
	if (symb == '!') {
		Stand(pos_x + 2, pos_y + 2, c2); cout << (char)1 << (char)1;
		Stand(pos_x + 2, pos_y + 3, c2); cout << (char)1 << (char)1;
	}
	else if (symb == '_') {
		Stand(pos_x + 2, pos_y + 2, c4); cout << (char)1 << (char)1;
		Stand(pos_x + 2, pos_y + 3, c4); cout << (char)1 << (char)1;
	}
	else if (symb == '^') {
		Stand(pos_x + 2, pos_y + 2, c3); cout << (char)1 << (char)1;
		Stand(pos_x + 2, pos_y + 3, c3); cout << (char)1 << (char)1;
	}
	else {
		Stand(pos_x + 2, pos_y + 2, c1); cout << symb << symb;
		Stand(pos_x + 2, pos_y + 3, c1); cout << symb << symb;
	}
}
void Fishka::Ghost() {
	int c1, c2, c3, c4, c5;
	if (can_see) {
		c1 = 14; c2 = 12; c3 = 10; c4 = 9; c5 = 6;
	}
	else {
		c1 = 7; c2 = 7; c3 = 7; c4 = 7; c5 = 8;
	}
	if (is_choosed) {
		c1 = 13; c2 = 13; c3 = 13; c4 = 13; c5 = 13;
	}
	Stand(pos_x, pos_y, 0); //FillConsoleOutputAttribute(H,c5,6,c,0);
	Stand(pos_x, pos_y + 1, 0); //FillConsoleOutputAttribute(H,c5,1,c,0);
	Stand(pos_x + 5, pos_y + 1, 0); //FillConsoleOutputAttribute(H,c5,1,c,0); 
	Stand(pos_x, pos_y + 2, 0); //FillConsoleOutputAttribute(H,c5,1,c,0);
	Stand(pos_x + 5, pos_y + 2, 0); //FillConsoleOutputAttribute(H,c5,1,c,0); 
	Stand(pos_x, pos_y + 3, 0); //FillConsoleOutputAttribute(H,c5,1,c,0);
	Stand(pos_x + 5, pos_y + 3, 0); //FillConsoleOutputAttribute(H,c5,1,c,0); 
	Stand(pos_x, pos_y + 4, 0); //FillConsoleOutputAttribute(H,c5,1,c,0);
	Stand(pos_x + 5, pos_y + 4, 0); //FillConsoleOutputAttribute(H,c5,1,c,0); 
	Stand(pos_x, pos_y + 5, 0); //FillConsoleOutputAttribute(H,c5,6,c,0);

	if (symb == '!') {
		Stand(pos_x + 2, pos_y + 2, c2); //FillConsoleOutputAttribute(H,c2,2,c,0);
		Stand(pos_x + 2, pos_y + 3, c2); //FillConsoleOutputAttribute(H,c2,2,c,0);
	}
	else if (symb == '_') {
		Stand(pos_x + 2, pos_y + 2, c4); //FillConsoleOutputAttribute(H,c4,2,c,0);
		Stand(pos_x + 2, pos_y + 3, c4); //FillConsoleOutputAttribute(H,c4,2,c,0);
	}
	else if (symb == '^') {
		Stand(pos_x + 2, pos_y + 2, c3); //FillConsoleOutputAttribute(H,c3,2,c,0);
		Stand(pos_x + 2, pos_y + 3, c3); //FillConsoleOutputAttribute(H,c3,2,c,0);
	}
	else {
		Stand(pos_x + 2, pos_y + 2, c1); //FillConsoleOutputAttribute(H,c1,2,c,0);
		Stand(pos_x + 2, pos_y + 3, c1); //FillConsoleOutputAttribute(H,c1,2,c,0);
	}
}
void Fishka::Set(int px, int py, int pz, bool pres, bool visib) {
	pos_x = px; pos_y = py; pos_z = pz; presence = pres; can_see = visib;
}
//////////////////////////////////////////////////////////////////////////
class Mahjong {
	int proc, score, ident;
	char* name; // название раскладки
	int count; // количество фишек
	char* mass; // набор рисунков для фишек
	Fishka massiv[840];
	Fishka retorn[840];
	int rcount;
public:
	~Mahjong() {
		delete[]name;
		delete[]mass;
	}
	Mahjong(int number = 1); // раскладка по умолчанию
	void Set_Symbols(int n); // создание массива символов
	void Show_All(int sl = 0); // показ всей раскладки
	void Game_Menu(); // показ меню
	void Mouse_Control();
	void Dark_Mad();
	void Lait_Mad();
	bool Pres(int zed, int iks, int igr);
	void SV(int zed, int iks, int igr);
	void Reshuffle();
	void Restart();
	void Construct(int number);
	void Visibility();
	void MC_Help(int zed);
	void Show_When_Curs_On(int z, int x, int y);
	void Show_When_Pressed(int z, int x, int y);
	void Edit_Score(int zed, int sign);
	void Advice();
	void Reset();
	void ReShow(int x, int y, int z);
	int Get_Count() { return count; }
};
void Mahjong::Visibility() {
	for (int k = 0; k < count; k++) massiv[k].Set_Viz(0);
	for (int z = 0; z < 10; z++) {
		for (int j = 0; j < 13; j++) {
			for (int i = 0; i < 23; i++) {
				if (Pres(z + 1, i * 3 + 4, j * 3 + 1) == 1) {
					if (Pres(z + 1, (i - 2) * 3 + 4, (j - 1) * 3 + 1) == 0 && Pres(z + 1, (i - 2) * 3 + 4, (j) * 3 + 1) == 0 && Pres(z + 1, (i - 2) * 3 + 4, (j + 1) * 3 + 1) == 0 || Pres(z + 1, (i + 2) * 3 + 4, (j - 1) * 3 + 1) == 0 && Pres(z + 1, (i + 2) * 3 + 4, (j) * 3 + 1) == 0 && Pres(z + 1, (i + 2) * 3 + 4, (j + 1) * 3 + 1) == 0) {
						if (Pres((z + 1) + 1, (i - 1) * 3 + 4, (j - 1) * 3 + 1) == 0 && Pres((z + 1) + 1, (i) * 3 + 4, (j - 1) * 3 + 1) == 0 && Pres((z + 1) + 1, (i + 1) * 3 + 4, (j - 1) * 3 + 1) == 0 && Pres((z + 1) + 1, (i - 1) * 3 + 4, (j) * 3 + 1) == 0 && Pres((z + 1) + 1, (i) * 3 + 4, (j) * 3 + 1) == 0 && Pres((z + 1) + 1, (i + 1) * 3 + 4, (j) * 3 + 1) == 0 && Pres((z + 1) + 1, (i - 1) * 3 + 4, (j + 1) * 3 + 1) == 0 && Pres((z + 1) + 1, (i) * 3 + 4, (j + 1) * 3 + 1) == 0 && Pres((z + 1) + 1, (i + 1) * 3 + 4, (j + 1) * 3 + 1) == 0) {
							SV(z + 1, i * 3 + 4, j * 3 + 1);
						}
					}
				}
			}
		}
	}
}
void Mahjong::Construct(int number) {
	count = 0;
	FILE* f;
	//////////////////////////////////////////////////////////////////////////
	if (number < 0) {
		f = fopen(global_address, "r+");
	}
	if (number == 0) f = fopen("Game_Maps\\0.txt", "r+");
	if (number == 666) f = fopen("Game_Maps\\666.txt", "r+");
	//////////////////////////////////////////////////////////////////////////
	name = new char[20];
	fgets(name, 20, f);
	fscanf(f, "%d", &count);
	for (int i = 0; i < count; i++) fscanf(f, "%d %d %d", &massiv[i].pos_x, &massiv[i].pos_y, &massiv[i].pos_z);
	fclose(f);
	mass = new char[count];
	Set_Symbols(count);
	for (int i = 0; i < count; i++) massiv[i].Set_Symb(mass[i]);
}
Mahjong::Mahjong(int number) {
	score = rcount = 0;
	ident = number;
	Construct(ident);
	proc = count;
}
/* установка случайного рисунка на фишке */
void Mahjong::Set_Symbols(int n) {
	int* random_access = new int[n]; // случайные индексы при перетасовке
	for (int i = 0; i < n; i++) random_access[i] = i;
	int t1, t2, temp;
	for (int i = 0; i < n * 2; i++) {
		t1 = rand() % n;
		t2 = rand() % n;
		temp = random_access[t1];
		random_access[t1] = random_access[t2];
		random_access[t2] = temp;
	}
	char temps;
	int type;
	for (int i = 0; i < n; i += 2) {
		type = rand() % 8;
		if (type == 0 || type == 1 || type == 2 || type == 3) { // буквы
			type = rand() % 18;
			temps = 65 + type;
		}
		else if (type == 4 || type == 5) { // цифры
			type = rand() % 9;
			temps = 49 + type;
		}
		else if (type == 6) {
			type = rand() % 3;
			if (type == 0) temps = '!';  // драконы
			else if (type == 1) temps = '_';
			else if (type == 2) temps = '^';
		}
		else if (type == 7) { // ветры
			type = rand() % 4;
			temps = 24 + type;
		}
		mass[random_access[i]] = mass[random_access[i + 1]] = temps;
	}
	delete[]random_access;
}
/* показ всех фишек */
void Mahjong::Show_All(int sl) {
	for (int z = 1; z < 11; z++) {
		for (int j = 1; j < 40; j += 3) {
			for (int i = 4; i < 76; i += 3) {
				for (int k = 0; k < count; k++) {
					if (massiv[k].pos_x == i && massiv[k].pos_y == j && massiv[k].pos_z == z) {
						massiv[k].Show();
						Sleep(sl);
					}
				}
			}
		}
	}
}
/* показ игровых кнопок */
void Mahjong::Game_Menu() {
	Stand(0, 44, 2); for (int i = 0; i < 80; i++) cout << (char)205;
	Stand(2, 46, 7); text("ГЛАЗ");
	Stand(3, 48, 7); text("ОТМЕНА ХОДА");
	Stand(10, 46, 7); text("ПЕРЕТАСОВКА");
	Stand(19, 48, 7); text("СОВЕТ");
	Stand(25, 46, 7); text("ПЕРЕИГРАТЬ");
	Stand(29, 48, 7); text("ВЫХОД");
	Stand(37, 44, 2); cout << (char)203;
	for (c.Y = 45; c.Y < 50; c.Y++) { Stand(37, c.Y, 2); cout << (char)186; }
	Stand(39, 46, 15); text("СЧЁТ:      0");
	Stand(80 - strlen(name), 46, 10); text(name);
	Stand(39, 47, 7); text("УРОВЕНЬ ФИШКИ:");
	Stand(70, 47, 8); text("0%");
}
/* показ уровня фишки */
void Mahjong::MC_Help(int zed) {
	menu_activ[4] = 1;
	Stand(54, 47, 6);
	if (zed < 10) cout << " ";
	cout << zed;
}
/* наведение курсора на фишки */
void Mahjong::Show_When_Curs_On(int z, int x, int y) {
	int tx, ty, tz, neon;
	for (int i = 0; i < count; i++) {
		if (massiv[i].is_visited && massiv[i].pos_x == x && massiv[i].pos_y == y && massiv[i].pos_z == z) return; // курсор находится над одной и той же фишкой - нет смысла ни снимать флажок посещения, ни менять цвет
		else if (massiv[i].is_visited && (massiv[i].pos_x != x || massiv[i].pos_y != y || massiv[i].pos_z != z)) {
			massiv[i].is_visited = 0;
			tx = massiv[i].pos_x;
			ty = massiv[i].pos_y;
			tz = massiv[i].pos_z;
			neon = i;
			//////////////////////////////////////////////////////////////////////////
			int a[4] = { 0,0,0,0 };
			for (int i = 0; i < count; i++) {
				if (massiv[i].pos_x == tx - 3 && massiv[i].pos_y == ty - 3 && massiv[i].pos_z == tz + 1) a[0]++;
				else if (massiv[i].pos_x == tx - 3 && massiv[i].pos_y == ty && massiv[i].pos_z == tz + 1) a[0]++;
				else if (massiv[i].pos_x == tx && massiv[i].pos_y == ty - 3 && massiv[i].pos_z == tz + 1) a[0]++;

				if (massiv[i].pos_x == tx - 3 && massiv[i].pos_y == ty && massiv[i].pos_z == tz + 1) a[1]++;
				else if (massiv[i].pos_x == tx - 3 && massiv[i].pos_y == ty + 3 && massiv[i].pos_z == tz + 1) a[1]++;
				else if (massiv[i].pos_x == tx && massiv[i].pos_y == ty + 3 && massiv[i].pos_z == tz + 1) a[1]++;

				if (massiv[i].pos_x == tx && massiv[i].pos_y == ty - 3 && massiv[i].pos_z == tz + 1) a[2]++;
				else if (massiv[i].pos_x == tx + 3 && massiv[i].pos_y == ty - 3 && massiv[i].pos_z == tz + 1) a[2]++;
				else if (massiv[i].pos_x == tx + 3 && massiv[i].pos_y == ty && massiv[i].pos_z == tz + 1) a[2]++;

				if (massiv[i].pos_x == tx && massiv[i].pos_y == ty + 3 && massiv[i].pos_z == tz + 1) a[3]++;
				else if (massiv[i].pos_x == tx + 3 && massiv[i].pos_y == ty && massiv[i].pos_z == tz + 1) a[3]++;
				else if (massiv[i].pos_x == tx + 3 && massiv[i].pos_y == ty + 3 && massiv[i].pos_z == tz + 1) a[3]++;
			}
			if (a[0] == 0) {
				if (menu_activ[0] == 0 || massiv[neon].can_see == 1) {
					for (int i = ty; i < ty + 3; i++) {
						Stand(tx, i, 0); //FillConsoleOutputAttribute(H,6,3,c,0);
					}
					Stand(tx + 2, ty + 2, 0);
					//if(massiv[neon].symb=='!') //FillConsoleOutputAttribute(H,12,1,c,0);
					//else if(massiv[neon].symb=='^') //FillConsoleOutputAttribute(H,10,1,c,0);
					//else if(massiv[neon].symb=='_') //FillConsoleOutputAttribute(H,9,1,c,0);
					//else //FillConsoleOutputAttribute(H,14,1,c,0);
				}
				else {
					for (int i = ty; i < ty + 3; i++) {
						Stand(tx, i, 0); //FillConsoleOutputAttribute(H,8,3,c,0);
					}
					Stand(tx + 2, ty + 2, 0);
					//FillConsoleOutputAttribute(H,7,1,c,0);
				}
			}
			if (a[1] == 0) {
				if (menu_activ[0] == 0 || massiv[neon].can_see == 1) {
					for (int i = ty + 3; i < ty + 6; i++) {
						Stand(tx, i, 0); //FillConsoleOutputAttribute(H,6,3,c,0);
					}
					Stand(tx + 2, ty + 3, 0);
					//if(massiv[neon].symb=='!') //FillConsoleOutputAttribute(H,12,1,c,0);
					//else if(massiv[neon].symb=='^') //FillConsoleOutputAttribute(H,10,1,c,0);
					//else if(massiv[neon].symb=='_') //FillConsoleOutputAttribute(H,9,1,c,0);
					//else //FillConsoleOutputAttribute(H,14,1,c,0);
				}
				else {
					for (int i = ty + 3; i < ty + 6; i++) {
						Stand(tx, i, 0); //FillConsoleOutputAttribute(H,8,3,c,0);
					}
					Stand(tx + 2, ty + 3, 0);
					//FillConsoleOutputAttribute(H,7,1,c,0);
				}

			}
			if (a[2] == 0) {
				if (menu_activ[0] == 0 || massiv[neon].can_see == 1) {
					for (int i = ty; i < ty + 3; i++) {
						Stand(tx + 3, i, 0); //FillConsoleOutputAttribute(H,6,3,c,0);
					}
					Stand(tx + 3, ty + 2, 0);
					//if(massiv[neon].symb=='!') //FillConsoleOutputAttribute(H,12,1,c,0);
					//else if(massiv[neon].symb=='^') //FillConsoleOutputAttribute(H,10,1,c,0);
					//else if(massiv[neon].symb=='_') //FillConsoleOutputAttribute(H,9,1,c,0);
					//else //FillConsoleOutputAttribute(H,14,1,c,0);
				}
				else {
					for (int i = ty; i < ty + 3; i++) {
						Stand(tx + 3, i, 0); //FillConsoleOutputAttribute(H,8,3,c,0);
					}
					Stand(tx + 3, ty + 2, 0);
					//FillConsoleOutputAttribute(H,7,1,c,0);
				}
			}
			if (a[3] == 0) {
				if (menu_activ[0] == 0 || massiv[neon].can_see == 1) {
					for (int i = ty + 3; i < ty + 6; i++) {
						Stand(tx + 3, i, 0); //FillConsoleOutputAttribute(H,6,3,c,0);
					}
					Stand(tx + 3, ty + 3, 0);
					//if(massiv[neon].symb=='!') //FillConsoleOutputAttribute(H,12,1,c,0);
					//else if(massiv[neon].symb=='^') //FillConsoleOutputAttribute(H,10,1,c,0);
					//else if(massiv[neon].symb=='_') //FillConsoleOutputAttribute(H,9,1,c,0);
					//else //FillConsoleOutputAttribute(H,14,1,c,0);
				}
				else {
					for (int i = ty + 3; i < ty + 6; i++) {
						Stand(tx + 3, i, 0); //FillConsoleOutputAttribute(H,8,3,c,0);
					}
					Stand(tx + 3, ty + 3, 0);
					//FillConsoleOutputAttribute(H,7,1,c,0);
				}
				if (massiv[neon].is_choosed == 1) {
					for (int i = ty; i < ty + 6; i++) {
						Stand(tx, i, 0); //FillConsoleOutputAttribute(H,13,6,c,0);
					}
				}
			}
		}
	}
	if (x == 0 && y == 0 && z == 0) return;
	int color = 0, index = 0;
	for (int i = 0; i < count; i++) {
		if (massiv[i].pos_x == x && massiv[i].pos_y == y && massiv[i].pos_z == z) {
			if (massiv[i].can_see == 1) color = 9;
			else color = 12;
			index = i;
			break;
		}
	}
	if (massiv[index].is_visited) return;
	int a[4] = { 0,0,0,0 };
	for (int i = 0; i < count; i++) {
		if (massiv[i].pos_x == x - 3 && massiv[i].pos_y == y - 3 && massiv[i].pos_z == z + 1) a[0]++;
		else if (massiv[i].pos_x == x - 3 && massiv[i].pos_y == y && massiv[i].pos_z == z + 1) a[0]++;
		else if (massiv[i].pos_x == x && massiv[i].pos_y == y - 3 && massiv[i].pos_z == z + 1) a[0]++;

		if (massiv[i].pos_x == x - 3 && massiv[i].pos_y == y && massiv[i].pos_z == z + 1) a[1]++;
		else if (massiv[i].pos_x == x - 3 && massiv[i].pos_y == y + 3 && massiv[i].pos_z == z + 1) a[1]++;
		else if (massiv[i].pos_x == x && massiv[i].pos_y == y + 3 && massiv[i].pos_z == z + 1) a[1]++;

		if (massiv[i].pos_x == x && massiv[i].pos_y == y - 3 && massiv[i].pos_z == z + 1) a[2]++;
		else if (massiv[i].pos_x == x + 3 && massiv[i].pos_y == y - 3 && massiv[i].pos_z == z + 1) a[2]++;
		else if (massiv[i].pos_x == x + 3 && massiv[i].pos_y == y && massiv[i].pos_z == z + 1) a[2]++;

		if (massiv[i].pos_x == x && massiv[i].pos_y == y + 3 && massiv[i].pos_z == z + 1) a[3]++;
		else if (massiv[i].pos_x == x + 3 && massiv[i].pos_y == y && massiv[i].pos_z == z + 1) a[3]++;
		else if (massiv[i].pos_x == x + 3 && massiv[i].pos_y == y + 3 && massiv[i].pos_z == z + 1) a[3]++;
	}
	if (a[0] == 0) {
		for (int i = y; i < y + 3; i++) {
			Stand(x, i, 0); //FillConsoleOutputAttribute(H,color,3,c,0);
		}
	}
	if (a[1] == 0) {
		for (int i = y + 3; i < y + 6; i++) {
			Stand(x, i, 0); //FillConsoleOutputAttribute(H,color,3,c,0);
		}
	}
	if (a[2] == 0) {
		for (int i = y; i < y + 3; i++) {
			Stand(x + 3, i, 0); //FillConsoleOutputAttribute(H,color,3,c,0);
		}
	}
	if (a[3] == 0) {
		for (int i = y + 3; i < y + 6; i++) {
			Stand(x + 3, i, 0); //FillConsoleOutputAttribute(H,color,3,c,0);
		}
	}
	massiv[index].is_visited = 1;
}
/* показ очков */
void Mahjong::Edit_Score(int zed, int sign) {
	score += zed * sign;
	Stand(45, 46, 7); cout << "      ";
	Stand(51 - Razer(score), 46, 12); cout << score;
}
/* заполнение чёрных пятен после снятия фишки */
void RSHelp(char symba, int col) {
	if (col == 8) {
		SetConsoleTextAttribute(H, 7);
		if (symba == '!' || symba == '^' || symba == '_') cout << (char)1;
		else cout << symba;
	}
	else {
		if (symba == '!') { SetConsoleTextAttribute(H, 12); cout << (char)1; }
		else if (symba == '^') { SetConsoleTextAttribute(H, 10); cout << (char)1; }
		else if (symba == '_') { SetConsoleTextAttribute(H, 9); cout << (char)1; }
		else { SetConsoleTextAttribute(H, 14); cout << symba; }
	}
	SetConsoleTextAttribute(H, col);
}
void Mahjong::ReShow(int x, int y, int z) {
	int l1;
	for (int i = 0; i < count; i++) {
		if (massiv[i].can_see) l1 = 6;
		else l1 = 8;
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y - 3 && massiv[i].pos_x == x - 3) {
			Stand(x, y, l1); RSHelp(massiv[i].symb, l1); cout << " " << (char)179;
			Stand(x, y + 1, l1); cout << "  " << (char)179;
			Stand(x, y + 2, l1); cout << (char)196 << (char)196 << (char)217;
		}
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y - 3 && massiv[i].pos_x == x) {
			Stand(x, y, l1); cout << (char)179 << " "; RSHelp(massiv[i].symb, l1); RSHelp(massiv[i].symb, l1); cout << " " << (char)179;
			Stand(x, y + 1, l1); cout << (char)179 << "    " << (char)179;
			Stand(x, y + 2, l1); cout << (char)192 << (char)196 << (char)196 << (char)196 << (char)196 << (char)217;
		}
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y - 3 && massiv[i].pos_x == x + 3) {
			Stand(x + 3, y, l1); cout << (char)179 << " "; RSHelp(massiv[i].symb, l1);
			Stand(x + 3, y + 1, l1); cout << (char)179 << "  ";
			Stand(x + 3, y + 2, l1); cout << (char)192 << (char)196 << (char)196;
		}
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y && massiv[i].pos_x == x - 3) {
			Stand(x, y, l1); cout << (char)196 << (char)196 << (char)191;
			Stand(x, y + 1, l1); cout << "  " << (char)179;
			Stand(x, y + 2, l1); RSHelp(massiv[i].symb, l1); cout << " " << (char)179;
			Stand(x, y + 3, l1); RSHelp(massiv[i].symb, l1); cout << " " << (char)179;
			Stand(x, y + 4, l1); cout << "  " << (char)179;
			Stand(x, y + 5, l1); cout << (char)196 << (char)196 << (char)217;
		}
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y && massiv[i].pos_x == x) {
			massiv[i].Show();
		}
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y && massiv[i].pos_x == x + 3) {
			Stand(x + 3, y, l1); cout << (char)218 << (char)196 << (char)196;
			Stand(x + 3, y + 1, l1); cout << (char)179 << "  ";
			Stand(x + 3, y + 2, l1); cout << (char)179 << " "; RSHelp(massiv[i].symb, l1);
			Stand(x + 3, y + 3, l1); cout << (char)179 << " "; RSHelp(massiv[i].symb, l1);
			Stand(x + 3, y + 4, l1); cout << (char)179 << "  ";
			Stand(x + 3, y + 5, l1); cout << (char)192 << (char)196 << (char)196;
		}
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y + 3 && massiv[i].pos_x == x - 3) {
			Stand(x, y + 3, l1); cout << (char)196 << (char)196 << (char)191;
			Stand(x, y + 4, l1); cout << "  " << (char)179;
			Stand(x, y + 5, l1); RSHelp(massiv[i].symb, l1); cout << " " << (char)179;
		}
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y + 3 && massiv[i].pos_x == x) {
			Stand(x, y + 3, l1); cout << (char)218 << (char)196 << (char)196 << (char)196 << (char)196 << (char)191;
			Stand(x, y + 4, l1); cout << (char)179 << "    " << (char)179;
			Stand(x, y + 5, l1); cout << (char)179 << " "; RSHelp(massiv[i].symb, l1); RSHelp(massiv[i].symb, l1); cout << " " << (char)179;
		}
		if (massiv[i].pos_z == z - 1 && massiv[i].pos_y == y + 3 && massiv[i].pos_x == x + 3) {
			Stand(x + 3, y + 3, l1); cout << (char)218 << (char)196 << (char)196;
			Stand(x + 3, y + 4, l1); cout << (char)179 << "  ";
			Stand(x + 3, y + 5, l1); cout << (char)179 << " "; RSHelp(massiv[i].symb, l1);
		}
	}
}
/* нажатие на фишку */
void Mahjong::Show_When_Pressed(int z, int x, int y) {
	Visibility();
	for (int i = 0; i < count; i++) {
		if (massiv[i].can_see == 1 && massiv[i].is_choosed == 0 && massiv[i].pos_x == x && massiv[i].pos_y == y && massiv[i].pos_z == z) {
			press = i;
			for (int j = 0; j < 6; j++) {
				Stand(x, y + j, 13); //FillConsoleOutputAttribute(H,13,6,c,0);
			}
			massiv[i].is_choosed = 1; break;
		}
		else if (massiv[i].is_choosed == 1 && massiv[i].pos_x == x && massiv[i].pos_y == y && massiv[i].pos_z == z) {
			massiv[i].is_choosed = 0;
			massiv[i].Show(); break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	int index[2] = { -1,-1 };
	for (int i = 0; i < count; i++) {
		if (massiv[i].can_see == 1 && massiv[i].is_choosed == 1) {
			if (index[0] < 0) { index[0] = i; continue; }
			if (index[1] < 0) index[1] = i;
		}
	}
	if (index[0] >= 0 && index[1] >= 0) {
		if (massiv[index[0]].symb == massiv[index[1]].symb) {
			rcount += 2;
			retorn[rcount - 2] = massiv[index[0]];
			retorn[rcount - 1] = massiv[index[1]];
			massiv[index[0]].Special_Show(1);
			massiv[index[1]].Special_Show(1);
			int tempx1 = massiv[index[0]].pos_x;
			int tempy1 = massiv[index[0]].pos_y;
			int tempz1 = massiv[index[0]].pos_z;
			int tempx2 = massiv[index[1]].pos_x;
			int tempy2 = massiv[index[1]].pos_y;
			int tempz2 = massiv[index[1]].pos_z;
			massiv[index[0]].is_choosed = massiv[index[1]].is_choosed = 0;
			massiv[index[0]].presence = massiv[index[1]].presence = 0;
			for (int i = index[0]; i < index[1]; i++) massiv[i] = massiv[i + 1];
			for (int i = index[1] - 1; i < count - 2; i++) massiv[i] = massiv[i + 2];
			count -= 2;
			//////////////////////////////////////////////////////////////////////////
			ReShow(tempx1, tempy1, tempz1);
			ReShow(tempx2, tempy2, tempz2);
			//////////////////////////////////////////////////////////////////////////
			bool temp_massiv[840] = { 0 };
			if (menu_activ[0] == 1) {
				for (int i = 0; i < count; i++) {
					if (massiv[i].can_see == 0 && massiv[i].presence == 1)
						temp_massiv[i] = 1;
				}
				Visibility();
				for (int i = 0; i < count; i++) {
					if (massiv[i].can_see == 1 && massiv[i].presence == 1 && temp_massiv[i] == 1) {
						massiv[i].Show();
					}
				}
			}
			else {
				for (int i = 0; i < count; i++) massiv[i].can_see = 1;
				ReShow(tempx1, tempy1, tempz1);
				ReShow(tempx2, tempy2, tempz2);
			}
			//////////////////////////////////////////////////////////////////////////
			int temp = (proc - count) * 100 / proc;
			Stand(71 - Razer(temp), 47, 12); cout << temp; cout << "%";
			for (c.X = 60; c.X <= 60 + temp / 5.55; c.X++) {
				Stand(c.X, 47, 0);
				//FillConsoleOutputAttribute(H,192,1,c,0);
			}
			Edit_Score(200, 1);
		}
		else {
			if (press == index[0]) {
				massiv[index[1]].is_choosed = 0;
				massiv[index[1]].Show();
			}
			else {
				massiv[index[0]].is_choosed = 0;
				massiv[index[0]].Show();
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (menu_activ[0] == 0) for (int k = 0; k < count; k++) massiv[k].Set_Viz(1);
}
void HighScore() {
	SetConsoleTextAttribute(H, 7); system("cls");
	Wind Congr("Поздравляем! Вы самый лучший игрок маджонга!");
	while (_kbhit() == 0) {
		Congr.linkw = rand() % 6 + 1;
		Congr.linkt = Congr.linkw + 8;
		Congr.Right(-Congr.widz, rand() % 40 + 5, 150, 50);
		if (_kbhit() == 0) {
			Congr.linkw = rand() % 6 + 1;
			Congr.linkt = Congr.linkw + 8;
			Congr.Left(80, rand() % 40 + 5, 150, 50);
		}
	}
	menu_activ[0] = 0;
	Main_Menu();
}
/* работа мыши: выбор-наведение-нажатие */
void Mahjong::Mouse_Control() {
	menu_activ[4] = 1;
	DWORD fdwMode, cNumRead;
	INPUT_RECORD irInBuf[128];
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(hin, fdwMode);
	int www = 0;
	while (www == 0) {
		ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
		for (int i = 0; i < cNumRead; i++) {
			if (irInBuf[i].EventType == MOUSE_EVENT) {
				// наведение и нажатие на кнопки меню
				if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 6 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 46 && menu_flags[0] == 0) {
					Window(0, 45, 8, 3, 2);
					Stand(2, 46, 10); text("ГЛАЗ");
					menu_flags[0] = 1;
					Stand(39, 48, 8);
					if (menu_activ[0] == 0) text("Затенить закрытые фишки");
					else text("Показать закрытые фишки");
				}
				else if ((irInBuf[i].Event.MouseEvent.dwMousePosition.X < 2 || irInBuf[i].Event.MouseEvent.dwMousePosition.X>5 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y != 46) && menu_flags[0] == 1) {

					Window_CL(0, 45, 8, 3, 7);
					Stand(2, 46, 7); text("ГЛАЗ");
					menu_flags[0] = 0;
					Stand(39, 48, 8); for (int k = 0; k < 40; k++) cout << " ";
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 6 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 46 && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					if (menu_activ[0] == 0) Dark_Mad();
					else Lait_Mad();
				}
				//////////////////////////////////////////////////////////////////////////
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 9 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 21 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 46 && menu_flags[1] == 0) {
					Window(8, 45, 15, 3, 6);
					Stand(10, 46, 14); text("ПЕРЕТАСОВКА");
					menu_flags[1] = 1;
					Stand(39, 48, 8);
					if (score < prices[0]) { text("Стоимость - "); cout << prices[0]; text(" очков"); }
					else { text("Перемешать фишки - "); cout << prices[0]; }
				}
				else if ((irInBuf[i].Event.MouseEvent.dwMousePosition.X < 10 || irInBuf[i].Event.MouseEvent.dwMousePosition.X>20 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y != 46) && menu_flags[1] == 1) {
					Window_CL(8, 45, 15, 3, 7);
					Stand(10, 46, 7); text("ПЕРЕТАСОВКА");
					menu_flags[1] = 0;
					Stand(39, 48, 8); for (int k = 0; k < 40; k++) cout << " ";
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 9 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 21 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 46 && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					if (score >= prices[0]) {
						Edit_Score(prices[0], -1);
						Reshuffle();
					}
				}
				//////////////////////////////////////////////////////////////////////////
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 24 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 35 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 46 && menu_flags[2] == 0) {
					Window(23, 45, 14, 3, 5);
					Stand(25, 46, 13); text("ПЕРЕИГРАТЬ");
					menu_flags[2] = 1;
					Stand(39, 48, 8); text("Начать игру заново");
				}
				else if ((irInBuf[i].Event.MouseEvent.dwMousePosition.X < 25 || irInBuf[i].Event.MouseEvent.dwMousePosition.X>34 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y != 46) && menu_flags[2] == 1) {
					Window_CL(23, 45, 14, 3, 7);
					Stand(25, 46, 7); text("ПЕРЕИГРАТЬ");
					menu_flags[2] = 0;
					Stand(39, 48, 8); for (int k = 0; k < 40; k++) cout << " ";
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 24 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 35 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 46 && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Restart();
				}
				//////////////////////////////////////////////////////////////////////////
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 14 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 48 && menu_flags[3] == 0) {
					Window(1, 47, 15, 3, 1);
					Stand(3, 48, 9); text("ОТМЕНА ХОДА");
					menu_flags[3] = 1;
					Stand(39, 48, 8);
					if (count == proc) text("Нет снятых фишек");
					else if (score < prices[2]) {
						text("Стоимость - "); cout << prices[2]; text(" очков");
					}
					else { text("Вернуть фишки на место - "); cout << prices[2]; }
				}
				else if ((irInBuf[i].Event.MouseEvent.dwMousePosition.X < 3 || irInBuf[i].Event.MouseEvent.dwMousePosition.X>13 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y != 48) && menu_flags[3] == 1) {
					Window_CL(1, 47, 15, 3, 7);
					Stand(3, 48, 7); text("ОТМЕНА ХОДА");
					menu_flags[3] = 0;
					Stand(39, 48, 8); for (int k = 0; k < 40; k++) cout << " ";
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 14 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 48 && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					if (count < proc && score >= prices[2]) {
						Edit_Score(prices[2], -1);
						Reset();
					}
				}
				//////////////////////////////////////////////////////////////////////////
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 18 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 24 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 48 && menu_flags[4] == 0) {
					Window(17, 47, 9, 3, 3);
					Stand(19, 48, 11); text("СОВЕТ");
					menu_flags[4] = 1;
					Stand(39, 48, 8);
					if (score < prices[1]) {
						text("Стоимость - "); cout << prices[1]; text(" очков");
					}
					else { text("Получить подсказку - "); cout << prices[1]; }
				}
				else if ((irInBuf[i].Event.MouseEvent.dwMousePosition.X < 19 || irInBuf[i].Event.MouseEvent.dwMousePosition.X>23 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y != 48) && menu_flags[4] == 1) {
					Window_CL(17, 47, 9, 3, 7);
					Stand(19, 48, 7); text("СОВЕТ");
					menu_flags[4] = 0;
					Stand(39, 48, 8); for (int k = 0; k < 40; k++) cout << " ";
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 18 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 24 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 48 && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					if (score >= prices[1]) Advice();
				}
				//////////////////////////////////////////////////////////////////////////
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 28 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 34 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 48 && menu_flags[5] == 0) {
					Window(27, 47, 9, 3, 4);
					Stand(29, 48, 12); text("ВЫХОД");
					menu_flags[5] = 1;
					Stand(39, 48, 8); text("Выйти в главное меню");
				}
				else if ((irInBuf[i].Event.MouseEvent.dwMousePosition.X < 29 || irInBuf[i].Event.MouseEvent.dwMousePosition.X>33 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y != 48) && menu_flags[5] == 1) {
					Window_CL(27, 47, 9, 3, 7);
					Stand(29, 48, 7); text("ВЫХОД");
					menu_flags[5] = 0;
					Stand(39, 48, 8); for (int k = 0; k < 40; k++) cout << " ";
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 28 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 34 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 48 && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					www = 1;
					system("cls");
					menu_activ[0] = 0;
					Main_Menu();
				}
				//////////////////////////////////////////////////////////////////////////
				// [ ПОКАЗ ТЕКУЩЕГО УРОВНЯ ФИШКИ ] //
				if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 3 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 76 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < 43) {
					if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1 || Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								MC_Help(z);
								break;
							}
							else menu_activ[4] = 0;
						}
					}
				}
				else menu_activ[4] = 0;
				//////////////////////////////////////////////////////////////////////////
				// [ НАВЕДЕНИЕ КУРСОРА НА ФИШКИ ] //
				if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 3 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 76 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < 43 && irInBuf[i].Event.MouseEvent.dwButtonState != FROM_LEFT_1ST_BUTTON_PRESSED) {
					if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Curs_On(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (z == 1) Show_When_Curs_On(0, 0, 0);
						}
					}
				}
				else Show_When_Curs_On(0, 0, 0);
				//////////////////////////////////////////////////////////////////////////
				// нажатие на незапертые фишки
				if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 3 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 76 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < 43 && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 1) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3);
								break;
							}
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 2) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 1);
								break;
							}
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 2 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 1, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
						}
					}
					else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X % 3 == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y % 3 == 0) {
						for (int z = 10; z > 0; z--) {
							if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 3 - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
							else if (Pres(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2) == 1) {
								Show_When_Pressed(z, irInBuf[i].Event.MouseEvent.dwMousePosition.X - 2, irInBuf[i].Event.MouseEvent.dwMousePosition.Y - 3 - 2);
								break;
							}
						}
					}
				}
			}
			if (menu_activ[4] == 0) { Stand(54, 47, 7); cout << "  "; }
			if (count == 0) HighScore();
		}
	}
	Main_Menu();
}
/* проверка есть фишка на этом месте в массиве или нет */
bool Mahjong::Pres(int zed, int iks, int igr) {
	for (int k = 0; k < count; k++)
		if (massiv[k].pos_x == iks && massiv[k].pos_y == igr && massiv[k].pos_z == zed)
			return massiv[k].presence;
	return 0;
}
/* установка видимости фишки */
void Mahjong::SV(int zed, int iks, int igr) {
	for (int k = 0; k < count; k++) {
		if (massiv[k].pos_x == iks && massiv[k].pos_y == igr && massiv[k].pos_z == zed) {
			massiv[k].can_see = 1;
			break;
		}
	}
}
/* режим глазик включён */
void Mahjong::Dark_Mad() {
	menu_activ[0] = 1; Visibility();
	for (int k = 0; k < count; k++) massiv[k].Ghost();
}
/* режим глазик отключён */
void Mahjong::Lait_Mad() {
	menu_activ[0] = 0;
	for (int k = 0; k < count; k++) {
		massiv[k].Set_Viz(1);
		massiv[k].Ghost();
	}
}
/* перетасовка */
void Mahjong::Reshuffle() {
	for (int i = count - 1; i >= 0; i--) {
		massiv[i].Special_Show(1);
		ReShow(massiv[i].pos_x, massiv[i].pos_y, massiv[i].pos_z);
		Sleep(20);
	}
	//////////////////////////////////////////////////////////////////////////
	int t1, t2;
	char temp;
	for (int i = 0; i < count; i++) {
		t1 = rand() % count;
		t2 = rand() % count;
		temp = massiv[i].symb;
		massiv[i].symb = massiv[t1].symb;
		massiv[t1].symb = massiv[t2].symb;
		massiv[t2].symb = temp;
	}
	//////////////////////////////////////////////////////////////////////////
	Sleep(200);
	for (int i = 0; i < count; i++) {
		massiv[i].Show();
		Sleep(20);
	}
}
/* начать заново */
void Mahjong::Restart() {
	score = 0;
	Stand(39, 46, 15); text("СЧЁТ:      0");
	Stand(60, 47, 7); cout << "                   ";
	Stand(70, 47, 8); text("0%");
	delete[]name; delete[]mass;
	Construct(ident);
	for (int i = 0; i < count; i++) {
		massiv[i].can_see = 1;
		massiv[i].is_choosed = 0;
		massiv[i].is_visited = 0;
		massiv[i].presence = 1;
	}
	if (menu_activ[0] == 1) Visibility();
	//////////////////////////////////////////////////////////////////////////
	int Sl = 15;
	int run_away = rand() % 4;
	if (run_away == 0) {
		for (int j = 0; j < 44; j++) {
			for (int i = 0; i < 80; i++) {
				Stand(i, j, 7); cout << " ";
			}
			Sleep(Sl);
		}
	}
	else if (run_away == 1) {
		for (int j = 43; j >= 0; j--) {
			for (int i = 0; i < 80; i++) {
				Stand(i, j, 7); cout << " ";
			}
			Sleep(Sl);
		}
	}
	else if (run_away == 2) {
		for (int i = 0; i < 80; i++) {
			for (int j = 43; j >= 0; j--) {
				Stand(i, j, 7); cout << " ";
			}
			Sleep(Sl);
		}
	}
	else {
		for (int i = 79; i >= 0; i--) {
			for (int j = 43; j >= 0; j--) {
				Stand(i, j, 7); cout << " ";
			}
			Sleep(Sl);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < count; i++) {
		massiv[i].Show();
		Sleep(15);
	}
}
/* вернуть фишки на доску */
void Mahjong::Reset() {
	retorn[rcount - 2].is_choosed = 0;
	retorn[rcount - 1].is_choosed = 0;
	massiv[count] = retorn[rcount - 2];
	massiv[count + 1] = retorn[rcount - 1];
	massiv[count].Show();
	massiv[count + 1].Show();
	count += 2; rcount -= 2;

	Fishka TEMP;
	for (int i = 0; i < count; i++) {
		for (int j = count - 1; j > i; j--) {
			if (massiv[j - 1].pos_z > massiv[j].pos_z) {
				TEMP = massiv[j - 1];
				massiv[j - 1] = massiv[j];
				massiv[j] = TEMP;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (menu_activ[0] == 1) Visibility();
	for (int i = 0; i < count; i++) {
		massiv[i].is_choosed = 0;
		massiv[i].is_visited = 0;
		massiv[i].Ghost();
	}
	//////////////////////////////////////////////////////////////////////////
	int temp = (proc - count) * 100 / proc;
	Stand(71 - Razer(temp), 47, 12); cout << temp; cout << "%";
	for (c.X = 60; c.X <= 60 + temp / 5.55; c.X++) {
		Stand(c.X, 47, 0);
		//FillConsoleOutputAttribute(H,192,1,c,0);
	}
}
/* подсказка */
void Mahjong::Advice() {
	int* random_access = new int[count]; // случайные индексы при перетасовке
	for (int i = 0; i < count; i++) random_access[i] = i;
	int t1, t2, temp;
	for (int i = 0; i < count * 2; i++) {
		t1 = rand() % count;
		t2 = rand() % count;
		temp = random_access[t1];
		random_access[t1] = random_access[t2];
		random_access[t2] = temp;
	}
	Visibility();
	int stop = 1, pause = 75;
	int w = 0, m;
	for (int i = random_access[w], t = 0; t < count && stop; i = random_access[w++], t++) {
		m = rand() % 2;
		if (m) {
			for (int j = i + 1; j < count && stop; j++) {
				if (massiv[i].can_see == 1 && massiv[j].can_see == 1 && massiv[i].symb == massiv[j].symb) {
					Edit_Score(prices[1], -1);
					for (int l = 0; l < 12; l++) {
						massiv[i].Special_Show();
						massiv[j].Special_Show();
						Sleep(pause);
						massiv[i].Show();
						massiv[j].Show();
						Sleep(pause);
					}
					stop = 0;
					break;
				}
			}
		}
		else {
			for (int j = count - 1; j > i && stop; j--) {
				if (massiv[i].can_see == 1 && massiv[j].can_see == 1 && massiv[i].symb == massiv[j].symb) {
					Edit_Score(prices[1], -1);
					for (int l = 0; l < 12; l++) {
						massiv[i].Special_Show();
						massiv[j].Special_Show();
						Sleep(pause);
						massiv[i].Show();
						massiv[j].Show();
						Sleep(pause);
					}
					stop = 0;
					break;
				}
			}
		}
	}
	delete[]random_access;
	if (menu_activ[0] == 0) for (int k = 0; k < count; k++) massiv[k].Set_Viz(1);
}
/* начало новой игры */
void New_Game(int key) {
	SetConsoleTextAttribute(H, 7); system("cls");
	if (key == 1) {
		char path[] = "Maps\\*.txt";
		// объявление указателя fileinfo на структуру _finddata_t
		// и создание динамического объекта структуры _finddata_t
		_finddata_t* fileinfo = new _finddata_t;
		int done = _findfirst(path, fileinfo); // поиск файла (находит 0.txt, а это не игровая раскладка)
		if (done == -1) {
			Stand(1, 1, 12);
			text("О ужас! Кто-то удалил все файлы раскладок!!!", 15);
			Stand(1, 3, 4);
			text("Совет: создайте раскладку в редакторе и поместите файл file.txt в папку Maps!", 15);
			Wind Back("Назад", 2, 45, 4, 7, 12, 15);
			Back.SW();
			//////////////////////////////////////////////////////////////////////////
			DWORD fdwMode, cNumRead;
			INPUT_RECORD irInBuf[128];
			HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
			HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
			fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
			SetConsoleMode(hin, fdwMode);
			bool flags = 0;
			bool www = 1;
			while (www) {
				ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
				for (int i = 0; i < cNumRead; i++) {
					if (irInBuf[i].EventType == MOUSE_EVENT) {
						// наведение и нажатие на кнопки меню
						if (flags == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Back.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Back.xpoz + Back.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Back.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Back.ypoz + Back.heits) {
							Back.SW(0); flags = 1;
						}
						else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Back.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Back.xpoz + Back.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Back.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Back.ypoz + Back.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
							www = 0;
							system("cls");
							Main_Menu();
						}
						else if (flags == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Back.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Back.xpoz + Back.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Back.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Back.ypoz + Back.heits))) {
							Back.SW(); flags = 0;
						}
					}
				}
			}
		}
		else {
			char* add[20];
			int MayWeWork = done;
			int mass[20] = { 0 };
			Stand(21, 5, 15); text("ВЫБЕРИТЕ ОДНУ ИЗ ДОСТУПНЫХ РАСКЛАДОК:");
			int count = 1; // счетчик содержит информацию о количестве найденных файлов.
			while (MayWeWork != -1 && count < 21) {
				char* address = new char[100];
				strcpy(address, "Maps\\");
				strcat(address, fileinfo->name);
				/* char d[100];
				AnsiToOem(address,d);
				cout<<d<<'\n'; // показать имя файла */
				add[count - 1] = new char[strlen(address)];
				strcpy(add[count - 1], address);
				FILE* f = fopen(address, "r+");
				char* string = new char[20];
				fgets(string, 20, f);
				fscanf(f, "%d", &mass[count - 1]);
				//////////////////////////////////////////////////////////////////////////
				Stand(16, count * 2 + 6, 2); if (count < 10) cout << " "; cout << count << ".";
				c.X = 40 - strlen(string) / 2;
				for (c.Y = 47; c.Y > count * 2 + 5; c.Y--) {
					Stand(c.X, c.Y + 1, 7); for (int i = 0; i < strlen(string); i++) cout << " ";
					Stand(c.X, c.Y - 1, 2); text(string);
					Sleep(sl3);
				}
				Stand(63 - Razer(mass[count - 1]), count * 2 + 6, 2); cout << mass[count - 1];
				//////////////////////////////////////////////////////////////////////////
				fclose(f);
				delete[]address;
				delete[]string;
				MayWeWork = _findnext(done, fileinfo); // найти следующий файл из группы
				count++;
			}
			count--;
			sl3 = 1;
			Wind Back("Назад", 2, 45, 4, 7, 12, 15);
			Back.SW();
			Wind Next("Начать!", 68, 45, 1, 7, 9, 15);
			Next.SW();
			//////////////////////////////////////////////////////////////////////////
			DWORD fdwMode, cNumRead;
			INPUT_RECORD irInBuf[128];
			HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
			HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
			fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
			SetConsoleMode(hin, fdwMode);
			bool flags[2] = { 0 };
			int flag = 0;
			bool www = 1;
			while (www) {
				ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
				for (int i = 0; i < cNumRead; i++) {
					if (irInBuf[i].EventType == MOUSE_EVENT) {
						// наведение и нажатие на кнопки меню
						if (flags[0] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Back.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Back.xpoz + Back.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Back.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Back.ypoz + Back.heits) {
							Back.SW(0); flags[0] = 1;
						}
						else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Back.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Back.xpoz + Back.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Back.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Back.ypoz + Back.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
							www = 0;
							system("cls");
							Main_Menu();
						}
						else if (flags[0] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Back.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Back.xpoz + Back.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Back.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Back.ypoz + Back.heits))) {
							Back.SW(); flags[0] = 0;
						}
						else if (flags[1] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Next.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Next.xpoz + Next.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Next.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Next.ypoz + Next.heits) {
							Next.SW(0); flags[1] = 1;
						}
						else if (flag && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Next.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Next.xpoz + Next.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Next.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Next.ypoz + Next.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
							www = 0;
							system("cls");
							global_address = new char[100];
							strcpy(global_address, add[((flag - 6) / 2) - 1]);
							SetConsoleTextAttribute(H, 7); system("cls");
							Mahjong A(-1);
							A.Show_All(15);
							A.Game_Menu();
							A.Mouse_Control();
						}
						else if (flags[1] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Next.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Next.xpoz + Next.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Next.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Next.ypoz + Next.heits))) {
							Next.SW(); flags[1] = 0;
						}
						else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 15 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 63 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>7 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < 47) {
							if (irInBuf[i].Event.MouseEvent.dwMousePosition.Y != flag) {
								Stand(16, irInBuf[i].Event.MouseEvent.dwMousePosition.Y, 0);
								//FillConsoleOutputAttribute(H,14,47,c,0);
							}
							for (c.Y = 8; c.Y < 47; c.Y++) {
								if (c.Y == flag) {
									Stand(16, c.Y, 0);
									//FillConsoleOutputAttribute(H,160,47,c,0);
								}
								else if (c.Y == irInBuf[i].Event.MouseEvent.dwMousePosition.Y) continue;
								else {
									Stand(16, c.Y, 0);
									//FillConsoleOutputAttribute(H,2,47,c,0);
								}
							}
							//////////////////////////////////////////////////////////////////////////
							if (irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
								if (irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 8 && count >= 1 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 10 && count >= 2 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 12 && count >= 3 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 14 && count >= 4 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 16 && count >= 5 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 18 && count >= 6 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 20 && count >= 7 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 22 && count >= 8 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 24 && count >= 9 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 26 && count >= 10 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 28 && count >= 11 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 30 && count >= 12 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 32 && count >= 13 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 34 && count >= 14 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 36 && count >= 15 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 38 && count >= 16 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 40 && count >= 17 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 42 && count >= 18 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 44 && count >= 19 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 46 && count >= 20) {
									flag = irInBuf[i].Event.MouseEvent.dwMousePosition.Y;
								}
							}

							//////////////////////////////////////////////////////////////////////////
						}
						else {
							for (c.Y = 8; c.Y < 47; c.Y++) {
								if (c.Y == flag) continue;
								Stand(16, c.Y, 0); //FillConsoleOutputAttribute(H,2,47,c,0);
							}
						}
					}
				}
			}
		}
		_findclose(done);
		delete fileinfo;
	}
	else {
		char path[] = "User_Maps\\*.txt";
		// объявление указателя fileinfo на структуру _finddata_t
		// и создание динамического объекта структуры _finddata_t
		_finddata_t* fileinfo = new _finddata_t;
		int done = _findfirst(path, fileinfo); // поиск файла (находит 0.txt, а это не игровая раскладка)
		if (done == -1) {
			Stand(1, 1, 12);
			text("О ужас! Кто-то удалил все файлы раскладок!!!", 15);
			Stand(1, 3, 4);
			text("Совет: создайте раскладку в редакторе и поместите file.txt в папку User_Maps!", 15);
			Wind Back("Назад", 2, 45, 4, 7, 12, 15);
			Back.SW();
			//////////////////////////////////////////////////////////////////////////
			DWORD fdwMode, cNumRead;
			INPUT_RECORD irInBuf[128];
			HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
			HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
			fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
			SetConsoleMode(hin, fdwMode);
			bool flags = 0;
			bool www = 1;
			while (www) {
				ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
				for (int i = 0; i < cNumRead; i++) {
					if (irInBuf[i].EventType == MOUSE_EVENT) {
						// наведение и нажатие на кнопки меню
						if (flags == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Back.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Back.xpoz + Back.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Back.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Back.ypoz + Back.heits) {
							Back.SW(0); flags = 1;
						}
						else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Back.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Back.xpoz + Back.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Back.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Back.ypoz + Back.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
							www = 0;
							system("cls");
							Main_Menu();
						}
						else if (flags == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Back.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Back.xpoz + Back.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Back.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Back.ypoz + Back.heits))) {
							Back.SW(); flags = 0;
						}
					}
				}
			}
		}
		else {
			char* add[20];
			int MayWeWork = done;
			int mass[20] = { 0 };
			Stand(21, 5, 15); text("ВЫБЕРИТЕ ОДНУ ИЗ ДОСТУПНЫХ РАСКЛАДОК:");
			int count = 1; // счетчик содержит информацию о количестве найденных файлов.
			while (MayWeWork != -1 && count < 21) {
				char* address = new char[100];
				strcpy(address, "User_Maps\\");
				strcat(address, fileinfo->name);
				/* char d[100];
				AnsiToOem(address,d);
				cout<<d<<'\n'; // показать имя файла */
				add[count - 1] = new char[strlen(address)];
				strcpy(add[count - 1], address);
				FILE* f = fopen(address, "r+");
				char* string = new char[20];
				fgets(string, 20, f);
				fscanf(f, "%d", &mass[count - 1]);
				//////////////////////////////////////////////////////////////////////////
				Stand(16, count * 2 + 6, 4); if (count < 10) cout << " "; cout << count << ".";
				c.X = 40 - strlen(string) / 2;
				for (c.Y = 47; c.Y > count * 2 + 5; c.Y--) {
					Stand(c.X, c.Y + 1, 7); for (int i = 0; i < strlen(string); i++) cout << " ";
					Stand(c.X, c.Y - 1, 4); text(string);
					Sleep(sl3);
				}
				Stand(63 - Razer(mass[count - 1]), count * 2 + 6, 4); cout << mass[count - 1];
				//////////////////////////////////////////////////////////////////////////
				fclose(f);
				delete[]address;
				delete[]string;
				MayWeWork = _findnext(done, fileinfo); // найти следующий файл из группы
				count++;
			}
			count--;
			sl3 = 1;
			Wind Back("Назад", 2, 45, 4, 7, 12, 15);
			Back.SW();
			Wind Next("Начать!", 68, 45, 1, 7, 9, 15);
			Next.SW();
			//////////////////////////////////////////////////////////////////////////
			DWORD fdwMode, cNumRead;
			INPUT_RECORD irInBuf[128];
			HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
			HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
			fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
			SetConsoleMode(hin, fdwMode);
			bool flags[2] = { 0 };
			int flag = 0;
			bool www = 1;
			while (www) {
				ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
				for (int i = 0; i < cNumRead; i++) {
					if (irInBuf[i].EventType == MOUSE_EVENT) {
						// наведение и нажатие на кнопки меню
						if (flags[0] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Back.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Back.xpoz + Back.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Back.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Back.ypoz + Back.heits) {
							Back.SW(0); flags[0] = 1;
						}
						else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Back.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Back.xpoz + Back.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Back.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Back.ypoz + Back.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
							www = 0;
							system("cls");
							Main_Menu();
						}
						else if (flags[0] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Back.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Back.xpoz + Back.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Back.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Back.ypoz + Back.heits))) {
							Back.SW(); flags[0] = 0;
						}
						else if (flags[1] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Next.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Next.xpoz + Next.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Next.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Next.ypoz + Next.heits) {
							Next.SW(0); flags[1] = 1;
						}
						else if (flag && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Next.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Next.xpoz + Next.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Next.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Next.ypoz + Next.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
							www = 0;
							system("cls");
							global_address = new char[100];
							strcpy(global_address, add[((flag - 6) / 2) - 1]);
							SetConsoleTextAttribute(H, 7); system("cls");
							Mahjong A(-1);
							A.Show_All(15);
							A.Game_Menu();
							A.Mouse_Control();
						}
						else if (flags[1] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Next.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Next.xpoz + Next.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Next.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Next.ypoz + Next.heits))) {
							Next.SW(); flags[1] = 0;
						}
						else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > 15 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 63 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>7 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < 47) {
							if (irInBuf[i].Event.MouseEvent.dwMousePosition.Y != flag) {
								Stand(16, irInBuf[i].Event.MouseEvent.dwMousePosition.Y, 0);
								//FillConsoleOutputAttribute(H,14,47,c,0);
							}
							for (c.Y = 8; c.Y < 47; c.Y++) {
								if (c.Y == flag) {
									Stand(16, c.Y, 0);
									//FillConsoleOutputAttribute(H,192,47,c,0);
								}
								else if (c.Y == irInBuf[i].Event.MouseEvent.dwMousePosition.Y) continue;
								else {
									Stand(16, c.Y, 0);
									//FillConsoleOutputAttribute(H,4,47,c,0);
								}
							}
							//////////////////////////////////////////////////////////////////////////
							if (irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
								if (irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 8 && count >= 1 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 10 && count >= 2 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 12 && count >= 3 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 14 && count >= 4 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 16 && count >= 5 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 18 && count >= 6 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 20 && count >= 7 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 22 && count >= 8 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 24 && count >= 9 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 26 && count >= 10 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 28 && count >= 11 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 30 && count >= 12 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 32 && count >= 13 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 34 && count >= 14 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 36 && count >= 15 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 38 && count >= 16 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 40 && count >= 17 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 42 && count >= 18 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 44 && count >= 19 ||
									irInBuf[i].Event.MouseEvent.dwMousePosition.Y == 46 && count >= 20) {
									flag = irInBuf[i].Event.MouseEvent.dwMousePosition.Y;
								}
							}

							//////////////////////////////////////////////////////////////////////////
						}
						else {
							for (c.Y = 8; c.Y < 47; c.Y++) {
								if (c.Y == flag) continue;
								Stand(16, c.Y, 0); //FillConsoleOutputAttribute(H,4,47,c,0);
							}
						}
					}
				}
			}
		}
		_findclose(done);
		delete fileinfo;
	}
}
void New_Game_Menu() {
	Wind Author("Новая  партия", 60, 34, 1, 7, 9, 15);
	Author.Left(100, 34, 45, 1);
	Author.Right(Author.xpoz - 5, 34, 7, 1);
	Author.Left(Author.xpoz + 2, 34, 3, 1);
	Wind Myself("Мои раскладки", 60, 38, 4, 7, 12, 15);
	Myself.Left(100, 38, 45, 1);
	Myself.Right(Myself.xpoz - 5, 38, 7, 1);
	Myself.Left(Myself.xpoz + 2, 38, 3, 1);
	Wind GoBack("    Назад    ", 60, 42, 5, 7, 13, 15);
	GoBack.Left(100, 42, 45, 1);
	GoBack.Right(GoBack.xpoz - 5, 42, 7, 1);
	GoBack.Left(GoBack.xpoz + 2, 42, 3, 1);
	//////////////////////////////////////////////////////////////////////////
	DWORD fdwMode, cNumRead;
	INPUT_RECORD irInBuf[128];
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(hin, fdwMode);
	bool flags[4] = { 0 };
	bool www = 1;
	while (www) {
		ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
		for (int i = 0; i < cNumRead; i++) {
			if (irInBuf[i].EventType == MOUSE_EVENT) {
				// наведение и нажатие на кнопки меню
				if (flags[0] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Author.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Author.xpoz + Author.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Author.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Author.ypoz + Author.heits) {
					Author.SW(0); flags[0] = 1;
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Author.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Author.xpoz + Author.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Author.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Author.ypoz + Author.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Author.Left(Author.xpoz, Author.ypoz, 5, 1);
					Author.Right(Author.xpoz - 6, Author.ypoz, 45, 1);
					Myself.Left(Myself.xpoz, Myself.ypoz, 5, 1);
					Myself.Right(Myself.xpoz - 6, Myself.ypoz, 45, 1);
					GoBack.Left(GoBack.xpoz, GoBack.ypoz, 5, 1);
					GoBack.Right(GoBack.xpoz - 6, GoBack.ypoz, 45, 1);
					www = 0;
					system("cls");
					New_Game(1);
				}
				else if (flags[0] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Author.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Author.xpoz + Author.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Author.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Author.ypoz + Author.heits))) {
					Author.SW(); flags[0] = 0;
				}
				else if (flags[1] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Myself.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Myself.xpoz + Myself.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Myself.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Myself.ypoz + Myself.heits) {
					Myself.SW(0); flags[1] = 1;
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Myself.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Myself.xpoz + Myself.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Myself.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Myself.ypoz + Myself.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Author.Left(Author.xpoz, Author.ypoz, 5, 1);
					Author.Right(Author.xpoz - 6, Author.ypoz, 45, 1);
					Myself.Left(Myself.xpoz, Myself.ypoz, 5, 1);
					Myself.Right(Myself.xpoz - 6, Myself.ypoz, 45, 1);
					GoBack.Left(GoBack.xpoz, GoBack.ypoz, 5, 1);
					GoBack.Right(GoBack.xpoz - 6, GoBack.ypoz, 45, 1);
					www = 0;
					system("cls");
					New_Game(2);
				}
				else if (flags[1] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Myself.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Myself.xpoz + Myself.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Myself.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Myself.ypoz + Myself.heits))) {
					Myself.SW(); flags[1] = 0;
				}
				else if (flags[2] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > GoBack.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<GoBack.xpoz + GoBack.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>GoBack.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < GoBack.ypoz + GoBack.heits) {
					GoBack.SW(0); flags[2] = 1;
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > GoBack.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<GoBack.xpoz + GoBack.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>GoBack.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < GoBack.ypoz + GoBack.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Author.Left(Author.xpoz, Author.ypoz, 5, 1);
					Author.Right(Author.xpoz - 6, Author.ypoz, 45, 1);
					Myself.Left(Myself.xpoz, Myself.ypoz, 5, 1);
					Myself.Right(Myself.xpoz - 6, Myself.ypoz, 45, 1);
					GoBack.Left(GoBack.xpoz, GoBack.ypoz, 5, 1);
					GoBack.Right(GoBack.xpoz - 6, GoBack.ypoz, 45, 1);
					Menu_Choice();
					www = 0;
				}
				else if (flags[2] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= GoBack.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= GoBack.xpoz + GoBack.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= GoBack.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= GoBack.ypoz + GoBack.heits))) {
					GoBack.SW(); flags[2] = 0;
				}
				if (irInBuf[i].Event.MouseEvent.dwMousePosition.X >= 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 80 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < 50) {
					int _c = rand() % 7 + 1;
					for (c.Y = 10; c.Y < 16; c.Y++) {
						Stand(7, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 12; c.Y < 14; c.Y++) {
						Stand(9, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 16; c.Y < 22; c.Y++) {
						Stand(16, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 18; c.Y < 20; c.Y++) {
						Stand(18, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 13; c.Y < 19; c.Y++) {
						Stand(25, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 15; c.Y < 17; c.Y++) {
						Stand(27, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 16; c.Y < 22; c.Y++) {
						Stand(34, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 18; c.Y < 20; c.Y++) {
						Stand(36, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 19; c.Y < 25; c.Y++) {
						Stand(46, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 21; c.Y < 23; c.Y++) {
						Stand(48, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 13; c.Y < 19; c.Y++) {
						Stand(55, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 15; c.Y < 17; c.Y++) {
						Stand(57, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 16; c.Y < 22; c.Y++) {
						Stand(64, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 18; c.Y < 20; c.Y++) {
						Stand(66, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
				}
			}
		}
	}
}
void Help() {
	SetConsoleTextAttribute(H, 7); system("cls");
	Stand(7, 1, 15); text("Пару одинаковых фишек можно снять. При этом снимаются только фишки", 15);
	Stand(1, 3, 15); text("свободные справа или слева, на которых сверху не лежит ни одной другой фишки.", 15);
	Mahjong Hlp(666); Hlp.Visibility(); Hlp.Show_All();
	Window(28, 16, 6, 6, 5); Stand(30, 18, 13); cout << "GG"; Stand(30, 19, 13); cout << "GG";
	Window(64, 13, 6, 6, 5); Stand(66, 15, 13); cout << "GG"; Stand(66, 16, 13); cout << "GG"; Sleep(500);
	Stand(1, 30, 12); text("Кнопка \"ГЛАЗ\" затеняет закрытые фишки.", 15);
	Stand(1, 32, 14); text("Кнопка \"ПЕРЕТАСОВКА\" поможет размешать фишки, если их трудно разобрать.", 15);
	Stand(1, 34, 10); text("Кнопка \"ОТМЕНА\" возвращает на место снятую пару фишек.", 15);
	Stand(1, 36, 9); text("Кнопка \"СОВЕТ\" подсказывает, какие фишки можно снять.", 15);
	Stand(44, 46, 7); text("По заказу Компьютерной Академии ШАГ", 15);
	Stand(5, 48, 8); text("Александр Загоруйко, генеральный программист компании ALEX GAMES, 2007 год", 15);
	Sleep(500);
	Wind Editor("Назад", 5, 40, 4, 7, 12, 15);
	Editor.Right(-Editor.widz, 40, 20, 1);
	Editor.Left(Editor.xpoz + 5, 40, 10, 1);
	Editor.Right(Editor.xpoz - 4, 40, 5, 1);
	DWORD fdwMode, cNumRead;
	INPUT_RECORD irInBuf[128];
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(hin, fdwMode);
	bool flags[4] = { 0 };
	bool www = 1;
	while (www) {
		ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
		for (int i = 0; i < cNumRead; i++) {
			if (irInBuf[i].EventType == MOUSE_EVENT) {
				// наведение и нажатие на кнопки меню
				if (flags[0] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Editor.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Editor.xpoz + Editor.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Editor.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Editor.ypoz + Editor.heits) {
					Editor.SW(0); flags[0] = 1;
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Editor.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Editor.xpoz + Editor.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Editor.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Editor.ypoz + Editor.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Editor.Right(Editor.xpoz, Editor.ypoz, 3, 1);
					Editor.Left(Editor.xpoz + 4, Editor.ypoz, 43, 1);
					www = 0;
					system("cls");
					Main_Menu();
				}
				else if (flags[0] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Editor.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Editor.xpoz + Editor.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Editor.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Editor.ypoz + Editor.heits))) {
					Editor.SW(); flags[0] = 0;
				}
			}
		}
	}
}
void Menu_Choice() {
	Sleep(500);
	Wind Editor("Редактор раскладок", 14, 30, 4, 7, 12, 15);
	Editor.Right(-Editor.widz, 30, 40, 1);
	Editor.Left(Editor.xpoz + 3, 30, 10, 1);
	Editor.Right(Editor.xpoz - 6, 30, 7, 1);
	Wind Rulers("  Правила   игры  ", 14, 34, 6, 7, 14, 15);
	Rulers.Right(-Rulers.widz, 34, 40, 1);
	Rulers.Left(Rulers.xpoz + 3, 34, 10, 1);
	Rulers.Right(Rulers.xpoz - 6, 34, 7, 1);
	Wind StartG("   Начать  игру   ", 14, 38, 2, 7, 10, 15);
	StartG.Right(-StartG.widz, 38, 40, 1);
	StartG.Left(StartG.xpoz + 3, 38, 10, 1);
	StartG.Right(StartG.xpoz - 6, 38, 7, 1);
	Wind BuyBuy("      Выход       ", 14, 42, 1, 7, 9, 15);
	BuyBuy.Right(-BuyBuy.widz, 42, 40, 1);
	BuyBuy.Left(BuyBuy.xpoz + 3, 42, 10, 1);
	BuyBuy.Right(BuyBuy.xpoz - 6, 42, 7, 1);
	//////////////////////////////////////////////////////////////////////////
	DWORD fdwMode, cNumRead;
	INPUT_RECORD irInBuf[128];
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(hin, fdwMode);
	bool flags[4] = { 0 };
	bool www = 1;
	while (www) {
		ReadConsoleInput(hin, irInBuf, 128, &cNumRead);
		for (int i = 0; i < cNumRead; i++) {
			if (irInBuf[i].EventType == MOUSE_EVENT) {
				// наведение и нажатие на кнопки меню
				if (flags[0] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Editor.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Editor.xpoz + Editor.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Editor.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Editor.ypoz + Editor.heits) {
					Editor.SW(0); flags[0] = 1;
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Editor.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Editor.xpoz + Editor.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Editor.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Editor.ypoz + Editor.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Editor.Right(Editor.xpoz, Editor.ypoz, 3, 1);
					Editor.Left(Editor.xpoz + 4, Editor.ypoz, 43, 1);
					Rulers.Right(Rulers.xpoz, Rulers.ypoz, 5, 1);
					Rulers.Left(Rulers.xpoz + 6, Rulers.ypoz, 45, 1);
					StartG.Right(StartG.xpoz, StartG.ypoz, 5, 1);
					StartG.Left(StartG.xpoz + 6, StartG.ypoz, 45, 1);
					BuyBuy.Right(BuyBuy.xpoz, BuyBuy.ypoz, 5, 1);
					BuyBuy.Left(BuyBuy.xpoz + 6, BuyBuy.ypoz, 45, 1);
					www = 0;
					system("cls");
					Edditor();
				}
				else if (flags[0] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Editor.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Editor.xpoz + Editor.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Editor.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Editor.ypoz + Editor.heits))) {
					Editor.SW(); flags[0] = 0;
				}
				else if (flags[1] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > Rulers.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Rulers.xpoz + Rulers.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Rulers.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Rulers.ypoz + Rulers.heits) {
					Rulers.SW(0); flags[1] = 1;
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > Rulers.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<Rulers.xpoz + Rulers.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>Rulers.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < Rulers.ypoz + Rulers.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Editor.Right(Editor.xpoz, Editor.ypoz, 3, 1);
					Editor.Left(Editor.xpoz + 4, Editor.ypoz, 43, 1);
					Rulers.Right(Rulers.xpoz, Rulers.ypoz, 5, 1);
					Rulers.Left(Rulers.xpoz + 6, Rulers.ypoz, 45, 1);
					StartG.Right(StartG.xpoz, StartG.ypoz, 5, 1);
					StartG.Left(StartG.xpoz + 6, StartG.ypoz, 45, 1);
					BuyBuy.Right(BuyBuy.xpoz, BuyBuy.ypoz, 5, 1);
					BuyBuy.Left(BuyBuy.xpoz + 6, BuyBuy.ypoz, 45, 1);
					www = 0;
					Help();
				}
				else if (flags[1] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= Rulers.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= Rulers.xpoz + Rulers.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= Rulers.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= Rulers.ypoz + Rulers.heits))) {
					Rulers.SW(); flags[1] = 0;
				}
				else if (flags[2] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > StartG.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<StartG.xpoz + StartG.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>StartG.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < StartG.ypoz + StartG.heits) {
					StartG.SW(0); flags[2] = 1;
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > StartG.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<StartG.xpoz + StartG.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>StartG.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < StartG.ypoz + StartG.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					Editor.Right(Editor.xpoz, Editor.ypoz, 3, 1);
					Editor.Left(Editor.xpoz + 4, Editor.ypoz, 43, 1);
					Rulers.Right(Rulers.xpoz, Rulers.ypoz, 5, 1);
					Rulers.Left(Rulers.xpoz + 6, Rulers.ypoz, 45, 1);
					StartG.Right(StartG.xpoz, StartG.ypoz, 5, 1);
					StartG.Left(StartG.xpoz + 6, StartG.ypoz, 45, 1);
					BuyBuy.Right(BuyBuy.xpoz, BuyBuy.ypoz, 5, 1);
					BuyBuy.Left(BuyBuy.xpoz + 6, BuyBuy.ypoz, 45, 1);
					www = 0;
					New_Game_Menu();
				}
				else if (flags[2] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= StartG.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= StartG.xpoz + StartG.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= StartG.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= StartG.ypoz + StartG.heits))) {
					StartG.SW(); flags[2] = 0;
				}
				else if (flags[3] == 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X > BuyBuy.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<BuyBuy.xpoz + BuyBuy.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>BuyBuy.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < BuyBuy.ypoz + BuyBuy.heits) {
					BuyBuy.SW(0); flags[3] = 1;
				}
				else if (irInBuf[i].Event.MouseEvent.dwMousePosition.X > BuyBuy.xpoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.X<BuyBuy.xpoz + BuyBuy.widz && irInBuf[i].Event.MouseEvent.dwMousePosition.Y>BuyBuy.ypoz - 1 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < BuyBuy.ypoz + BuyBuy.heits && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && irInBuf[i].Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
					system("cls"); exit(666);
				}
				else if (flags[3] == 1 && ((irInBuf[i].Event.MouseEvent.dwMousePosition.X <= BuyBuy.xpoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.X >= BuyBuy.xpoz + BuyBuy.widz) || (irInBuf[i].Event.MouseEvent.dwMousePosition.Y <= BuyBuy.ypoz - 1 || irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= BuyBuy.ypoz + BuyBuy.heits))) {
					BuyBuy.SW(); flags[3] = 0;
				}
				if (irInBuf[i].Event.MouseEvent.dwMousePosition.X >= 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.X < 80 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y >= 0 && irInBuf[i].Event.MouseEvent.dwMousePosition.Y < 50) {
					int _c = rand() % 7 + 1;
					for (c.Y = 10; c.Y < 16; c.Y++) {
						Stand(7, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 12; c.Y < 14; c.Y++) {
						Stand(9, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 16; c.Y < 22; c.Y++) {
						Stand(16, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 18; c.Y < 20; c.Y++) {
						Stand(18, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 13; c.Y < 19; c.Y++) {
						Stand(25, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 15; c.Y < 17; c.Y++) {
						Stand(27, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 16; c.Y < 22; c.Y++) {
						Stand(34, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 18; c.Y < 20; c.Y++) {
						Stand(36, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 19; c.Y < 25; c.Y++) {
						Stand(46, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 21; c.Y < 23; c.Y++) {
						Stand(48, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 13; c.Y < 19; c.Y++) {
						Stand(55, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 15; c.Y < 17; c.Y++) {
						Stand(57, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
					//
					for (c.Y = 16; c.Y < 22; c.Y++) {
						Stand(64, c.Y, 0); //FillConsoleOutputAttribute(H,_c,6,c,0);
					}
					for (c.Y = 18; c.Y < 20; c.Y++) {
						Stand(66, c.Y, 0); //FillConsoleOutputAttribute(H,_c+8,2,c,0);
					}
				}
			}
		}
	}
}
void Quake(int k) {
	int slip = 20;
	for (int i = 0; i < k; i++) {
		Stand(0, 50, 0); cout << " ";
		Sleep(slip);
		Stand(0, 0, 0); cout << " ";
		Sleep(slip);
	}
	Sleep(200);
}
void Main_Menu() {
	SetConsoleTextAttribute(H, 7); system("cls");
	Mahjong Logo(0);
	Logo.Visibility();
	Logo.Show_All(sl2);
	int col = 12, wcol = 4;
	Window(7, 10, 6, 6, wcol);
	Stand(9, 12, col); text("ММ");
	Stand(9, 13, col); text("ММ");
	Quake(sl2);
	Window(16, 16, 6, 6, wcol);
	Stand(18, 18, col); text("AA");
	Stand(18, 19, col); text("AA");
	Quake(sl2);
	Window(25, 13, 6, 6, wcol);
	Stand(27, 15, col); text("ДД");
	Stand(27, 16, col); text("ДД");
	Quake(sl2);
	Window(34, 16, 6, 6, wcol);
	Stand(36, 18, col); text("ЖЖ");
	Stand(36, 19, col); text("ЖЖ");
	Quake(sl2);
	Window(46, 19, 6, 6, wcol);
	Stand(48, 21, col); text("ОО");
	Stand(48, 22, col); text("ОО");
	Quake(sl2);
	Window(55, 13, 6, 6, wcol);
	Stand(57, 15, col); text("НН");
	Stand(57, 16, col); text("НН");
	Quake(sl2);
	Window(64, 16, 6, 6, wcol);
	Stand(66, 18, col); text("ГГ");
	Stand(66, 19, col); text("ГГ");
	Quake(sl2);
	sl2 = 3, sl = 100; Sleep(300);
	//////////////////////////////////////////////////////////////////////////
	Menu_Choice();
}
//////////////////////////////////////////////////////////////////////////
// ПОЛНОЭКРАННЫЙ РЕЖИМ
//////////////////////////////////////////////////////////////////////////
#define X_CONSOLE_FULLSCREEN_MODE 0x01
#define X_CONSOLE_WINDOW_MODE 0x02
BOOL __stdcall fnExChangeConsoleDisplayMode(
	HANDLE hConsoleOutput, //дескриптор буфера консоли (HANDLE), для которого применяется новый режим
	unsigned char btMode //режим отображения :: (1)-полноэранный, (2)-оконный
) {
	HMODULE hLibrary = ::LoadLibrary("kernel32.dll");
	if (hLibrary == NULL) return(FALSE);
	typedef int(__stdcall* LP_FUNC_MODULE)(HANDLE, DWORD, COORD*);
	LP_FUNC_MODULE lpThisFunction = (LP_FUNC_MODULE)::GetProcAddress(hLibrary, "SetConsoleDisplayMode");
	if (lpThisFunction == NULL) {
		::FreeLibrary(hLibrary);
		return(FALSE);
	}
	::SetLastError(0);
	BOOL bFSuccess = lpThisFunction(hConsoleOutput, ((DWORD)(btMode)), NULL);
	::FreeLibrary(hLibrary);
	return(bFSuccess);
}
////////////////////////////////////////////////////////////////////////////////
void main() {
	//включение полноэкранного режима
	fnExChangeConsoleDisplayMode(::GetStdHandle(STD_OUTPUT_HANDLE), X_CONSOLE_FULLSCREEN_MODE);
	// отменить показ мигающего курсора
	CONSOLE_CURSOR_INFO cci; cci.bVisible = 0; cci.dwSize = 100; SetConsoleCursorInfo(H, &cci);
	srand(time(NULL));
	Sleep(2000);
	Main_Menu();
}