#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>

namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};
}

void MapChipField::ResetMapChipDate() {
	mapChipDate_.date.clear();
	mapChipDate_.date.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDateLine : mapChipDate_.date) {
		mapChipDateLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipcsv(const std::string& filePath) {
	ResetMapChipDate();
	std::ifstream flie;
	flie.open(filePath);
	assert(flie.is_open());

	std::stringstream mapChipCsv;
	mapChipCsv << flie.rdbuf();
	flie.close();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipDate_.date[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {

		return MapChipType::kBlank;
	}
	return mapChipDate_.date[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {

	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<int32_t>((position.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = static_cast<int32_t>((position.y + kBlockHeight / 2) / kBlockHeight);
	int maxIndexY = kNumBlockVirtical - 1;
	indexSet.yIndex = maxIndexY - indexSet.yIndex;

	return indexSet;
}
