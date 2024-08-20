#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include"CameraController.h"
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
#include"Enemy.h"
#include"DeathParticles.h"
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
	//当たり判定
	void CheckAllCollisions();
	//エネミーの更新
	void UpdateEnemys();
	//カメラ類の更新
	void UpdateCameras();
	//フェーズの変更
	void ChangePhase();
	// 終了フラグのgetter
	bool IsGameOver() const { return gameover_; }
	

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
	

	//敵
	Enemy* enemy_ = nullptr;
	Model* modelEnemy_ = nullptr;
	std::list<Enemy*>enemies_;
	//カメラコントローラー
	CameraController* cameraController_ = nullptr;
	CameraController::Rect movebleArea_ = {};

	//死亡時パーティクル
	DeathParticles* deathParticles_ = nullptr;
	Model* modelDeathParticles_ = nullptr;
	////死亡フラグ
	//bool isDead_ = false;
	//ゲームのフェーズ
	enum class Phase {
		kPlay,//ゲームプレイ
		kDeath,//デス演出
	};
	//ゲームの現在フェーズ
	Phase phase_;

	// 終了フラグ
	bool gameover_ = false;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
