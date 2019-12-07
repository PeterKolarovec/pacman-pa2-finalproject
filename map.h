#ifndef SEMESTRALKA_MAP_H
#define SEMESTRALKA_MAP_H

#include <vector>
class Tiles;
	
class Map{
public:
	Map();
	~Map();
	Map & Add(const Tiles & tile, int & y, int &x);
	Tiles * Tile(int x, int y);
	void IncCoinCnt();
	void IncBonusCnt();
	void IncFrigCnt();
	int CoinCnt();
	int BonusCnt();
	int FrigCnt();
	void Resize(int y, int x);
private:
	int m_CoinCnt;
	int m_BonusCnt;
	int m_FrigCnt;
	std::vector<Tiles *> m_Map;
};

#endif //SEMESTRALKA_MAP_H
