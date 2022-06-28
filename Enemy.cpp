#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "Enemy.h"
#include "Player.h"

//Matrix4 Enemy::CreateMatScale(Vector3 scale)
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

//Matrix4 Enemy::CreateMatRotX(Vector3 rotation)
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

//Matrix4 Enemy::CreateMatRotY(Vector3 rotation)
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

//Matrix4 Enemy::CreateMatRotZ(Vector3 rotation)
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

//Matrix4 Enemy::CreateMatTrans(Vector3 tranlation)
//{
//	行列の更新
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
//	worldTransform_.matWorld_ *= matTrans;
//
//	return matTrans;
//}

Vector3 Enemy::Velocity(Vector3 velocity, WorldTransform worldTransform)
{
	Vector3 v;
	v.x = velocity.x * worldTransform.matWorld_.m[0][0] + velocity.y * worldTransform.matWorld_.m[1][0] + velocity.z * worldTransform.matWorld_.m[2][0];
	v.y = velocity.x * worldTransform.matWorld_.m[0][1] + velocity.y * worldTransform.matWorld_.m[1][1] + velocity.z * worldTransform.matWorld_.m[2][1];
	v.z = velocity.x * worldTransform.matWorld_.m[0][2] + velocity.y * worldTransform.matWorld_.m[1][2] + velocity.z * worldTransform.matWorld_.m[2][2];

	return v;
}

void Enemy::approach(WorldTransform& worldtransform, Vector3 appSpeed)
{
	//移動(ベクトル加算)
	worldTransform_.translation_ += appSpeed;
	//既定の位置に達したら離脱
	/*if (worldTransform_.translation_.z < 0)
	{
		phase_ = Phase::Leave;
	}*/
	//発射タイマーカウントダウン
	shotTimer--;
	//指定時間に達した
	if (shotTimer < 0)
	{
		//弾を発射 
		Fire();
		//発射タイマーを初期化
		shotTimer = kFireInterval;
	}
}

void Enemy::leave(WorldTransform& worldtransform, Vector3 leaveSpeed)
{
	//移動(ベクトルを加算)
	worldTransform_.translation_ += leaveSpeed;
}

void Enemy::Initialize(Model* model/*, const Vector3& position*/)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.translation_ = { 10,2,30 };
	//ワ-ルドトランスフォームの初期化
	worldTransform_.Initialize();


	//弾を発射
	//Fire();

	//接近フェーズ初期化
	ApproachInitialize();

}

void Enemy::Update()
{
	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	////移動処理
	//移動の速さ
	float kEnemySpeed = 0.02f;
	//座標を移動させる(1フレームの移動量を足しこむ)
	worldTransform_.translation_.x -= kEnemySpeed;

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	Vector3 approachSpeed = { 0,0,0 };
	Vector3 leaveSpeed = { -0.05f,0.05f,0 };


	//switch文による実装
	/*switch (phase_)
	{

	case Phase::Approach:
	default:
		approach(worldTransform_, approachSpeed);
		break;
	case Phase::Leave:
		leave(worldTransform_, leaveSpeed);
		break;
	}*/

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

	approach(worldTransform_, approachSpeed);
	//worldTransform_.matWorld_ = matrix_->CreateMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_, worldTransform_);

	//worldTransform_.matWorld_ *= matrix_->CreateMatScale(worldTransform_.scale_,worldTransform_);
	//worldTransform_.matWorld_ *= matrix_->CreateMatRotZ(worldTransform_.rotation_, worldTransform_);
	//worldTransform_.matWorld_ *= matrix_->CreateMatRotX(worldTransform_.rotation_, worldTransform_);
	//worldTransform_.matWorld_ *= matrix_->CreateMatRotY(worldTransform_.rotation_, worldTransform_);
	//worldTransform_.matWorld_ *= matrix_->CreateMatTrans(worldTransform_.translation_, worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	//弾の速度
	const float kBulletSpeed = -0.1f;
	Vector3 velocity(0, 0, kBulletSpeed);

	////速度ベクトルを自機の向きに合わせて回転させる
	//velocity = Velocity(velocity, worldTransform_);

	/*player_->GetWorldPosition();
	GetWorldPosition();
	Vector3 velocity = {
		player_->GetWorldPosition().x - GetWorldPosition().x,
		player_->GetWorldPosition().y - GetWorldPosition().y,
		player_->GetWorldPosition().z - GetWorldPosition().z
	};

	velocity.normalize();
	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;*/

	player_->GetWorldPosition();
	GetWorldPosition();
	velocity = {
		player_->GetWorldPosition().x - GetWorldPosition().x,
		player_->GetWorldPosition().y - GetWorldPosition().y,
		player_->GetWorldPosition().z - GetWorldPosition().z
	};

	float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	velocity /= length;

	velocity *= -kBulletSpeed;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	//発射タイマーを初期化
	shotTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
	//何もしない
}