#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;
enum Settings
{
	BackColor = 0,
	ForeColor = 15,
	SelectBackColor = 2,
	SelectForeColor = 15,
	LeftMenu = 45,
	TopMenu = 5,
	WIDTH_SCREEN = 1000,
	HEIGHT_SCREEN = 500
};

void SetColor(unsigned short backColor, unsigned short foreColor) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD color = ((backColor & 0x0F) << 4) + (foreColor & 0x0F);
	SetConsoleTextAttribute(h, color);
}
void SetCursor(short row, short col) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { col ,row };
	SetConsoleCursorPosition(h, coord);
}
void DrawMenu(const char* textMenu[], size_t sizeMenu, size_t selected = 0) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND hwndConsole = GetConsoleWindow();
	RECT rect;
	GetWindowRect(hwndConsole, &rect);
	MoveWindow(hwndConsole, rect.left, rect.top, Settings::WIDTH_SCREEN, Settings::HEIGHT_SCREEN, FALSE);

	SetColor(Settings::BackColor, Settings::ForeColor);
	SetCursor(Settings::TopMenu, Settings::LeftMenu);
	cout << "---------- MENU ----------";
	for (short i = 0; i < sizeMenu; i++)
	{
		SetCursor(Settings::TopMenu + i + 1, Settings::LeftMenu);
		cout << (i == selected ? SetColor(Settings::SelectBackColor, Settings::SelectForeColor), "==>" : "   ");
		cout << textMenu[i];
		SetColor(Settings::BackColor, Settings::ForeColor);
	}
	cout << endl;
}

size_t Menu(const char* textMenu[], size_t sizeMenu, size_t selected = 0) {

	unsigned char key = 72;
	while (true)
	{
		if (key == 72 or key == 80)
			DrawMenu(textMenu, sizeMenu, selected);
		key = _getch();

		switch (key) {
		case 13: return selected;
		case 72:
			if (selected == 0)
				selected = sizeMenu;
			--selected;
			break;
		case 80:
			++selected;
			if (selected == sizeMenu) selected = 0;
			break;
		default:
			break;
		}
	}

	return selected;
}

class Gallows
{
public:
	Gallows() :position_letters(6)
	{

	};
	void readFile(const string& filename) {
		srand(time(NULL));
		vector<string> content;
		ifstream file(filename);

		if (!file.is_open()) {
			cout << "Unable to open file " << filename << endl;
			return;
		}
		while (file)
		{
			string temp;
			getline(file, temp);
			content.push_back(decrypted(temp));
		}
		size_t index = 0 + rand() % content.size();
		word = content[index];
		file.close();
	}
	void writeFile(const string& filename, const vector<string>& vec) {
		ofstream file(filename);

		if (!file.is_open()) {
			cout << "Unable to open file " << filename << " for writing" << endl;
			return;
		}
		for (size_t i = 0; i < vec.size(); i++)
		{

			file << encrypt(vec[i]) << endl;

		}
		file.close();
	}

	string encrypt(const string& word) {
		string encrypted;
		for (char c : word) {
			encrypted += c + 1;
		}
		return encrypted;
	}

	string decrypted(const string& word) {
		string decrypted;
		for (char c : word) {
			decrypted += c - 1;
		}
		return decrypted;
	}

	void drawWallows()
	{

		SetCursor(0, 0);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		cout << string(13, ' ') << string(12, '-') << "|" << endl;
		for (short i = 0; i < 15; i++)
			cout << string(25, ' ') << '|' << endl;

		for (short i = 0; i < 3; i++)
		{
			SetCursor(1 + i, 0);
			cout << string(13, ' ') << '|' << endl;
		}

		switch (CountOfAttempts) {
		case 6: // Ліва нога
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
			for (short i = 0; i < 2; i++) {
				SetCursor(10 + i, 12 - i);
				cout << '/';
			}
		case 5: // Права нога
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
			for (short i = 0; i < 2; i++) {
				SetCursor(10 + i, 14 + i);
				cout << '\\';
			}
		case 4: // Ліва рука
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
			for (short i = 0; i < 2; i++) {
				SetCursor(8 - i, 12 - i);
				cout << '\\';
			}
		case 3: // Права рука
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
			for (short i = 0; i < 2; i++) {
				SetCursor(8 - i, 14 + i);
				cout << '/';
			}
		case 2: // Тулуб
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			for (short i = 0; i < 3; i++) {
				SetCursor(7 + i, 13);
				cout << '|';
			}
		case 1: // Голова
			SetCursor(4, 13);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
			cout << '*';
			SetCursor(5, 12);
			cout << "* *";
			SetCursor(6, 13);
			cout << '*';
			break;
		default:

			break;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		SetCursor(20, 0);
	}
	void Clear()
	{
		CountOfAttempts = 0;
		CountOfCorrect = 2;
		mistakes_letters.clear();
		for (size_t i = 0; i < word.size(); i++)
			position_letters[i] = false;
		word.clear();
	}
	void ShowWord()
	{
		cout << "Your word: ";
		for (size_t i = 0; i < word.size(); i++)
			cout << (position_letters[i] ? word[i] : '_') << " ";
		cout << endl;
	}
	void ShowMistakes()
	{
		cout << "Mistakes(" << CountOfAttempts << "): ";
		for (char c : mistakes_letters)
			cout << c << " ";
		cout << endl;
	}
	void EndGame()
	{
		SetCursor(2, 36);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
		cout << "Word: " << word << endl;

		SetCursor(3, 36);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		ShowWord();

		SetCursor(4, 36);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		ShowMistakes();
		SetCursor(25, 0);

	}
	void game()
	{


		auto start = steady_clock::now();
		position_letters[0] = true;
		position_letters[word.size() - 1] = true;
		while (true)
		{
			SetCursor(20, 0);
			drawWallows();
			ShowWord();
			char letter;
			cout << "Enter your letter: "; cin >> letter;
			cin.clear();
			cin.ignore();
			bool found = false;
			for (size_t i = 1; i < word.size() - 1; i++) {
				if (letter == word[i]) {
					position_letters[i] = true;
					found = true;
					CountOfCorrect++;
				}
			}


			if (!found) {
				mistakes_letters.push_back(letter);
				++CountOfAttempts;
			}

			ShowMistakes();

			(void)_getch();
			system("cls");
			if (CountOfCorrect == 6)
			{
				auto end = steady_clock::now();
				auto duration = duration_cast<seconds>(end - start);
				drawWallows();
				SetCursor(0, 42);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				cout << "You win!!!" << endl;
				SetCursor(1, 36);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "Execution time: " << duration.count() << " sec" << endl;
				EndGame();

				break;
			}
			if (CountOfAttempts == 6)
			{
				auto end = steady_clock::now();
				auto duration = duration_cast<seconds>(end - start);
				drawWallows();
				SetCursor(0, 42);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				cout << "You lose!!!" << endl;
				SetCursor(1, 36);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "Execution time: " << duration.count() << " sec" << endl;
				EndGame();

				break;
			}
		}
		(void)_getch();
		return;
	}
	void Rules()
	{
		cout << "  " << string((55), '-') << "Rules" << string(55, '-') << "\n\n"
			<< "Комп’ютер навмання вибирає слово, малює на екрані першу та останню літери слова й позначає місця для інших букв,\nнаприклад, рисками."
			<< "Також малює шибеницю з петлею."
			<< "Слово буде іменником, загальною назвою, у називному відмінку однини,\n"
			<< "Твоя задача пропонувати літери які можуть входити в це слово.Якщо така літера є в слові, то комп’ютер пише\nїї над відповідними рисками — стільки разів, скільки вона зустрічається в слові.\n"
			<< "Якщо такої літери немає, то до шибениці домальовують коло в петлі, що зображає голову.\n"
			<< "Ти повинен продовжувати відгадувати літери доти, доки не відгадає все слово. За кожну неправильну відповідь комп’ютер\nдодає одну частину тулуба до шибениці."
			<< "Якщо тулуб у шибениці намальований повністю,\nто гравець, що відгадував, вважається повішеним.Якщо гравцеві вдається вгадати слово, він виграє.\n";
	}
	void setCountOfAttempts(size_t num) { CountOfAttempts = num; }
private:

	size_t CountOfAttempts = 0;
	size_t CountOfCorrect = 2;
	string word;
	vector<bool> position_letters;
	vector<char> mistakes_letters;

};
void main_menu(Gallows& game)
{
	const char* textMenu[]{
	  "Exit                        ",
	  "Rules                       ",
	  "drawWallows                 ",
	  "game                        ",
	};
	size_t sizeMenu = sizeof(textMenu) / sizeof(*textMenu);
	SetColor(Settings::BackColor, Settings::ForeColor);
	size_t selected = 1;
	do
	{
		system("cls");
		selected = Menu(textMenu, sizeMenu, selected);
		system("cls");
		switch (selected)
		{
		case 0:
			return;
		case 1:
			game.Rules();
			(void)_getch();
			break;
		case 2:
			game.setCountOfAttempts(6);
			game.drawWallows();
			break;
		case 3:
			game.game();
			game.Clear();
			game.readFile("words.txt");
			break;
		}

		cout << "\nDone\n";
		(void)_getch();
	} while (true);
}




int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	setlocale(LC_ALL, "Ukrainian");
	vector<string> words({ "Mother", "Circle", "Garden", "Rocket", "Window", "Forest", "Action", "Credit", "Export" });
	vector<string> words2({ "ангіна", "абаван", "анемія", "анкета", "анонім", "анчоус", "байкар" });
	vector<string> words3({ "ангіна" });
	Gallows obj;
	obj.writeFile("words.txt", words);
	obj.readFile("words.txt");
	main_menu(obj);

}


