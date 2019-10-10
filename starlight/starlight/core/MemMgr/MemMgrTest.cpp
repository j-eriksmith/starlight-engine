#include "MemMgr.h"
// used for uintptr_t
#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>
#include "test/PoolAllocTestFixture.h"
#include "test/MemMgrTestFixture.h"

/*
	Author: Alejandro Valdes
	Date: July 2019
*/

// ============================== TESTS ==============================
//class payToWinClub;
//class Bag;
//class Item;
//class Player;
//class MtxPlayer;
//
//void testPoolAddrs(uint start)
//{
//	// 50 Mib storage 52428800
//	MemMgr m = MemMgr(102400);
//	for (uint i = start; i >= 4; i /= 2)
//	{
//		//std::cout << "Address of pool for size " << i << " blocks is "
//		//	<< m.getPoolAddress(i) << std::endl;
//	}
//}
//
//void initMemMgr()
//{
//	// Alloc ~50 MiB of storage
//	MemMgr mgr = MemMgr(102400);
//}
//
//// 32
//// size 24 object
//class Player
//{
//	uint health;
//	double x_pos;
//	double y_pos;
//public:
//	Player(uint h, double x, double y)
//		:health(h), x_pos(x), y_pos(y)
//	{}
//	Player()
//		:health(0), x_pos(0), y_pos(0)
//	{}
//	void printInfo()
//	{
//		std::cout << "health: " << health << ", x_pos: " << x_pos << ", y_pos: " << y_pos << std::endl;
//	}
//};
//
//// test 128
//// size 88
//class Item
//{
//	double price;
//	uint metaData[10];
//
//public:
//	Item(double p)
//		:price(p)
//	{}
//	explicit Item()
//		:price(0.0)
//	{}
//};
//
//// 256
//// size 196
//class Bag
//{
//	Item bag[2];
//
//public:
//	explicit Bag() {}
//};
//
//
//
//// 512
//// size 24 + 264 + 8 = 296
//class MtxPlayer : public Player
//{
//	double totalSpent;
//	Item emote;
//	Item hat;
//	Item ezSkin;
//
//
//public:
//	explicit MtxPlayer()
//		:Player(0, 0.0, 0.0), totalSpent(0.0), emote(Item(0.0)), hat(Item(0.0)), ezSkin(Item(0.0))
//	{}
//
//	MtxPlayer(uint health, double x, double y, double spent)
//		:Player(health, x, y), totalSpent(spent), emote(Item(10.0)), hat(Item(11.0)), ezSkin(Item(12.0))
//	{}
//};
//
//// 1024
//// size 592
//class payToWinClub
//{
//	MtxPlayer coolKids[2];
//public:
//	explicit payToWinClub() {}
//};
//
//
//Player* spawnPlayer(uint h, double x_pos, double y_pos, MemMgr& m)
//{
//	Player x(h, x_pos, y_pos);
//	// Allocate enough space to hold a vector of numPlayers Player objects
//	Player* addr = reinterpret_cast<Player*>(m.Alloc(sizeof Player));
//	*addr = x;
//
//	return addr;
//}
//
//void deletePlayer(uint itemSize, Player* p, MemMgr& m)
//{
//	m.Free(itemSize, p);
//}
//
//// @todo 4, 8, 16, 64
//
//void test_16(MemMgr& m, int AllocSize)
//{
//	Player** players = new Player* [AllocSize];
//	Player* currentPlayer = nullptr;
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		players[i] = reinterpret_cast<Player*>(m.Alloc(sizeof Player));
//		if (players[i])
//		{
//			*players[i] = Player(i, i, i);
//		}
//		else
//		{
//			std::cout << "POOL IS FULL. CANNOT AllocATE ITEM" << std::endl;
//		}
//	}
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		m.Free(sizeof Player, players[i]);
//	}
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		players[i] = reinterpret_cast<Player*>(m.Alloc(sizeof Player));
//		if (players[i])
//		{
//			*players[i] = Player(i, i, i);
//		}
//		else
//		{
//			std::cout << "POOL IS FULL. CANNOT AllocATE ITEM" << std::endl;
//		}
//	}
//}
//
//void test_128(MemMgr& m, int AllocSize)
//{
//	Bag** players = new Bag * [AllocSize];
//	Bag* currentPlayer = nullptr;
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		players[i] = reinterpret_cast<Bag*>(m.Alloc(sizeof Bag));
//		if (players[i])
//		{
//			*players[i] = Bag();
//		}
//		else
//		{
//			std::cout << "POOL IS FULL. CANNOT AllocATE ITEM" << std::endl;
//		}
//	}
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		m.Free(sizeof Bag, players[i]);
//	}
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		players[i] = reinterpret_cast<Bag*>(m.Alloc(sizeof Bag));
//		if (players[i])
//		{
//			*players[i] = Bag();
//		}
//		else
//		{
//			std::cout << "POOL IS FULL. CANNOT AllocATE ITEM" << std::endl;
//		}
//	}
//}
//
//void test_256(MemMgr& m, int AllocSize)
//{
//	MtxPlayer** players = new MtxPlayer * [AllocSize];
//	MtxPlayer* currentPlayer = nullptr;
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		players[i] = reinterpret_cast<MtxPlayer*>(m.Alloc(sizeof MtxPlayer));
//		if (players[i])
//		{
//			*players[i] = MtxPlayer();
//		}
//		else
//		{
//			std::cout << "POOL IS FULL. CANNOT AllocATE ITEM" << std::endl;
//		}
//	}
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		m.Free(sizeof MtxPlayer, players[i]);
//	}
//	for (int i = 0; i < AllocSize; ++i)
//	{
//		players[i] = reinterpret_cast<MtxPlayer*>(m.Alloc(sizeof MtxPlayer));
//		if (players[i])
//		{
//			*players[i] = MtxPlayer();
//		}
//		else
//		{
//			std::cout << "POOL IS FULL. CANNOT AllocATE ITEM" << std::endl;
//		}
//	}
//}


int main()
{


	//initMemMgr();
	//testPoolAddrs(1024);
	//MemMgr m = MemMgr(102400);
	////std::vector<Player>* players = spawnPlayers(25, m);
	//Player* p1 = spawnPlayer(100, 1.5, 1.5, m);
	//Player* p2 = spawnPlayer(100, 2.5, 2.5, m);
	//Player* p3 = spawnPlayer(100, 3.5, 3.5, m);
	//deletePlayer(sizeof Player, p1, m);
	//Player* p4 = spawnPlayer(100, 4.5, 4.5, m);
	////test_16(m, 50);
	////test_128(m, 50);
	////test_256(m, 50);
	//MemMgrTestFixture m;
	//m.RunTests();
	PoolAllocTestFixture p;
	p.RunTests();

	return 0;
}