#include "GameClearScene.h"
#include <Input.h>
#include <cassert>
void GameClearScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	// テキスト
	gameClearText = new GameClearText;
	modelText = Model::CreateFromOBJ("gameClearText", true);
	gameClearText->Initialize(modelText, &viewProjection_);

	// 天球(背景)
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);
}

void GameClearScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	gameClearText->Update();
	skydome_->Update();
}

void GameClearScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	///
	skydome_->Draw();
	gameClearText->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
}