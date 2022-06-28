#pragma once

#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "EnemyBullet.h"
#include "Matrix4.h"

class Player;

///<summary>
///敵
/// </summary>
class Enemy {
public:
	///<>summary
	///初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="mposition">初期座標</param>
	void Initialize(Model* model/*, const Vector3& position*/);

	///<>summary
	///更新
	/// </summary>
	void Update();

	///<>summary
	///描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	Vector3 Velocity(Vector3 velocity, WorldTransform worldTransform);

	void approach(WorldTransform& worldtransform, Vector3 appSpeed);
	void leave(WorldTransform& worldtransform, Vector3 leaveSpeed);

	void SetPlayer(Player* player) { player_ = player; };

	Vector3 GetWorldPosition();

	///<>summary
	///弾発射
	/// </summary>
	void Fire();

	//発射感覚
	static const int kFireInterval = 60;

	//接近フェーズ初期化
	void ApproachInitialize();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルのポインタ(借りてくるやつ)
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力するため
	Input* input_ = nullptr;

	Matrix4* matrix_ = nullptr;

	enum class Phase {
		Approach,	//接近する
		Leave,		//離脱する
	};
	Phase phase_ = Phase::Approach;

	std::list < std::unique_ptr < EnemyBullet >>bullets_;

	//発射タイマー
	int32_t shotTimer = 0;

	Player* player_ = nullptr;
};