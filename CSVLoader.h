#pragma once
#include <vector>

class CSVLoader
{
public:
	CSVLoader(const char* _defaultPath);
	static void LoadCSV(const char* _path);
	static void LoadCSV_sp(const char* _path);
	static std::vector<std::vector<int>>* GetDataPointer();

	static std::vector<std::vector<int>>* GetPiecePointer();

	static uint32_t getNum;

private:

	static std::vector<std::vector<int>> data;
	static std::vector<std::vector<std::vector<int>>> piece;
	char defaultPath[64];
};