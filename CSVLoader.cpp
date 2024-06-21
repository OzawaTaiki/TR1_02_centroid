#include "CSVLoader.h"
#include <cassert>

std::vector< std::vector<int>>  CSVLoader::data;
std::vector<std::vector<std::vector<int>>> CSVLoader::piece;
uint32_t CSVLoader::getNum = 0;

CSVLoader::CSVLoader(const char* _defaultPath)
{
	strcpy_s(defaultPath, 64, _defaultPath);
	LoadCSV(_defaultPath);
	getNum = 0;
}

void CSVLoader::LoadCSV(const char* _path)
{
	getNum = 0;
	FILE* fp = nullptr;
	fopen_s(&fp, _path, "rt");

	char buffer[1024]{};
	char* token = nullptr;
	char* saveptr = nullptr;

	if (fp == nullptr)
	{
		return;
	}
	else
	{
		data.clear();

		int row = 0, col = 0;
		while (fgets(buffer, sizeof(buffer), fp))
		{
			token = strtok_s(buffer, ",", &saveptr);

			if (token == nullptr || *token == '\n')	break;

			data.push_back(std::vector<int>(0));
			data[row].push_back(0);
			data[row][col++] = atoi(token);

			while (true)
			{
				token = strtok_s(nullptr, ",", &saveptr);
				if (token == nullptr || *token == '\n')	break;
				data[row].push_back(0);
				data[row][col++] = atoi(token);

			}
			row++;
			col = 0;
		}
	}

	fclose(fp);
}

void CSVLoader::LoadCSV_sp(const char* _path)
{
	getNum = 0;

	FILE* fp = nullptr;
	fopen_s(&fp, _path, "rt");

	char buffer[1024]{};
	char* token = nullptr;
	char* saveptr = nullptr;

	if (fp == nullptr)
	{
		return;
	}
	else
	{
		data.clear();

		int row = 0, col = 0;
		while (fgets(buffer, sizeof(buffer), fp))
		{
			token = strtok_s(buffer, ",", &saveptr);

			if (token == nullptr || *token == '\n')	break;

			data.push_back(std::vector<int>(0));
			data[row].push_back(0);
			data[row][col++] = atoi(token);

			while (true)
			{
				token = strtok_s(nullptr, ",", &saveptr);
				if (token == nullptr || *token == '\n')	break;
				data[row].push_back(0);
				data[row][col++] = atoi(token);

			}
			row++;
			col = 0;
		}

		///ピースの読み取り
		piece.clear();

		row = 0, col = 0;
		int elem = 0;

		while (fgets(buffer, sizeof(buffer), fp))
		{
			piece.push_back(std::vector< std::vector<int>>(0));
			bool isExit = false;

			while (!isExit)
			{
				token = strtok_s(buffer, ",", &saveptr);
				if (*token == '\n' && row != 0)	break;
				else if (*token == '\n' && row == 0)
				{
					fgets(buffer, sizeof(buffer), fp);
					continue;
				}

				piece[elem].push_back(std::vector<int>(0));
				piece[elem][row].push_back(0);
				piece[elem][row][col++] = atoi(token);

				while (true)
				{
					token = strtok_s(nullptr, ",", &saveptr);
					if (token == nullptr || *token == '\n' || *token == '\0')
					{
						isExit = !fgets(buffer, sizeof(buffer), fp);
						break;
					}
					piece[elem][row].push_back(0);
					piece[elem][row][col++] = atoi(token);

				}
				row++;
				col = 0;
			}
			row = 0;
			elem++;
		}

	}

	fclose(fp);
}


std::vector<std::vector<int>>* CSVLoader::GetDataPointer()
{
	return &data;
}

std::vector<std::vector<int>>* CSVLoader::GetPiecePointer()
{

	if (piece.size() <= getNum)
		return nullptr;
	return &piece[getNum++];
}

