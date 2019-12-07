#include "ghosts.h"
#include "map.h"
#include "tiles.h"
#include "pacman.h"
#include <cmath>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define ROWS 24
#define COLS 36

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
Ghost::Ghost(int Ypos, int Xpos){
	m_Ypos = Ypos;
	m_Xpos = Xpos;
	m_OldX = m_Xpos;
	m_OldY = m_Ypos;
	m_StartX = Xpos;
	m_StartY = Ypos;
	m_GameMode = 0;
	m_Released = false;
	m_Dir = 0;
	m_Char = 'R';
}
int Ghost::Move(Map & map, int & GhostX, int & GhostY, const Pacman & pacman){
	int x = m_Xpos;
	int y = m_Ypos;
	m_LastTile = map.Tile(m_OldX, m_OldY);
	map.Tile(x,y)->Redraw();
	m_LastTile = map.Tile(m_OldX, m_OldY);
	
	if (!m_Released)
		this->Release();
	
	if (Crossroad(*this, map))
		Decide(pacman, map, *this);
	
	m_OldY = m_Ypos;
	m_OldX = m_Xpos;
	
	m_LastTile = map.Tile(m_OldX, m_OldY);
	
	if (m_Ypos == 12)
		Teleport();
	
	switch (m_Dir) {
		case KEY_LEFT:
			if (map.Tile(x - 1, y)->isPath())
				m_Xpos--;
			break;
		case KEY_RIGHT:
			if (map.Tile(x + 1, y)->isPath())
				m_Xpos++;
			break;
		case KEY_UP:
			if (map.Tile(x, y - 1)->isPath())
				m_Ypos--;
			break;
		case KEY_DOWN:
			if (map.Tile(x, y + 1)->isPath())
				m_Ypos++;
			break;
		default:
			break;
	}
	m_LastTile->Redraw();
	m_ActualTile = map.Tile(m_Xpos,m_Ypos);
	this->Redraw();
	
	GhostX = m_Xpos;
	GhostY = m_Ypos;
	
	m_GameMode = 0;
	
	return 0;
}
void Ghost::Teleport(){
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

void Ghost::Frightened(Map & map, int & GhostX, int & GhostY, const Pacman & pacman){
	char oldChar = m_Char;
	m_Char = '#';
	int x = m_Xpos;
	int y = m_Ypos;
	map.Tile(x,y)->Redraw();
	
	srand (time(NULL));
	int randDir = rand() % 4;
	
	if (!m_Released)
		this->Release();
	
	if (Crossroad(*this, map))
		switch (randDir){
			case UP:
				m_Dir = KEY_UP;
				break;
			case DOWN:
				m_Dir = KEY_DOWN;
				break;
			case LEFT:
				m_Dir = KEY_LEFT;
				break;
			case RIGHT:
				m_Dir = KEY_RIGHT;
				break;
		}
	
	
	m_OldY = m_Ypos;
	m_OldX = m_Xpos;
	
	m_LastTile = map.Tile(m_OldX, m_OldY);
	
	if (m_Ypos == 12)
		Teleport();
	
	switch (m_Dir) {
		case KEY_LEFT:
			if (map.Tile(x - 1, y)->isPath())
				m_Xpos--;
			break;
		case KEY_RIGHT:
			if (map.Tile(x + 1, y)->isPath())
				m_Xpos++;
			break;
		case KEY_UP:
			if (map.Tile(x, y - 1)->isPath())
				m_Ypos--;
			break;
		case KEY_DOWN:
			if (map.Tile(x, y + 1)->isPath())
				m_Ypos++;
			break;
		default:
			break;
	}
	m_LastTile->Redraw();
	this->Redraw();
	m_ActualTile = map.Tile(m_Xpos,m_Ypos);
	
	GhostX = m_Xpos;
	GhostY = m_Ypos;
	m_Char = oldChar;
	return;
}

void Ghost::Scatter(Map & map, int & GhostX, int & GhostY, const Pacman & pacman){
	m_GameMode = 2;
	
	if (m_Scatter) {
		switch (m_Dir) {
			case KEY_UP:
				m_Dir = KEY_DOWN;
				break;
			case KEY_DOWN:
				m_Dir = KEY_UP;
				break;
			case KEY_LEFT:
				m_Dir = KEY_RIGHT;
				break;
			case KEY_RIGHT:
				m_Dir = KEY_LEFT;
				break;
		}
	}
	this->Move(map, GhostX, GhostY, pacman);
}

void Ghost::Redraw() const{
	mvwaddch(stdscr,m_Ypos,m_Xpos,m_Char);
}

void Ghost::Release(){
	m_Released = true;
	m_Ypos -= 2;
	m_Dir = KEY_RIGHT;
	return;
}
void Ghost::Decide(const Pacman & pacman, Map & map, Ghost & ghost){
	GetTOI(pacman);
	int dir = 10;
	int minDist = 100;
	Tiles * directions[4];
	double distance[4];
	
	directions[LEFT] = map.Tile(ghost.m_Xpos - 1, ghost.m_Ypos);
	directions[RIGHT] = map.Tile(ghost.m_Xpos + 1, ghost.m_Ypos);
	directions[DOWN] = map.Tile(ghost.m_Xpos, ghost.m_Ypos + 1);
	directions[UP] = map.Tile(ghost.m_Xpos, ghost.m_Ypos - 1);
	
	for (int i = 0; i < 4; ++i) {
		if (m_LastTile->GetX() == directions[i]->GetX() && m_LastTile->GetY() == directions[i]->GetY()){
			distance[i] = 0;
			continue;
		}
		if(directions[i]->isPath()) {
			distance[i] = (int) floor(sqrt(
					pow((double) std::abs(m_TOI.m_X - directions[i]->GetX()), 2)
					+ pow((double) std::abs(m_TOI.m_Y - directions[i]->GetY()), 2)));
		}
		else distance[i] = 0;
	}
	
	for (int k = 0; k < 4; ++k) {
		if (distance[k] < minDist && distance[k] != 0){
			minDist = distance[k];
			dir = k;
		}
	}
	
	switch (dir) {
		case UP:
			m_Dir = KEY_UP;
			break;
		case DOWN:
			m_Dir = KEY_DOWN;
			break;
		case LEFT:
			m_Dir = KEY_LEFT;
			break;
		case RIGHT:
			m_Dir = KEY_RIGHT;
			break;
		default:
			break;
	}
	return;
}
bool Ghost::Crossroad(Ghost & ghost, Map & map) const{
	if ((ghost.m_Dir == KEY_LEFT || ghost.m_Dir == KEY_RIGHT) && (map.Tile(ghost.m_Xpos,ghost.m_Ypos - 1)->isPath() || map.Tile(ghost.m_Xpos,ghost.m_Ypos + 1)->isPath()))
		return true;
	else if ((ghost.m_Dir == KEY_UP || ghost.m_Dir == KEY_DOWN) && (map.Tile(ghost.m_Xpos + 1,ghost.m_Ypos)->isPath() || map.Tile(ghost.m_Xpos - 1,ghost.m_Ypos)->isPath()))
		return true;
	
	return false;
}
void Ghost::Reset(){
	m_ActualTile->Redraw();
	m_Xpos = m_StartX;
	m_Ypos = m_StartY;
	m_Released = false;
	Redraw();
	m_OldX = m_Xpos;
	m_OldY = m_Ypos;
}
bool & Ghost::SetScatter(){
	return m_Scatter;
}

void Ghost::Quit() {
	m_Char = ' ';
	this->Redraw();
}
//----------------------------------------------------------------------------------------------------------------------
Chaser::Chaser(int Ypos, int Xpos) : Ghost(Ypos,Xpos){
	m_Char = 'R';
}
void Chaser::GetTOI(const Pacman & pacman){
	if (m_GameMode == 0){
		m_TOI.m_X = pacman.GetX();
		m_TOI.m_Y = pacman.GetY();
	}
	else if (m_GameMode == 2){
		m_TOI.m_X = 0;
		m_TOI.m_Y = 0;
	}
}
//----------------------------------------------------------------------------------------------------------------------
Ambusher::Ambusher(int Ypos, int Xpos) : Ghost(Ypos,Xpos){
	m_Char = 'A';
}
void Ambusher::GetTOI(const Pacman & pacman) {
	if (m_GameMode == 0) {
		switch (pacman.GetDir()) {
			case KEY_UP:
				m_TOI.m_X = pacman.GetX();
				m_TOI.m_Y = pacman.GetY() - 4;
				break;
			case KEY_DOWN:
				m_TOI.m_X = pacman.GetX();
				m_TOI.m_Y = pacman.GetY() + 4;
				break;
			case KEY_LEFT:
				m_TOI.m_X = pacman.GetX() - 4;
				m_TOI.m_Y = pacman.GetY();
				break;
			case KEY_RIGHT:
				m_TOI.m_X = pacman.GetX() + 4;
				m_TOI.m_Y = pacman.GetY();
				break;
			default:
				break;
		}
	}
	else if (m_GameMode == 2){
		m_TOI.m_X = COLS;
		m_TOI.m_Y = 0;
	}
	if (m_TOI.m_Y > ROWS) m_TOI.m_Y = ROWS;
	if (m_TOI.m_X > COLS) m_TOI.m_X = COLS;
}
//----------------------------------------------------------------------------------------------------------------------
Random::Random(int Ypos, int Xpos) : Ghost(Ypos,Xpos){
	m_Char = 'D';
	srand(time(NULL));
	
}
void Random::GetTOI(const Pacman & pacman){
	if (m_GameMode == 0) {
		m_TOI.m_X = rand() % 36;
		m_TOI.m_Y = rand() % 24;
	}
	else if (m_GameMode == 2){
		m_TOI.m_X = 0;
		m_TOI.m_Y = ROWS;
	}
}
