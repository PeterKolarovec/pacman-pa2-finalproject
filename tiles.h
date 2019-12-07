#ifndef SEMESTRALKA_TILES_H
#define SEMESTRALKA_TILES_H

#include <ncurses.h>
class Pacman;

class Tiles{
public:
	Tiles(int Ypos, int Xpos);
	virtual ~Tiles();
	virtual bool isPath() const = 0;
	virtual void Action(Pacman & x) const = 0;
	virtual Tiles * copy() const = 0;
	virtual void Redraw() const = 0;
	virtual int GetX() const = 0;
	virtual int GetY() const = 0;
protected:
	char m_Char;
	bool m_Path;
	int m_Ypos;
	int m_Xpos;
};

class TopWall : public Tiles{
public:
	TopWall(int Ypos, int Xpos);
	virtual bool isPath() const;
	virtual void Action(Pacman & x) const;
	virtual Tiles * copy() const;
	virtual void Redraw() const;
	virtual int GetX() const;
	virtual int GetY() const;
};

class Wall : public Tiles{
public:
	Wall(int Ypos, int Xpos);
	virtual bool isPath() const;
	virtual void Action(Pacman & x) const;
	virtual Tiles * copy() const;
	virtual void Redraw() const;
	virtual int GetX() const;
	virtual int GetY() const;
};

class PWall : public Tiles{
public:
	PWall(int Ypos, int Xpos);
	virtual bool isPath() const;
	virtual void Action(Pacman & x) const;
	virtual Tiles * copy() const;
	virtual void Redraw() const;
	virtual int GetX() const;
	virtual int GetY() const;
};

class Teleport : public Tiles{
public:
	Teleport(int Ypos, int Xpos);
	virtual bool isPath() const;
	virtual void Action(Pacman & x) const;
	virtual void Redraw() const;
	virtual Tiles * copy() const;
	virtual int GetX() const;
	virtual int GetY() const;
};

class Bonus : public Tiles{
public:
	Bonus(int Ypos, int Xpos);
	virtual bool isPath() const;
	virtual void Action(Pacman & x) const;
	virtual void Redraw() const;
	virtual Tiles * copy() const;
	virtual int GetX() const;
	virtual int GetY() const;
};

class Coin : public Tiles{
public:
	Coin(int Ypos, int Xpos);
	virtual bool isPath() const;
	virtual void Action(Pacman & x) const;
	virtual void Redraw() const;
	virtual Tiles * copy() const;
	virtual int GetX() const;
	virtual int GetY() const;
};

class Frightened : public Tiles{
public:
	Frightened(int Ypos, int Xpos);
	virtual bool isPath() const;
	virtual void Action(Pacman & x) const;
	virtual void Redraw() const;
	virtual Tiles * copy() const;
	virtual int GetX() const;
	virtual int GetY() const;
};

class Empty : public Tiles{
public:
	Empty(int Ypos, int Xpos);
	virtual bool isPath() const;
	virtual void Action(Pacman & x) const;
	virtual Tiles * copy() const;
	virtual void Redraw() const;
	virtual int GetX() const;
	virtual int GetY() const;
};

#endif //SEMESTRALKA_TILES_H
