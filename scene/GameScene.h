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
//#include<random>

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

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 最小値出す
	/// </summary>
	float MinNum(float num, float num2)
	{
		if (num < num2)
		{
			return num;
		}
		return num2;
	}

	/// <summary>
	/// 最大値出す
	/// </summary>
	float MaxNum(float num, float num2)
	{
		if (num > num2)
		{
			return num;
		}
		return num2;
	}
	
	/// <summary>
	/// 最小値,最大値を超えないようにする関数
	/// </summary>
	float Clamp(float min, float max, float num)
	{
		if (num < min)
		{
			num = min;
		}
		else if (num > max)
		{
			num = max;
		}
		return num;
	}

	//パーツID
	enum PartId
	{
		kRoot,	//大元
		kSpine,	//脊椎
		kChest,	//胸
		kHead,	//頭
		kArmL,	//左腕
		kArmR,	//右腕
		kHip,	//尻
		kLegL,	//左足
		kLegR,	//右足

		kNumPartId
	};


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	WorldTransform worldTransforms_[9];
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//ラジアン変換の角度
	//float degree;

	////乱数シード生成
	//std::random_device seed_gen;
	////メルセンヌ・ツイスターの乱数エンジン
	//std::mt19937_64 engine(seed_gen());
	////乱数範囲の指定
	//std::uniform_real_distribution<float>dist(-5.0f, 5.0f);
	///// <summary>
	/// ゲームシーン用
	/// </summary>
};
