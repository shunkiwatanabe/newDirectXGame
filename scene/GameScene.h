#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include"Player.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "Vector3.h"
#include <memory>

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

	///<summary>
	///衝突判定と応答
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	WorldTransform worldTransform_;
	DebugCamera* debugCanera_ = nullptr;

	uint32_t textureHandle_ = 0;


	Model* model_ = nullptr;
	ViewProjection viewProjection_;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	EnemyBullet* enemyBullet_ = nullptr;
	Skydome* skydome_ = nullptr;
	//RailCamera* railCamera_ = nullptr;
	std::unique_ptr<RailCamera>railCamera_;
	//自キャラの半径
	float playerRadius = 2.0f;
	float playerBulletRadius = 1.0f;
	//敵弾の半径
	float enemyRadius = 2.0f;
	float enemyBulletRadius = 1.0f;

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//3Dモデル描画
	

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
