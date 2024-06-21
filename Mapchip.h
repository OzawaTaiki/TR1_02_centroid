#pragma once

#include<vector>
#include "Box.h"

class Mapchip
{
public:

	void Initialize();
	void Update();
	void Draw();

private:

	bool CheckCollision();

	const char* path[64] = {
	"./Resources/mapdata/testMapdata.csv"
	};

	std::vector<std::vector<int>> map;
	const int kMapchipSize = 64;

	std::vector<Box*> box;
};