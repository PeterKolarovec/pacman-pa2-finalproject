#include "map.h"
#include "tiles.h"
#define ROWS 24
#define COLS 36

Map::Map(){
	m_BonusCnt = 0;
	m_CoinCnt = 0;
	m_FrigCnt = 0;
}
Map::~Map() {
	for (auto it: m_Map)
			delete it;
}
Map & Map::Add(const Tiles & tile, int & y, int &x){
	Tiles * tmp = tile.copy();
	m_Map[y + COLS*x] = tmp;
	return *this;
}
Tiles * Map::Tile(int x, int y){
	return m_Map[y + COLS*x];
}
void Map::IncCoinCnt(){
	m_CoinCnt++;
}
void Map::IncBonusCnt(){
	m_BonusCnt++;
}
void Map::IncFrigCnt(){
	m_FrigCnt++;
}
int Map::CoinCnt(){
	return m_CoinCnt;
}
int Map::BonusCnt(){
	return m_BonusCnt;
}
int Map::FrigCnt(){
	return m_FrigCnt;
}
void Map::Resize(int y, int x) {
	m_Map.resize(y + COLS*x);
}
