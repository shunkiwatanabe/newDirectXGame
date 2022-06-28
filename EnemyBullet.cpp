#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "EnemyBullet.h"

//Matrix4 EnemyBullet::CreateMatScale(Vector3 scale)
//{
//	//行列の更新
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matScale = matIdentity;
//	matScale.m[0][0] = worldTransform_.scale_.x;
//	matScale.m[1][1] = worldTransform_.scale_.y;
//	matScale.m[2][2] = worldTransform_.scale_.z;
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matScale;
//	return matScale;
//}

//Matrix4 EnemyBullet::CreateMatRotX(Vector3 rotation)
//{
//	//行列の更新
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matRotX = matIdentity;
//	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
//	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
//	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
//	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matRotX;
//
//	return matRotX;
//}

//Matrix4 EnemyBullet::CreateMatRotY(Vector3 rotation)
//{
//	//行列の更新
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matRotY = matIdentity;
//	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
//	matRotY.m[0][2] = sin(worldTransform_.rotation_.y);
//	matRotY.m[2][0] = -sin(worldTransform_.rotation_.y);
//	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matRotY;
//
//	return matRotY;
//}

//Matrix4 EnemyBullet::CreateMatRotZ(Vector3 rotation)
//{
//	//行列の更新
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matRotZ = matIdentity;
//	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
//	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
//	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
//	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matRotZ;
//
//	return matRotZ;
//}

//Matrix4 EnemyBullet::CreateMatTrans(Vector3 tranlation)
//{
//	//行列の更新
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matTrans = matIdentity;
//	matTrans.m[3][0] = worldTransform_.translation_.x;
//	matTrans.m[3][1] = worldTransform_.translation_.y;
//	matTrans.m[3][2] = worldTransform_.translation_.z;
//	matTrans.m[3][3] = 1;
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matTrans;
//
//	return matTrans;
//}



void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("blue.png");

	//ワ-ルドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	//座標を移動させる(1フレームの移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	//行列の更新
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	Matrix4 matScale = matIdentity;
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;

	Matrix4 matRotZ = matIdentity;
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	Matrix4 matRotX = matIdentity;
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);

	Matrix4 matRotY = matIdentity;
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = -sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);

	Matrix4 matTrans = matIdentity;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1;

	worldTransform_.matWorld_ = matIdentity;
	worldTransform_.matWorld_ *= matScale;

	worldTransform_.matWorld_ *= matRotZ;
	worldTransform_.matWorld_ *= matRotX;
	worldTransform_.matWorld_ *= matRotY;

	worldTransform_.matWorld_ *= matTrans;

	worldTransform_.TransferMatrix();
	//worldTransform_.matWorld_ = matrix_->CreateMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_, worldTransform_);

	/*if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}*/

}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}