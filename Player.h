#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"

///<summary>
///自キャラ
/// </summary>

class Player {
public:
	///<summary>
	///初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	///<summary>
	///更新
	/// </summary>
	void Update();

	///<summary>
	///回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	///<summary>
	///描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	//ワールド変換データ
	WorldTransform worldTransform_;
private://メンバ変数
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力するため
	Input* input_ = nullptr;
	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	ViewProjection* viewProjection_;

	//弾
	PlayerBullet* bullet_ = nullptr;
};