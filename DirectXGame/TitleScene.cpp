#include "TitleScene.h"
#include <Input.h>
#include <cassert>
void TitleScene::Initialize() { 

	dxCommon_ = DirectXCommon::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();
   // テキスト
	title = new Title;
	modelText = Model::CreateFromOBJ("title", true);
	title->Initialize(modelText, &viewProjection_);

	// 天球(背景)
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);
}

void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		finished_ = true;
	}
	title->Update();
	skydome_->Update();
}

void TitleScene::Draw() { 
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
	title->Draw(); 

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion


}
