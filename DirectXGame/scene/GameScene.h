#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<vector>
#include"MT.h"
#include"DebugCamera.h"
#include"Skydome.h"
#include"MapChipField.h"
#include "Player.h"
#include"CameraController.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {


public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	void GenerateBlocks();
	std::vector<std::vector<WorldTransform*>>worldTransformBlocks_;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_; 

	//ブロック(地面)
	uint32_t blockTextureHandle_ = 0;
	Model* block_ = nullptr;
	MapChipField* mapChipFild_;

	//デバッグカメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	//天球(背景)
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	
	//プレイヤー
	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;
	uint32_t textureHandle_ = 0;

	//カメラコントローラー
	CameraController* cameraController_ = nullptr;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
