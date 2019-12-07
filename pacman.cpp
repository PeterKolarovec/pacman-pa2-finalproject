#include "pacman.h"
#include "map.h"
#include "tiles.h"
#define ROWS 24
#define COLS 36
#define ENDGAME -2
#define GAMEOVER -3
#define STARTY 18
#define STARTX 18
//----------------------------------------------------------------------------------------------------------------------
Pacman::Pacman(){
	m_Char = 'C';
	m_Dir = -2;
	m_Ypos = STARTY;
	m_Xpos = STARTX;
	m_Bonus = 0;
	m_Frig = 0;
	m_Coin = 0;
	m_Life = 3;
	keypad(stdscr,true);
	m_ChangeMode = false;
}
Pacman::~Pacman() {}
int Pacman::Move(Map & map, int & CharX, int & CharY, int & Frig){
	int x = m_Xpos;
	int y = m_Ypos;
	m_ChangeMode = false;
	int oldDir = m_Dir;
	
	int choice = getch();
	
	if (choice != -1) {
		oldDir = m_Dir;
		m_Dir = choice;
	}
	else if (choice == -1){
		m_Dir = oldDir;
	}
	
	map.Tile(m_Xpos,m_Ypos)->Action(*this);
	
	//special occasion TELEPORT
	if(m_Ypos == 12)
		Teleport();
		
	switch (m_Dir){
		case KEY_LEFT:
			delete map.Tile(x,y);
			map.Add(Empty(y,x),y,x);
			if(map.Tile(x-1,y)->isPath())
				m_Xpos--;
			else m_Dir = oldDir;
			break;
		case KEY_RIGHT:
			delete map.Tile(x,y);
			map.Add(Empty(y,x),y,x);
			if(map.Tile(x+1,y)->isPath())
				m_Xpos++;
			else m_Dir = oldDir;
			break;
		case KEY_UP:
			delete map.Tile(x,y);
			map.Add(Empty(y,x),y,x);
			if(map.Tile(x,y-1)->isPath())
				m_Ypos--;
			else m_Dir = oldDir;
			break;
		case KEY_DOWN:
			delete map.Tile(x,y);
			map.Add(Empty(y,x),y,x);
			if(map.Tile(x,y+1)->isPath())
				m_Ypos++;
			else m_Dir = oldDir;
			break;
		default:
			break;
	}
		
	if (m_Frig == map.FrigCnt() && m_Coin == map.CoinCnt() && m_Bonus == map.BonusCnt())
		return ENDGAME;
	if (m_Life == 0)
		return GAMEOVER;
	
	this->Redraw();
	
	//return Char position
	CharX = m_Xpos;
	CharY = m_Ypos;
	
	if (m_ChangeMode)
		Frig = 1;
	else
		Frig = 0;
	
	return choice;
}
void Pacman::Teleport(){
	if (m_Xpos == 2) {
		m_Xpos = COLS - 2;
		m_Dir = KEY_LEFT;
	}
	else if (m_Xpos == COLS - 2){
		m_Xpos = 2;
		m_Dir = KEY_RIGHT;
	}
	return;
}
void Pacman::Coin(){
	m_Coin++;
	return;
}
void Pacman::Bonus(){
	m_Bonus++;
	return;
}
void Pacman::Frightened(){
	m_Frig++;
	m_ChangeMode = true;
}
void Pacman::Redraw(){
	mvwaddch(stdscr,m_Ypos,m_Xpos,m_Char);
}

int Pacman::GetX() const{
	return m_Xpos;
}

int Pacman::GetY() const{
	return m_Ypos;
}
int Pacman::GetDir() const{
	return m_Dir;
}
void Pacman::Reset(){
	
	m_Ypos = STARTY;
	m_Xpos = STARTX;
	m_Dir = 0;
}
int Pacman::LostLife(){
	m_Life--;
	
	if (m_Life <= 0)
		return GAMEOVER;
	else
		return 0;
}
int Pacman::GetLife() const{
	return m_Life;
}
void Pacman::Quit() {
	m_Char = ' ';
	this->Redraw();
}
int Pacman::GetScore() const{
	return m_Coin+(m_Bonus*200);
}

