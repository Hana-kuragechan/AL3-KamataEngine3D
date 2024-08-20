#include "GameOverScene.h"
#include <Input.h>
#include <cassert>
void GameOverScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	// テキスト
	gameoverText = new GameOverText;
	modelText = Model::CreateFromOBJ("gameoverText", true);
	gameoverText->Initialize(modelText, &viewProjection_);
}

void GameOverScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	gameoverText->Update();
}

void GameOverScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	///

	gameoverText->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion



}
