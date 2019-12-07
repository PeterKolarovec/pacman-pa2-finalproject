#include <iostream>
#include <ncurses.h>
#include <string>
#include <fstream>
#include "map.h"
#include "tiles.h"
#include "pacman.h"
#include "ghosts.h"

#define ENDGAME -2
#define GAMEOVER -3

using namespace std;
int playdiff;
int rows = 0,
	cols = 0;
bool ModeFrightened = false;
bool ModeScatter = false;
int ghostPos[8];

void BannerPrint(const string & banner){
	int x = 0,
		y = 1;
	
	string line;
	ifstream logo;
	logo.open(banner);
	while(!logo.eof()){
		getline(logo,line);
		x = cols/2 - (int)line.length()/2;
		mvaddstr(y++,x,line.c_str());
	}
	logo.close();
}

void newgame(){
	string diff = "Choose your difficulty:";
	
	BannerPrint("logo.txt");
		
	//Difficulty
	mvwaddstr(stdscr,rows/2+4,cols/2-(int)diff.length()/2,diff.c_str());
	WINDOW * diffwin = subwin(stdscr,5,10,rows/2+5,cols/2-5);
	box(diffwin,'+','+');
	keypad(diffwin,true);
	wrefresh(stdscr);
	
	string difficulty[3] = {"easy", "normal", "hard"};
	int choice;
	int highlight = 0;
	
	for(;;){
		for (int i = 0; i < 3; ++i) {
			if (i == highlight)
				wattron(diffwin, A_STANDOUT);
			mvwprintw(diffwin,i+1,1,difficulty[i].c_str());
			wattroff(diffwin,A_STANDOUT);
		}
		choice = wgetch(diffwin);
		
		switch (choice){
			case KEY_UP:
				highlight--;
				if(highlight == -1)
					highlight = 0;
				break;
			case KEY_DOWN:
				highlight++;
				if(highlight == 3)
					highlight = 2;
				break;
			default:
				break;
		}
		if (choice == 10)
			break;
	}
	playdiff = highlight;
}

void loadmap(string s_map, Map & map){
	int x = 0,
		y = 0;
	
	clear();
	map.Resize(rows,cols);
	string loc = "examples/";
	loc.append(s_map);
		
	string line;
	char ch;
	int ghost = 0;
	ifstream level;
	level.open(loc.c_str());
	
	if (!level.fail()) {
		while (level.get(ch)) {
			switch (ch) {
				case '=':
					map.Add(TopWall(y, x), y, x);
					x++;
					break;
				case '|':
					map.Add(Wall(y, x), y, x);
					x++;
					break;
				case '+':
					map.Add(PWall(y, x), y, x);
					x++;
					break;
				case '.':
					map.Add(Coin(y, x), y, x);
					map.IncCoinCnt();
					x++;
					break;
				case 'T':
					map.Add(Teleport(y, x), y, x);
					x++;
					break;
				case 'B':
					map.Add(Bonus(y, x), y, x);
					map.IncBonusCnt();
					x++;
					break;
				case 'F':
					map.Add(Frightened(y, x), y, x);
					map.IncFrigCnt();
					x++;
					break;
				case ' ':
					map.Add(Empty(y, x), y, x);
					x++;
					break;
				case '\n':
					x = 0;
					y++;
					break;
				case 'G':
					map.Add(Empty(y, x), y, x);
					ghostPos[ghost] = y;
					ghostPos[ghost+1] = x;
					x++;
					ghost+=2;
					break;
				default:
					throw 1;
					break;
					
			}
		}
	}
	else{
		throw 2;
		return;
	}
}


void endgame(){
	clear();
	BannerPrint("win.txt");
	getch();
}
void gameover(){
	clear();
	BannerPrint("gameover.txt");
	getch();
}

void update(const Pacman & pacman, WINDOW * winscore){
	int score = 0;
	int lives = 0;
	
	score = pacman.GetScore();
	string showsc = to_string(score);
	lives = pacman.GetLife();
	string showlv = to_string(lives);
	mvwprintw(winscore,0,8,showsc.c_str());
	mvwprintw(winscore,1,8,showlv.c_str());
	
	wrefresh(stdscr);
	wrefresh(winscore);
}

void play(Map & map, Ghost & ghost1, Ghost & ghost2, Ghost & ghost3, Ghost & ghost4){
	Pacman pacman;
	halfdelay(2);
	
	int ret;
	int PacmanX, PacmanY;
	int G1X = 0, G1Y= 0;
	int G2X = 0, G2Y = 0;
	int G3X = 0, G3Y = 0;
	int G4X = 0, G4Y = 0;
	int ScatterCounter = 0;
	int FrigCounter = 0;
	
	pacman.Redraw();
	ghost1.Redraw();
	ghost2.Redraw();
	ghost3.Redraw();
	ghost4.Redraw();
	
	getch();
		
	WINDOW * winscore = newwin(2, 12, 0, 39);
	mvwaddstr(winscore,0,0,"Score: " );
	mvwaddstr(winscore,1,0,"Lives: " );
		
	while (pacman.GetLife() > 0){
		int gamemode;
		update(pacman, winscore);
		
		if ((ret = pacman.Move(map,PacmanX,PacmanY, gamemode)) == ENDGAME)
			endgame();
		
		if (gamemode == 1) {
			ModeFrightened = true;
			while (ModeFrightened) {
				update(pacman, winscore);
				
				if ((ret = pacman.Move(map, PacmanX, PacmanY, gamemode)) == ENDGAME) {
					endgame();
					pacman.Quit();
					ghost1.Quit();
					ghost2.Quit();
					ghost3.Quit();
					ghost4.Quit();
				}
				
				if (PacmanX == G1X && PacmanY == G1Y) {
					ghost1.Reset();
					pacman.Bonus();
				}
				else if (PacmanX == G2X && PacmanY == G2Y){
					ghost2.Reset();
					pacman.Bonus();
				}
				else if (PacmanX == G3X && PacmanY == G3Y){
					ghost3.Reset();
					pacman.Bonus();
				}
				else if (PacmanX == G4X && PacmanY == G4Y){
					ghost4.Reset();
					pacman.Bonus();
				}
				
				ghost1.Frightened(map, G1X, G1Y, pacman);
				
				ghost2.Frightened(map, G2X, G2Y, pacman);
				
				ghost3.Frightened(map, G3X, G3Y, pacman);
				
				ghost4.Frightened(map, G4X, G4Y, pacman);
				
				if (PacmanX == G1X && PacmanY == G1Y) {
					ghost1.Reset();
					pacman.Bonus();
				}
				else if (PacmanX == G2X && PacmanY == G2Y){
					ghost2.Reset();
					pacman.Bonus();
				}
				else if (PacmanX == G3X && PacmanY == G3Y){
					ghost3.Reset();
					pacman.Bonus();
				}
				else if (PacmanX == G4X && PacmanY == G4Y){
					ghost4.Reset();
					pacman.Bonus();
				}
				
				FrigCounter++;
				if (FrigCounter == 30) {
					FrigCounter = 0;
					ModeFrightened = false;
				}
				if (gamemode == 1)
					FrigCounter = 0;
			}
		}
		
		if ((PacmanX == G1X && PacmanY == G1Y) || (PacmanX == G2X && PacmanY == G2Y) ||
		    (PacmanX == G3X && PacmanY == G3Y) || (PacmanX == G4X && PacmanY == G4Y)) {
			pacman.LostLife();
			pacman.Reset();
			ghost1.Reset();
			ghost2.Reset();
			ghost3.Reset();
			ghost4.Reset();
			continue;
		}
		
		if (ScatterCounter % 30 != 0 || ScatterCounter == 0) {
			ghost1.Move(map, G1X, G1Y, pacman);
			ghost2.Move(map, G2X, G2Y, pacman);
			ghost3.Move(map, G3X, G3Y, pacman);
			ghost4.Move(map, G4X, G4Y, pacman);
			
		}
		else {
			ModeScatter = true;
			ScatterCounter = 0;
			
			ghost1.SetScatter() = true;
			ghost2.SetScatter() = true;
			ghost3.SetScatter() = true;
			ghost4.SetScatter() = true;
			
			while (ModeScatter) {
				update(pacman, winscore);
							
				if ((ret = pacman.Move(map, PacmanX, PacmanY, gamemode)) == ENDGAME) {
					endgame();
					pacman.Quit();
					ghost1.Quit();
					ghost2.Quit();
					ghost3.Quit();
					ghost4.Quit();
				}
				
				if ((PacmanX == G1X && PacmanY == G1Y) || (PacmanX == G2X && PacmanY == G2Y) ||
				    (PacmanX == G3X && PacmanY == G3Y) || (PacmanX == G4X && PacmanY == G4Y)) {
					ret = pacman.LostLife();
					pacman.Reset();
					ghost1.Reset();
					ghost2.Reset();
					ghost3.Reset();
					ghost4.Reset();
					continue;
				}
				
				ghost1.Scatter(map, G1X, G1Y, pacman);
				ghost2.Scatter(map, G2X, G2Y, pacman);
				ghost3.Scatter(map, G3X, G3Y, pacman);
				ghost4.Scatter(map, G4X, G4Y, pacman);
				
				ghost1.SetScatter() = false;
				ghost2.SetScatter() = false;
				ghost3.SetScatter() = false;
				ghost4.SetScatter() = false;
				
				if ((PacmanX == G1X && PacmanY == G1Y) || (PacmanX == G2X && PacmanY == G2Y) ||
				    (PacmanX == G3X && PacmanY == G3Y) || (PacmanX == G4X && PacmanY == G4Y)) {
					pacman.LostLife();
					pacman.Reset();
					ghost1.Reset();
					ghost2.Reset();
					ghost3.Reset();
					ghost4.Reset();
					continue;
				}
				
				if (gamemode == 1) {
					ModeFrightened = true;
					while (ModeFrightened) {
						update(pacman, winscore);
						
						if ((ret = pacman.Move(map, PacmanX, PacmanY, gamemode)) == ENDGAME) {
							endgame();
							pacman.Quit();
							ghost1.Quit();
							ghost2.Quit();
							ghost3.Quit();
							ghost4.Quit();
						}
						
						if (PacmanX == G1X && PacmanY == G1Y) {
							ghost1.Reset();
							pacman.Bonus();
						}
						else if (PacmanX == G2X && PacmanY == G2Y){
							ghost2.Reset();
							pacman.Bonus();
						}
						else if (PacmanX == G3X && PacmanY == G3Y){
							ghost3.Reset();
							pacman.Bonus();
						}
						else if (PacmanX == G4X && PacmanY == G4Y){
							ghost4.Reset();
							pacman.Bonus();
						}
						
						ghost1.Frightened(map, G1X, G1Y, pacman);
						
						ghost2.Frightened(map, G2X, G2Y, pacman);
						
						ghost3.Frightened(map, G3X, G3Y, pacman);
						
						ghost4.Frightened(map, G4X, G4Y, pacman);
						
						if (PacmanX == G1X && PacmanY == G1Y) {
							ghost1.Reset();
							pacman.Bonus();
						}
						else if (PacmanX == G2X && PacmanY == G2Y){
							ghost2.Reset();
							pacman.Bonus();
						}
						else if (PacmanX == G3X && PacmanY == G3Y){
							ghost3.Reset();
							pacman.Bonus();
						}
						else if (PacmanX == G4X && PacmanY == G4Y){
							ghost4.Reset();
							pacman.Bonus();
						}
						
						FrigCounter++;
						if (FrigCounter == 30) {
							FrigCounter = 0;
							ModeFrightened = false;
						}
						if (gamemode == 1)
							FrigCounter = 0;
					}
				}
				
				ScatterCounter++;
				
				if (ScatterCounter == 10){
					ModeScatter = false;
					ScatterCounter = 0;
				}
			}
		}
		
		if ((PacmanX == G1X && PacmanY == G1Y) || (PacmanX == G2X && PacmanY == G2Y) ||
		    (PacmanX == G3X && PacmanY == G3Y) || (PacmanX == G4X && PacmanY == G4Y)) {
			pacman.LostLife();
			pacman.Reset();
			ghost1.Reset();
			ghost2.Reset();
			ghost3.Reset();
			ghost4.Reset();
			continue;
		}
		
		if (gamemode == 1)
			mvaddch(21,55,'F');
		else
			mvaddch(21,55,' ');
		
		ScatterCounter++;
		
	}
	clear();
	delwin(winscore);
	pacman.Quit();
	ghost1.Quit();
	ghost2.Quit();
	ghost3.Quit();
	ghost4.Quit();
	gameover();
}

int main(int argc, char* argv[]) {
	
	cout << "Please enter a map name within examples folder" << endl;
	string MapLocation;
	getline(cin,MapLocation);
	
	//Start ncurses, init
	initscr();
	noecho();
	curs_set(0);
	box(stdscr,'|','-');
	getmaxyx(stdscr,rows,cols);
	Map map;
	
	if (cols < 50 || rows < 25){
		delwin(stdscr);
		endwin();
		cout<< "your terminal window is too small" << endl;
		cout << rows << " " << cols << endl;
		return 0;
	}
	
	//new game
	newgame();
		
	//load map and play
	try{
	loadmap(MapLocation,map);
	}
	catch (int e){
		endwin();
		if (e == 1)
			cout << "Error: map file is corrupted" << endl;
		else if (e == 2)
			cout << "Error: " << '"' << MapLocation << '"' << " is not a file" << endl;
		return 1;
	}
	
	if (playdiff == 0) {
		Random ghost1(ghostPos[0], ghostPos[1]);
		Random ghost2(ghostPos[2], ghostPos[3]);
		Random ghost3(ghostPos[4], ghostPos[5]);
		Random ghost4(ghostPos[6], ghostPos[7]);
		play(map, ghost1, ghost2, ghost3, ghost4);
	}
	else if (playdiff == 1) {
		Chaser ghost1(ghostPos[0], ghostPos[1]);
		Ambusher ghost2(ghostPos[2], ghostPos[3]);
		Random ghost3(ghostPos[4], ghostPos[5]);
		Random ghost4(ghostPos[6], ghostPos[7]);
		play(map, ghost1, ghost2, ghost3, ghost4);
	}
	else if (playdiff == 2) {
		Chaser ghost1(ghostPos[0],ghostPos[1]);
		Ambusher ghost2(ghostPos[2],ghostPos[3]);
		Chaser ghost3(ghostPos[4],ghostPos[5]);
		Ambusher ghost4(ghostPos[6],ghostPos[7]);
		play(map, ghost1, ghost2, ghost3, ghost4);
	}
		
	delwin(stdscr);
	endwin();
	
	return 0;
}
