#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete block_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete mapChipFild_;
	delete player_;
	delete enemy_;
	delete deathParticles_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//ブロック
	blockTextureHandle_ = TextureManager::Load("cube/cube.jpg");
	block_ = Model::Create();
	mapChipFild_ = new MapChipField;
	mapChipFild_->LoadMapChipcsv("Resources/blocks.csv");
	GenerateBlocks();

	//天球(背景)
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	//プレイヤー
	modelPlayer_ = Model::CreateFromOBJ("AL3_player", true);
	player_ = new Player();
	Vector3 playerPosition = mapChipFild_->GetMapChipPositionByIndex(2, 18);
	player_->Initialize(modelPlayer_, &viewProjection_,playerPosition);
	player_->SetMapChipField(mapChipFild_);

	//敵
	modelEnemy_ = Model::CreateFromOBJ("AL3_enemy", true);
	for (int32_t i= 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipFild_->GetMapChipPositionByIndex(10 + i * 4, 18-i);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);
		enemies_.push_back(newEnemy);
	}
	
	//パーティクル
	modelDeathParticles_ = Model::CreateFromOBJ("particle", true);
	
	//カメラコントローラー
	movebleArea_ = {17, 181, 9, 50};

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->SetMovebleArea(movebleArea_);
	cameraController_->Reset();
	debugCamera_ = new DebugCamera(1280, 720);

	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}

void GameScene::Update() {

	switch (phase_) { 

	case Phase::kPlay:
		// 天球
		skydome_->Update();
		// プレイヤー
		player_->Update();
		// エネミー
		UpdateEnemys();
		// カメラ
		UpdateCameras();
		//ブロック
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->translation_, worldTransformBlock->rotation_);
				worldTransformBlock->TransferMatrix();
			}
		}
		//当たり判定
		CheckAllCollisions();

		if (player_->IsDead() == true) {
			phase_ = Phase::kDeath;
			const Vector3& deathParticlesPoation = player_->GetWorldPosition();
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, deathParticlesPoation);
		
		}
		
		break;

	case Phase::kDeath:

		// 天球
		skydome_->Update();

		//デスパーティクル
		if (deathParticles_) {
			deathParticles_->Update();
		}
		//エネミー
		UpdateEnemys();
		//カメラ
		UpdateCameras();
		// ブロック
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->translation_, worldTransformBlock->rotation_);
				worldTransformBlock->TransferMatrix();
			}
		}
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}
		break;
	}


	


#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 
	
	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			block_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	if (player_->IsDead() == false) {
		player_->Draw();
	}
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (deathParticles_) {
		deathParticles_->Draw();
	}
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtcal = mapChipFild_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipFild_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtcal);

	for (uint32_t i = 0; i < numBlockVirtcal; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtcal; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipFild_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipFild_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
	#pragma region

	AABB aabb1, aabb2;
	aabb1 = player_->GetAABB();
	for (Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollosion(enemy);
			enemy->OnCollosion(player_);
		}
	}
	#pragma endregion

}

void GameScene::UpdateEnemys() {
	// エネミー
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
}

void GameScene::UpdateCameras() {
	// カメラコントローラー
	cameraController_->Update();
	// デバッグカメラ
	debugCamera_->Update();
	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
	viewProjection_.matView = cameraController_->GetViewProjection().matView;
	viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:
		break;
	case Phase::kDeath:
		break;
	}

}
