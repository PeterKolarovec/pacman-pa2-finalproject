#include "tiles.h"
#include "pacman.h"

Tiles::Tiles(int Ypos = 0, int Xpos = 0){
	m_Xpos = Xpos;
	m_Ypos = Ypos;
}
Tiles::~Tiles(){}
//----------------------------------------------------------------------------------------------------------------------
TopWall::TopWall(int Ypos, int Xpos) : Tiles(Ypos,Xpos){
	m_Path = false;
	m_Char = '=';
	Redraw();
}
bool TopWall::isPath() const{
	return m_Path;
}
void TopWall::Action(Pacman & x) const {
	return;
}
Tiles * TopWall::copy() const{
	return new TopWall(this->GetY(),this->GetX());
}
void TopWall::Redraw() const{
	mvaddch(m_Ypos,m_Xpos,m_Char);
}
int TopWall::GetX() const{
	return m_Xpos;
}

int TopWall::GetY() const{
	return m_Ypos;
}
//----------------------------------------------------------------------------------------------------------------------
Wall::Wall(int Ypos, int Xpos) : Tiles(Ypos,Xpos){
	m_Path = false;
	m_Char = '|';
	Redraw();
}
bool Wall::isPath() const{
	return m_Path;
}
void Wall::Action(Pacman & x) const {
	return;
}
Tiles * Wall::copy() const{
	return new Wall(this->GetY(),this->GetX());
}
void Wall::Redraw() const{
	mvaddch(m_Ypos,m_Xpos,m_Char);
}
int Wall::GetX() const{
	return m_Xpos;
}

int Wall::GetY() const{
	return m_Ypos;
}
//----------------------------------------------------------------------------------------------------------------------
PWall::PWall(int Ypos, int Xpos) : Tiles(Ypos,Xpos){
	m_Path = false;
	m_Char = '+';
	Redraw();
}
bool PWall::isPath() const{
	return m_Path;
}
void PWall::Action(Pacman & x) const {
	return;
}
Tiles * PWall::copy() const{
	return new PWall(this->GetY(),this->GetX());
}
void PWall::Redraw() const{
	mvaddch(m_Ypos,m_Xpos,m_Char);
}
int PWall::GetX() const{
	return m_Xpos;
}

int PWall::GetY() const{
	return m_Ypos;
}
//----------------------------------------------------------------------------------------------------------------------
Teleport::Teleport(int Ypos, int Xpos) : Tiles(Ypos,Xpos){
	m_Path = true;
	m_Char = 'T';
	Redraw();
}
bool Teleport::isPath() const{
	return m_Path;
}
void Teleport::Action(Pacman & x) const {
	x.Teleport();
	this->Redraw();
}
void Teleport::Redraw() const{
	mvaddch(m_Ypos,m_Xpos,m_Char);
}
Tiles * Teleport::copy() const{
	return new Teleport(this->GetY(),this->GetX());
}
int Teleport::GetX() const{
	return m_Xpos;
}

int Teleport::GetY() const{
	return m_Ypos;
}
//----------------------------------------------------------------------------------------------------------------------
Bonus::Bonus(int Ypos, int Xpos) : Tiles(Ypos,Xpos){
	m_Path = true;
	m_Char = 'B';
	Redraw();
}
bool Bonus::isPath() const{
	return m_Path;
}
void Bonus::Action(Pacman & x) const{
	x.Bonus();
}
void Bonus::Redraw() const{
	mvaddch(m_Ypos,m_Xpos,m_Char);
}
Tiles * Bonus::copy() const{
	return new Bonus(this->GetY(),this->GetX());
}
int Bonus::GetX() const{
	return m_Xpos;
}

int Bonus::GetY() const{
	return m_Ypos;
}
//----------------------------------------------------------------------------------------------------------------------
Coin::Coin(int Ypos, int Xpos) : Tiles(Ypos,Xpos){
	m_Path = true;
	m_Char = '.';
	Redraw();
}
bool Coin::isPath() const{
	return m_Path;
}
void Coin::Action(Pacman & x) const{
	x.Coin();
}
void Coin::Redraw() const{
	mvaddch(m_Ypos,m_Xpos,m_Char);
}
Tiles * Coin::copy() const{
	return new Coin(this->GetY(),this->GetX());
}
int Coin::GetX() const{
	return m_Xpos;
}

int Coin::GetY() const{
	return m_Ypos;
}
//----------------------------------------------------------------------------------------------------------------------
Frightened::Frightened(int Ypos, int Xpos) : Tiles(Ypos,Xpos){
	m_Path = true;
	m_Char = 'F';
	Redraw();
}
bool Frightened::isPath() const{
	return m_Path;
}
void Frightened::Action(Pacman & x) const{
	x.Frightened();
}
void Frightened::Redraw() const{
	mvaddch(m_Ypos,m_Xpos,m_Char);
}
Tiles * Frightened::copy() const{
	return new Frightened(this->GetY(),this->GetX());
}
int Frightened::GetX() const{
	return m_Xpos;
}

int Frightened::GetY() const{
	return m_Ypos;
}
//----------------------------------------------------------------------------------------------------------------------
Empty::Empty(int Ypos, int Xpos) : Tiles(Ypos,Xpos){
	m_Path = true;
	m_Char = ' ';
	Empty::Redraw();
}
bool Empty::isPath() const{
	return m_Path;
}
Tiles * Empty::copy() const{
	return new Empty(this->GetY(),this->GetX());
}
void Empty::Action(Pacman & x) const{
	return;
}
void Empty::Redraw() const{
	mvaddch(m_Ypos,m_Xpos,m_Char);
}
int Empty::GetX() const{
	return m_Xpos;
}

int Empty::GetY() const{
	return m_Ypos;
}

