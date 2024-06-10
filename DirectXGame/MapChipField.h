#include <stdint.h>
#include<vector>
#include <string>
#include<Vector3.h>

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipDate {
	std::vector<std::vector<MapChipType>> date;
};
struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;

};
class MapChipField {

public:

	void ResetMapChipDate();
	void LoadMapChipcsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; };
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; };
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

private:

	//ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
	MapChipDate mapChipDate_;
	
};
