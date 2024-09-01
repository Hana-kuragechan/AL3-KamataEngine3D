#pragma once
#include "DirectXCommon.h"
#include "GameClearText.h"
#include"Skydome.h"
#include <ViewProjection.h>
class GameClearScene {

	public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	// 終了フラグのgetter
	bool IsFinished() const { return finished_; }
	// 終了フラグ
	bool finished_ = false;

private:
	DirectXCommon* dxCommon_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Model* modelText = nullptr;
	GameClearText* gameClearText = nullptr;


	
	// 天球(背景)
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
};
