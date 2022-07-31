#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "Vector3.h"

///<summary>
///レールカメラ
///</summary>
class RailCamera
{
public:
	///<summary>
	///初期化
	///</summary>
	void Initialize(Vector3 position, Vector3 rotation);

	///<summary>
	///更新
	///</summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; };

	//ワールド行列を取得
	const WorldTransform& GetWorldMatrix()const { return worldTransform_; };

	Vector3 Velocity(Vector3 velocity, Matrix4 matWorld_);
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//入力するため
	Input* input_ = nullptr;
	//デバッグテキスト
	DebugText* debugText_ = nullptr;
};