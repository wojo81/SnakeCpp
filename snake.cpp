#include <iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class Game;
class HighScore;

class Game
{
	public:
		Game() : exit(false), end(false), diff(1), dWall(1) {}
		void getHighs();
		void disMen();
		void strtBrd();
		void disBrd();
		void logic();
		void endGame();
		bool checkEnd() {return end;}
		bool checkExit() {return exit;}
	private:
		bool exit;
		bool end;
		bool dWall;
		void newFood();
		void move();
		void disHighs();
		int checkHigh();
		void addHigh(int);
		int diff;
		int dire;
		int score;
		int speed;
		COORD head;
		COORD food;
		COORD last;
		vector <COORD> tail;
		vector <HighScore> highs[3][2];
		static const string DIFFOPT[3];
		static const string WALLOPT[2];
};

const string Game::DIFFOPT[3] = {"Hard", "Medium", "Easy"};
const string Game::WALLOPT[2] = {"On", "Off"};

class HighScore
{
	public:
		HighScore() : hScore(0), name(""), color(7) {}
		HighScore(int s, int c, string n) : hScore(s), color(c), name(n) {}
		void set(int s, int c, string n) {hScore = s; color = c; name = n;}
		void setColor(int c) {color = c;}
		int getScore() {return hScore;}
		int getColor() {return color;}
		string getName() {return name;}
	private:
		int color;
		int hScore;
		string name;
};

void gotoxy(int, int);
void gotoxy(COORD);
void putCenter(string, int);
void ShowConsoleCursor(bool);
void disControls();
void disInstructions();

int main()
{
	Game exec;
	srand(time(0));
	ShowConsoleCursor(false);
	disInstructions();
	disControls();
	exec.getHighs();
	exec.disMen();
	while(!exec.checkExit())
	{
		exec.strtBrd();
		while(!exec.checkEnd())
		{
			exec.disBrd();
			exec.logic();
		}//end while
		exec.disMen();
 	}//end while
	return 0;
}//end of main function

void gotoxy(COORD p)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}//end of gotoxy function

void gotoxy(int posX, int posY)
{
	COORD p;
	p.X = posX;
	p.Y = posY;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}//end of gotoxy function

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}//end of ShowConsole Cursor function

void putCenter(string str, int y)
{
	gotoxy(40 - str.length()/2, y);
	cout << str;
}//end of putCenter function

void disControls()
{
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	putCenter("Controls", 4);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	putCenter("WASD keys to move / navigate menu / change options", 7);
	putCenter("SPACEBAR or ENTER key to pause game / enter menu option / change options", 9);
	putCenter("ENTER key to finsih high score input", 11);
	putCenter("ESCAPE key to end game / exit program / cancel high score input", 13);
	system("pause>nul");
}//end of disControls function

void disInstructions()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	putCenter("Instructions", 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	putCenter("You are the head of the snake (light green O).", 5);
	putCenter("Try to get as high of a score as possible by collecting food (red x's).", 7);
	putCenter("Getting food will also increase your tail size.", 9);
	putCenter("The Difficulty option affects your speed and how much your tail grows.", 11);
	putCenter("If you touch your tail (green o's) then the game ends.", 13);
	putCenter("If the option: Deadly Walls is on and you touch the wall (blue #'s)", 15);
	putCenter("then the game will also end.", 17);
	putCenter("Otherwise, you will be teleported to the other side of the map.", 19);
	putCenter("If your score is high enough then your score can be added to the scoreboard.", 21);
	system("pause>nul");
}//end of disInstructions function

void Game::getHighs()
{
	ifstream inFile("highscores.txt");

	if(inFile.is_open())
	{
		HighScore inputHigh;
		int inDiff;
		int inWall;
		int inScore;
		int inColor;
		string inName;
		inFile.ignore();
		while(!inFile.eof())
		{
			inFile >> inDiff;
			inFile.ignore();
			inFile >> inWall;
			inFile.ignore();

			inFile >> inScore;
			inFile.ignore();
			
			getline(inFile, inName, '#');
			
			inFile >> inColor;
			inFile.ignore();

			inputHigh.set(inScore, inColor, inName);
			highs[inDiff][inWall].push_back(inputHigh);
		}
		inFile.close();
	}//end if
}//end of getHighs function

void Game::disMen()
{
	const string OPT[6] = {"Play", "Difficulty: ", "Deadly Walls: ", "Display Scoreboard", "Controls", "Exit"};
	bool con = true;
	bool change = false;
	int opt  = 0;
	char key;
	while(con)
	{
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		putCenter("Snake", 4);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		for(int i = 0; i < 6; i++)
		{
			if(i == 1)
				putCenter(OPT[i] + DIFFOPT[diff], 7 + 2 * i);
			else if(i == 2)
				putCenter(OPT[i] + WALLOPT[dWall], 7 + 2 * i);
			else
				putCenter(OPT[i], 7 + 2 * i);
			//end if
			if(i == opt)
			{
				cout << " ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 * 16);
				cout << " ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}//end if
		}//end for
		change = false;
		while(!change)
		{
			Sleep(100);
			char key = _getch();
			key = tolower(key);
			switch (key)
			{
				case 's':
				{
					change = true;
					if(opt != 5)
						opt++;
					else
						opt = 0;
					break;
				}
				case 'w':
				{
					change = true;
					if(opt)
						opt--;
					else
						opt = 5;
					break;
				}
				case 'd':
				{
					if(opt == 1)
					{
						change = true;
						diff--;
						if(diff < 0)
							diff = 2;
					}
					else if(opt == 2)
					{
						change = true;
						dWall = 1 - dWall;
					}//end if
					break;
				}
				case 'a':
				{
					if(opt == 1)
					{
						change = true;
						diff++;
						if(diff > 2)
							diff = 0;
					}
					else if(opt == 2)
					{
						change = true;
						dWall = 1 - dWall;
					}//end if
					break;
				}
				case ' ':
				case '\r':
				{
					switch(opt)
					{
						case 0:
						{
							change = true;
							con = false;
							break;
						}
						case 1:
						{
							change = true;
							diff--;
							if(diff < 0)
								diff = 2;
							break;
						}
						case 2:
						{
							change = true;
							dWall = 1 - dWall;
							break;
						}
						case 3:
						{
							change = true;
							disHighs();
							system("pause>nul");
							break;
						}
						case 4:
						{
							change = true;
							disControls();
							break;
						}
						case 5:
						{
							change = true;
							con = false;
							exit = true;
							break;
						}
					}//end switch
					break;
				}
				case 27:
				{
					change = true;
					con = false;
					exit = true;
					break;
				}
			}//end switch
		}//end while
	}//end while
}//end of disMen function

void Game::strtBrd()
{
	system("cls");
	end = false;
	for(int y = 0; y < 25; y++)
	{
		for(int x = 0; x < 79; x++)
		{
			if(!(x % 78) || !(y % 24))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9 * 16);
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
			}
			cout << " ";
			//end if
		}//end for
		cout << endl;
	}//end for
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(2, 0);
	cout << "--SNAKE--";
	gotoxy(12, 0);
	cout << "Score: 0";
	gotoxy(2, 24);
	score = 0;
	cout << "High Score: " << checkHigh();
	speed = (diff + 7) * 5;
	dire = 0;
	head.X = 39;
	head.Y = 12;
	newFood();
}//end of strtBrd function

void Game::newFood()
{
	int randX;
	int randY;
	
	do
	{
		randX = rand() % 77 + 1;
		randY = rand() % 23 + 1;
	}
	while(randX == head.X && randY == head.Y);
	
	food.X = randX;
	food.Y = randY;
	
	bool setFood = true;
	for(int i = 0; i < tail.size() && setFood; i++)
	{
		if(food.X == tail[i].X && food.Y == tail[i].Y)
			setFood = false;
		//end if
	}//end for
	
	if(setFood)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12 * 16);
		gotoxy(food);
		cout << " ";
	}//end if
}

void Game::disBrd()
{
	gotoxy(last);
	if(food.X == last.X && food.Y == last.Y)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12 * 16);
		cout << " ";
	}
	else if(last.X && last.Y)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		cout << " ";
	}//end if
	
	if(tail.size())
	{
		if(tail[0].X && tail[0].Y)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2 * 16);
			gotoxy(tail[0]);
			cout << " ";
		}
	}//end if
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 * 16);
	gotoxy(head);
	cout << " ";
}//end of disBrd function

void Game::move()
{
	switch(dire)
	{
		case 1:
		{
			head.X--;
			break;
		}
		case 2:
		{
			head.Y++;
			break;
		}
		case 3:
		{
			head.X++;
			break;
		}
		case 4:
		{
			head.Y--;
			break;
		}
	}//end switch
}//end of move function

void Game::logic()
{
	if(tail.size())
	{
		last = tail[tail.size() - 1];
		COORD lastT = tail[0];
		COORD last2T;
		tail[0] = head;
		for(int i = 1; i < tail.size(); i++)
		{
			last2T = tail[i];
			tail[i] = lastT;
			lastT = last2T;
		}//end for
	}
	else
		last = head;
	//end if
	
 	char key;
	int newDire = 0;
	if(_kbhit())
	{
		key = _getch();
		key = tolower(key);
		switch (key)
		{
			case 'a':
			{
				newDire = 1;
				break;
			}
			case 's':
			{
				newDire = 2;
				break;
			}
			case 'd':
			{
				newDire = 3;
				break;
			}
			case 'w':
			{
				newDire = 4;
				break;
			}
			case ' ':
			case '\r':
			{
				gotoxy(69, 0);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "-PAUSED-";
				char newKey = _getch();
				while(newKey != ' ' && newKey != '\r')
					newKey = _getch();
				//end while
				gotoxy(69, 0);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9 * 16);
				cout << "        ";
				break;
			}
			case 27:
			{
				endGame();
				return;
			}
		}//end switch
	}//end if
	if((abs(dire - newDire) != 2 || !dire) && newDire)
		dire = newDire;
	//end if
	move();

	if(!dWall)
	{
		if(!(head.X % 78) || !(head.Y % 24))
		{
			endGame();
		}//end if
	}
	else
	{
		if(!head.X) head.X = 77;
		else if(!head.Y) head.Y = 23;
		else if(head.X == 78) head.X = 1;
		else if(head.Y == 24) head.Y = 1;
	}//end if
	
	if(head.X == food.X && head.Y == food.Y)
	{
		newFood();
		score += 10;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(19, 0);
		cout << score;
		if(score > checkHigh())
		{
			gotoxy(14, 24);
			cout << score;
		}//end if
		COORD p;
		p.X = 0; p.Y = 0;
		for(int i = 0; i < 5 - diff; i++)
			tail.push_back(p);
		//end for
	}//end if
	
	for(int i = 0; i < tail.size() && !end; i++)
	{
		if(head.X == tail[i].X && head.Y == tail[i].Y)
			endGame();
		//end if
	}//end for
	
	Sleep(speed);
}//end of logic function

int Game::checkHigh()
{
	if(highs[diff][dWall].size())
		return highs[diff][dWall][0].getScore();
	else
		return 0;
}//end of checkHigh function

void Game::endGame()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	putCenter("GAME OVER", 12);
	Sleep(250);
	system("pause>nul");
	if(score)
	{
		if(highs[diff][dWall].size())
		{
			bool con = true;
			for(int i = 0; i < highs[diff][dWall].size() && con; i++)
			{
				if(score > highs[diff][dWall][i].getScore())
				{
					addHigh(i);
					con = false;
				}//end if
			}//end for
			if(con)
			{
				if(highs[diff][dWall].size() < 10)
					addHigh(highs[diff][dWall].size());
				else
					disHighs();
				//end if
			}//end if
		}
		else
			addHigh(0);
		//end if
	}
	else
		disHighs();
	//end if
	tail.clear();
	end = true;
	system("pause>nul");
}//end of endGame function

void Game::disHighs()
{
	system("cls");
	for(int y = 0; y < 25; y++)
	{
		for(int x = 0; x < 79; x++)
		{
			if(!(x % 26) || !(y % 12))
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9 * 16);
			else
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
			//end if
			cout << " ";
		}//end for
		cout << endl;
	}//end for
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(2, 0);
	cout << "--HIGH SCORES--";
	for(int y = 0; y < 2; y++)
	{
		for(int x = 0; x < 3; x++)
		{
			gotoxy(26 * x + 1, 12 * y + 1);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			cout << "Diff: " << DIFFOPT[x] << " " << "DWalls: " << WALLOPT[y];
			if(diff == x && dWall == y)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 * 16);
				cout << " ";
			}//end if
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			for(int i = 0; i < 10; i++)
			{
				gotoxy(26 * x + 2, 12 * y + 2 + i);
				if(i < highs[x][y].size())
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), highs[x][y][i].getColor());
					cout << left << setw(4) << i+1 << setw(5) << highs[x][y][i].getScore() << right << setw(14) << highs[x][y][i].getName();
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
				{
					cout << left << setw(4) << i+1 << "----" << right << setw(15) << "-------------";
				}//end if
			}//end for
		}//end for
	}//end for
}//end of disHighs function

void Game::addHigh(int pos)
{
	char key;
	string inName;
	int textColor = 7;
	int backColor = 0;
	putCenter("NEW HIGH SCORE", 12);
	Sleep(500);
	HighScore inHigh(score, 7, "");
	highs[diff][dWall].insert(highs[diff][dWall].begin()+pos, inHigh);
	disHighs();
	bool con = true;
	bool change = false;
	ShowConsoleCursor(true);
	int y = 12 * dWall + 2 + pos;
	while(con)
	{
		gotoxy(26 * (diff + 1) - 2, y);
		if(!change)
			key = _getch();
		switch(key)
		{
			case '\b':
			{
				if(inName.length() > 0)
				{
					inName.erase(inName.length() - 1, 1);
					change = true;
				}
				break;
			}
			case 27:
			{
				highs[diff][dWall].erase(highs[diff][dWall].begin()+pos);
				con = false;
				break;
			}
			case ']':
			{
				textColor++;
				if(textColor == 16)
					textColor = 0;
				change = true;
				break;
			}
			case '[':
			{
				textColor--;
				if(textColor == -1)
					textColor = 15;
				change = true;
				break;
			}
			case '}':
			{
				backColor++;
				if(backColor == 16)
					backColor = 0;
				change = true;
				break;
			}
			case '{':
			{
				backColor--;
				if(backColor == -1)
					backColor = 15;
				change = true;
				break;
			}
			case '\r':
			{
				highs[diff][dWall][pos].set(score, textColor + backColor * 16, inName);
				if(highs[diff][dWall].size() > 10)
					highs[diff][dWall].pop_back();
				ofstream outFile("highscores.txt");
				for(int x = 0; x < 3; x++)
				{
					for(int y = 0; y < 2; y++)
					{
						for(int i = 0; i < highs[x][y].size(); i++)
						{
							outFile << endl << x << "#" << y << "#" << highs[x][y][i].getScore() << "#" << highs[x][y][i].getName() << "#" << highs[x][y][i].getColor();
						}//end for
					}//end for
				}//end for
				outFile.close();
				con = false;
				break;
			}
			default:
			{
				if(inName.length() < 13 && key != '\t')
				{
					inName += key;
					change = true;
				}//end if
				break;
			}
		}//end switch
		if(change)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor + backColor * 16);
			gotoxy(26 * diff + 1, y);
			cout << " " << left << setw(4) << pos+1 << setw(5) << highs[diff][dWall][pos].getScore() << right << setw(14) << inName << " ";
			if(textColor != backColor)
				change = false;
			//end if
		}
	}//end while
	ShowConsoleCursor(false);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}//end of addHigh function
