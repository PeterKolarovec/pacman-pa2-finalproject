#ifndef SEMESTRALKA_CHARACTERS_H
#define SEMESTRALKA_CHARACTERS_H

#include <ncurses.h>

class Map;

class Pacman{
public:
	Pacman();
	~Pacman();
	int Move(Map & map, int & CharX, int & CharY, int & Frig);
	void Teleport();
	void Coin();
	void Bonus();
	void Frightened();
	void Redraw();
	int GetX() const;
	int GetY() const;
	int GetDir() const;
	int GetLife() const;
	int LostLife();
	void Reset();
	void Quit();
	int GetScore() const;
protected:
	int m_Ypos;
	int m_Xpos;
	char m_Char;
	int m_Coin;
	int m_Bonus;
	int m_Frig;
	int m_Dir;
	int m_Life;
	bool m_ChangeMode;
};

#endif //SEMESTRALKA_CHARACTERS_H
