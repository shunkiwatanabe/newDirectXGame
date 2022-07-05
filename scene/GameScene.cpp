﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "Enemy.h"
#include <memory>
#include <list>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	delete debugCamera_;

	//自キャラの解放
	delete player_;
	//敵の解放
	delete enemy_;

	delete enemyBullet_;

	delete skydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	//カメラ始点座標を設定
	viewProjection_.eye = { 0,0,-10 };
	////カメラ注視点座標を設定
	//viewProjection_.target = { 0,0,0 };

	viewProjection_.Initialize();
	worldTransform_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵の生成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initialize(model_);

	enemy_->SetPlayer(player_);

	enemyBullet_ = new EnemyBullet();

	//天球の生成
	skydome_ = new Skydome();
	//std::list<std::unique_ptr<Skydome>>skydome_;
	

	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	//レールカメラの生成
	railcamera_ = new RailCamera();
	//レールカメラの初期化
	railcamera_->Initialize(worldTransform_,worldTransform_.rotation_);

	debugCamera_ = new DebugCamera(1280, 720);

	

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update()
{
	debugCamera_->Update();

	//自キャラの更新
	player_->Update();
	player_->Rotate();

	//敵の更新
	/*if (enemy_ != NULL)*/ enemy_->Update();

	skydome_->Update();

	debugCamera_->Update();

	//始点の移動ベクトル
	Vector3 move = { 0,0,0 };

	//始点の移動速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_UP))
	{
		move = { 0,0,kEyeSpeed };
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move = { 0,0,-kEyeSpeed };
	}
	//視点移動(ベクトルの加算)
	viewProjection_.eye += move;

	//行列の再計算
	viewProjection_.UpdateMatrix();

	////注視点の移動ベクトル
	//move = { 0,0,0 };
	////tyy支店の移動速さ
	//const float kTargetSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_LEFT))
	//{
	//	move = { -kTargetSpeed,0,0 };
	//}
	//else if (input_->PushKey(DIK_RIGHT))
	//{
	//	move = { kTargetSpeed,0,0 };
	//}
	////注視点移動(ベクトルの加算)
	//{
	//	viewProjection_.target += move;
	//}
	////行列の再計算
	//viewProjection_.UpdateMatrix();

	CheckAllCollision();
}

void GameScene::CheckAllCollision()
{
	Vector3 posA, posB;

	//自弾リストの取得8
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = { player_->GetWorldPosition() };

	//自キャラと敵弾の当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		//敵弾の座標
		posB = { bullet->GetWorldPosition() };
		float x = (posA.x - posB.x) * (posA.x - posB.x);
		float y = (posA.y - posB.y) * (posA.y - posB.y);
		float z = (posA.z - posB.z) * (posA.z - posB.z);
		//球と球の交差判定
		if (x + y + z <= (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius))
		{
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラの座標
	posA = { enemy_->GetWorldPosition() };

	//自キャラと敵弾の当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		//自弾の座標
		posB = { bullet->GetWorldPosition() };
		float x = (posA.x - posB.x) * (posA.x - posB.x);
		float y = (posA.y - posB.y) * (posA.y - posB.y);
		float z = (posA.z - posB.z) * (posA.z - posB.z);
		//球と球の交差判定
		if (x + y + z <= (playerBulletRadius + enemyRadius) * (playerBulletRadius + enemyRadius))
		{
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			posA = { bullet->GetWorldPosition() };
			posB = { enemyBullet->GetWorldPosition() };
			float x = (posA.x - posB.x) * (posA.x - posB.x);
			float y = (posA.y - posB.y) * (posA.y - posB.y);
			float z = (posA.z - posB.z) * (posA.z - posB.z);

			//球と球の交差判定
			if (x + y + z <= (playerBulletRadius + enemyBulletRadius) * (playerBulletRadius + enemyBulletRadius))
			{
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion
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
	skydome_->Draw(viewProjection_);
	//自キャラの描画
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	//天球の描画
	skydome_->Draw(viewProjection_);

	//model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
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
