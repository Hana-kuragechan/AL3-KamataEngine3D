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
}

void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	title->Update();
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

	title->Draw(); 

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion


}
