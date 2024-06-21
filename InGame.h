#pragma once
#include <Vector2.h>
#include "Mapchip.h"


class InGame
{
public:
	void Initialize();
	void Update(const char* _keys, const char* _preKeys);
	void Draw();

private:
	Mapchip* mapchip = nullptr;
};