#include "RailCamera.h"

Vector3 RailCamera::Velocity(Vector3 velocity, Matrix4 matWorld_)
{
	Vector3 v;
	v.x = velocity.x * matWorld_.m[0][0] + velocity.y * matWorld_.m[1][0] + velocity.z * matWorld_.m[2][0];
	v.y = velocity.x * matWorld_.m[0][1] + velocity.y * matWorld_.m[1][1] + velocity.z * matWorld_.m[2][1];
	v.z = velocity.x * matWorld_.m[0][2] + velocity.y * matWorld_.m[1][2] + velocity.z * matWorld_.m[2][2];

	return v;
}

void RailCamera::Initialize(Vector3 position, Vector3 rotation)
{
	//ワールドトランスフォームの初期化
	//worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	debugText_ = DebugText::GetInstance();
}

void RailCamera::Update()
{
	Vector3 move = { 0,0,0 };

	//移動スピード
	float kRailSpeed = 0.1f;

	//座標を移動させる(1フレームの移動量を足しこむ)
	worldTransform_.translation_.x -= kRailSpeed;

	//回転のスピード
	const float rotateSpeed = 0.05f;
	//角度を加算
	worldTransform_.rotation_.y += rotateSpeed;

	

	viewProjection_.eye = worldTransform_.translation_;

	//移動(ベクトル加算)
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
	//ワールドトランスフォームを更新
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

	//ワールド行列の平行移動成分を取得(ワールド座標)
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = Velocity(forward, worldTransform_.matWorld_);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target.x = viewProjection_.eye.x + forward.x;
	viewProjection_.target.y = viewProjection_.eye.y + forward.y;
	viewProjection_.target.z = viewProjection_.eye.z + forward.z;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 1);
	//レールカメラの回転を反映(れーるかめらの上方ベクトル)
	viewProjection_.up = Velocity(up, worldTransform_.matWorld_);
	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	////行列の更新
	//Matrix4 viewMatIdentity;
	//viewMatIdentity.m[0][0] = 1;
	//viewMatIdentity.m[1][1] = 1;
	//viewMatIdentity.m[2][2] = 1;
	//viewMatIdentity.m[3][3] = 1;

	//Matrix4 matEye = viewMatIdentity;
	//matEye.m[0][0] = viewProjection_.eye.x;
	//matEye.m[1][1] = viewProjection_.eye.y;
	//matEye.m[2][2] = viewProjection_.eye.z;

	//Matrix4 matTargetZ = viewMatIdentity;
	//matTargetZ.m[0][0] = cos(viewProjection_.target.z);
	//matTargetZ.m[0][1] = sin(viewProjection_.target.z);
	//matTargetZ.m[1][0] = -sin(viewProjection_.target.z);
	//matTargetZ.m[1][1] = cos(viewProjection_.target.z);

	//Matrix4 matTargetX = viewMatIdentity;
	//matTargetX.m[1][1] = cos(viewProjection_.target.x);
	//matTargetX.m[1][2] = sin(viewProjection_.target.x);
	//matTargetX.m[2][1] = -sin(viewProjection_.target.x);
	//matTargetX.m[2][2] = cos(viewProjection_.target.x);

	//Matrix4 matTargetY = viewMatIdentity;
	//matTargetY.m[0][0] = cos(viewProjection_.target.y);
	//matTargetY.m[0][2] = sin(viewProjection_.target.y);
	//matTargetY.m[2][0] = -sin(viewProjection_.target.y);
	//matTargetY.m[2][2] = cos(viewProjection_.target.y);

	//Matrix4 matUp = viewMatIdentity;
	//matUp.m[3][0] = viewProjection_.up.x;
	//matUp.m[3][1] = viewProjection_.up.y;
	//matUp.m[3][2] = viewProjection_.up.z;
	//matUp.m[3][3] = 1;

	//viewProjection_.matView = viewMatIdentity;
	//viewProjection_.matView *= matEye;

	//viewProjection_.matView *= matTargetZ;
	//viewProjection_.matView *= matTargetX;
	//viewProjection_.matView *= matTargetY;

	//viewProjection_.matView *= matUp;

	//viewProjection_.TransferMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	debugText_->SetPos(20, 100);
	debugText_->Printf("pos:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}