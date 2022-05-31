#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	//キャラクターの大元
	worldTransforms_[PartId::kRoot].Initialize();
	//脊髄
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0,4.5f,0 };

	//上半身
	//胸
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };

	//頭
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].translation_ = { 0,3.0f,0 };

	//左腕
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].translation_ = { -3.0f,0,0 };

	//右腕
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].translation_ = { 3.0f,0,0 };

	//下半身
	//尻
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kHip].translation_ = { 0,-3.0f,0 };

	//左足
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].translation_ = { -3.0f,-3.0f,0 };

	//左足
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].translation_ = { 3.0f,-3.0f,0 };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの作成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_/*debugCamera_->GetViewProjection()*/);
	//X, Y, Z方向のスケーリングを設定
	//worldTransform_.scale_ = { 5,5,5 };
	//worldTransform_.rotation_ = { 3.14f / 4.0f,3.14f / 4.0f ,0};
	//worldTransform_.translation_ = { 10,10,10 };
	//for
}

void GameScene::Update() //視点移動処理
{
	//デバッグカメラの更新
	//debugCamera_->Update();
	//視点の移動ベクトル
	//Vector3 move = { 0,0,0 };
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	//視点の移動速さ
	const float kEyeSpeed = 0.2f;
	//上方向の回転速さ[ラジアン/frame]
	const float kUpSpeed = 0.05f;
	//キャラクター移動処理
	{
		//キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kCharacterSpeed,0,0 };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move = { kCharacterSpeed,0,0 };
		}
	}

	worldTransforms_[PartId::kRoot].translation_ += move;

	//大元から順に更新していく
	for (int i = 0; i < PartId::kNumPartId; i++) {

		Matrix4 matIdentity;
		matIdentity.m[0][0] = 1;
		matIdentity.m[1][1] = 1;
		matIdentity.m[2][2] = 1;
		matIdentity.m[3][3] = 1;

		Matrix4 matScale = matIdentity;
		matScale.m[0][0] = worldTransforms_[i].scale_.x;
		matScale.m[1][1] = worldTransforms_[i].scale_.y;
		matScale.m[2][2] = worldTransforms_[i].scale_.z;
		matScale.m[3][3] = 1;

		Matrix4 matRotZ = matIdentity;
		matRotZ.m[0][0] = cos(worldTransforms_[i].rotation_.z);
		matRotZ.m[0][1] = sin(worldTransforms_[i].rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransforms_[i].rotation_.z);
		matRotZ.m[1][1] = cos(worldTransforms_[i].rotation_.z);

		Matrix4 matRotX = matIdentity;
		matRotX.m[1][1] = cos(worldTransforms_[i].rotation_.x);
		matRotX.m[1][2] = sin(worldTransforms_[i].rotation_.x);
		matRotX.m[2][1] = -sin(worldTransforms_[i].rotation_.x);
		matRotX.m[2][2] = cos(worldTransforms_[i].rotation_.x);

		Matrix4 matRotY = matIdentity;
		matRotY.m[0][0] = cos(worldTransforms_[i].rotation_.y);
		matRotY.m[0][2] = sin(worldTransforms_[i].rotation_.y);
		matRotY.m[2][0] = -sin(worldTransforms_[i].rotation_.y);
		matRotY.m[2][2] = cos(worldTransforms_[i].rotation_.y);

		Matrix4 matTrans = matIdentity;
		matTrans.m[3][0] = worldTransforms_[i].translation_.x;
		matTrans.m[3][1] = worldTransforms_[i].translation_.y;
		matTrans.m[3][2] = worldTransforms_[i].translation_.z;
		matTrans.m[3][3] = 1;

		worldTransforms_[i].matWorld_ = matIdentity;
		worldTransforms_[i].matWorld_ *= matScale;

		worldTransforms_[i].matWorld_ *= matRotZ;
		worldTransforms_[i].matWorld_ *= matRotX;
		worldTransforms_[i].matWorld_ *= matRotY;

		worldTransforms_[i].matWorld_ *= matTrans;

		if (i == 1) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[PartId::kRoot].matWorld_;
		}
		if (i == 2 || i == 6) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[PartId::kSpine].matWorld_;
		}
		if (i > 2 && i < 6) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[PartId::kChest].matWorld_;
		}
		if (i > 6 && i < kNumPartId) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[PartId::kHip].matWorld_;
		}

		worldTransforms_[i].TransferMatrix();

	}
	//上半身の回転処理
	{
		//上半身の回転速さ[ラジアン/frame]
		const float kChestRotSpeed = 0.05f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransforms_[PartId::kChest].rotation_.y -= kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransforms_[PartId::kChest].rotation_.y += kChestRotSpeed;
		}
	}
	//下半身の回転処理
	{
		//上半身の回転速さ[ラジアン/frame]
		const float kHipRotSpeed = 0.05f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J)) {
			worldTransforms_[PartId::kHip].rotation_.y -= kHipRotSpeed;
		}
		else if (input_->PushKey(DIK_K)) {
			worldTransforms_[PartId::kHip].rotation_.y += kHipRotSpeed;
		}
	}
	debugText_->SetPos(50, 150);
	debugText_->Printf("Root:(%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x, worldTransforms_[PartId::kRoot].translation_.y, worldTransforms_[PartId::kRoot].translation_.z);
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
	//3Dモデル描画
	for (int i = 0; i < PartId::kNumPartId; i++) {
		if (i == 0 || i == 1) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}