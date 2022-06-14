#pragma once

#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"

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

	Matrix4 CreateMatScale(Vector3 scale);
	Matrix4 CreateMatRotX(Vector3 rotation);
	Matrix4 CreateMatRotY(Vector3 rotation);
	Matrix4 CreateMatRotZ(Vector3 rotation);
	Matrix4 CreateMatTrans(Vector3 translation);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルのポインタ(借りてくるやつ)
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};