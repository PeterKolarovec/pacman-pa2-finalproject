#ifndef SEMESTRALKA_GHOSTS_H
#define SEMESTRALKA_GHOSTS_H
#include <ncurses.h>

class Map;
class Pacman;
class Tiles;

//TILE OF INTEREST
struct TOI{
	int m_X;
	int m_Y;
	TOI(){
		m_X = 0;
		m_Y = 0;
	}
	~TOI(){}
};

class Ghost{
public:
	Ghost(int Ypos, int Xpos);
	int Move(Map & map, int & GhostX, int & GhostY, const Pacman & pacman);
	void Teleport();
	void Frightened(Map & map, int & GhostX, int & GhostY, const Pacman & pacman);
	void Scatter(Map & map, int & GhostX, int & GhostY, const Pacman & pacman);
	void Redraw() const;
	void Release();
	void Decide(const Pacman & pacman, Map & map, Ghost & ghost);
	bool Crossroad(Ghost & ghost, Map & map) const;
	void Reset();
	bool & SetScatter();
	virtual void GetTOI(const Pacman & pacman) = 0;
	void Quit();
protected:
	int m_Ypos;
	int m_Xpos;
	int m_OldY;
	int m_OldX;
	int m_StartX;
	int m_StartY;
	char m_Char;
	int m_Dir;
	bool m_Released;
	int m_GameMode;
	bool m_Scatter;
	Tiles * m_ActualTile;
	Tiles * m_LastTile;
	TOI m_TOI;
};

class Chaser : public Ghost{
public:
	Chaser(int Ypos, int Xpos);
	virtual void GetTOI(const Pacman & pacman);
};

class Ambusher : public Ghost{
public:
	Ambusher(int Ypos, int Xpos);
	virtual void GetTOI(const Pacman & pacman);
};

class Random : public Ghost{
public:
	Random(int Ypos, int Xpos);
	virtual void GetTOI(const Pacman & pacman);
};

#endif //SEMESTRALKA_GHOSTS_H
