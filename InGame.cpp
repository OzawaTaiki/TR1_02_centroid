#include "InGame.h"
#include<Novice.h>

void InGame::Initialize()
{
	mapchip = new Mapchip;
	mapchip->Initialize();
}

void InGame::Update(const char* _keys, const char* _preKeys)
{
	if (_keys[DIK_R] && !_preKeys[DIK_R])
		Initialize();

	mapchip->Update();
}

void InGame::Draw()
{
	mapchip->Draw();
}
