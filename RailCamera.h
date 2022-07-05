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
	void Initialize(WorldTransform worldTransform, Vector3 radian);

	///<summary>
	///更新
	///</summary>
	void Update();
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