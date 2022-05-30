#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include<random>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	//worldTransforms_[100].Initialize();

	//乱数シード生成
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float>dist(-5.0f, 5.0f);
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float>rotDist(0.0f, 3.14f);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float>posDist(-10.0f, 10.0f);

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		//X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = { 1.0f,1.0f,1.0f };
		//X,Y,Z軸周りの回転角を乱数で設定
		worldTransform.rotation_ = { rotDist(engine),rotDist(engine), rotDist(engine) };
		//X,Y,Z軸周りの平行移動(座標)を乱数で設定
		worldTransform.translation_ = { posDist(engine),posDist(engine) ,posDist(engine) };
		//スケーリング・回転・平行移動を合成した行列を計算してワールドトランスフォームに代入
		Matrix4 matIdentity;
		matIdentity.m[0][0] = 1;
		matIdentity.m[1][1] = 1;
		matIdentity.m[2][2] = 1;
		matIdentity.m[3][3] = 1;

		Matrix4 matScale = matIdentity;
		matScale.m[0][0] = worldTransform.scale_.x;
		matScale.m[1][1] = worldTransform.scale_.y;
		matScale.m[2][2] = worldTransform.scale_.z;
		//matIdentity.m[3][3] = 1;

		Matrix4 matRotZ = matIdentity;
		matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform.rotation_.z);

		Matrix4 matRotX = matIdentity;
		matRotX.m[1][1] = cos(worldTransform.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform.rotation_.x);

		Matrix4 matRotY = matIdentity;
		matRotY.m[0][0] = cos(worldTransform.rotation_.y);
		matRotY.m[0][2] = sin(worldTransform.rotation_.y);
		matRotY.m[2][0] = -sin(worldTransform.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform.rotation_.y);

		Matrix4 matTrans = matIdentity;
		matTrans.m[3][0] = worldTransform.translation_.x;
		matTrans.m[3][1] = worldTransform.translation_.y;
		matTrans.m[3][2] = worldTransform.translation_.z;
		matTrans.m[3][3] = 1;

		worldTransform.matWorld_ = matIdentity;
		worldTransform.matWorld_ *= matScale;

		worldTransform.matWorld_ *= matRotZ;
		worldTransform.matWorld_ *= matRotX;
		worldTransform.matWorld_ *= matRotY;

		worldTransform.matWorld_ *= matTrans;

		//行列の転送
		worldTransform.TransferMatrix();
	}


	debugCamera_ = new DebugCamera(1280, 720);

	//カメラ垂直方向視野角を設定
	degree = 10.0f;
	viewProjection_.fovAngleY = degree * 3.14f / 180;

	//アスペクトヒを設定
	//viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	/// <summary>

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_/*&debugCamera_->GetViewProjection()*/);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//worldTransforms_.TransferMatrix();
}

void GameScene::Update()
{
	debugCamera_->Update();


	////始点の移動ベクトル
	//Vector3 move = { 0,0,0 };

	////始点の移動速さ
	//const float kEyeSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_W))
	//{
	//	move = { 0,0,kEyeSpeed };
	//}
	//else if (input_->PushKey(DIK_S))
	//{
	//	move = { 0,0,-kEyeSpeed };
	//}
	////視点移動(ベクトルの加算)
	//viewProjection_.eye += move;

	////行列の再計算
	//viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);


	////注視点移動処理
	////注視点の移動ベクトル
	//move = { 0,0,0 };
	////tyy支店の移動速さ
	//const float kTargetSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_LEFT))
	//{
	//	move = { -kTargetSpeed,0,0 };
	//}
	//else if (input_->PushKey(DIK_RIGHT))
	//{
	//	move = { kTargetSpeed,0,0 };
	//}
	////注視点移動(ベクトルの加算)
	//{
	//	viewProjection_.target += move;
	//}
	////行列の再計算
	//viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	////上方向回転処理
	////上方向の回転速さ[ラジアン/flame]
	//const float kUpRotSpeed = 0.05f;
	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_SPACE))
	//{
	//	viewAngle += kUpRotSpeed;
	//	//2πを超えたら0に戻す
	//	viewAngle = fmodf(viewAngle, 3.14f * 2.0f);
	//}
	////上方向ベクトルを計算(半径1の円周上の座標)
	//viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };
	////行列の再計算
	//viewProjection_.UpdateMatrix();
	debugText_->SetPos(50, 90);
	debugText_->Printf("up(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	//Fov変更処理
	//上キーで視野角が広がる
	if (input_->PushKey(DIK_UP))
	{
		viewProjection_.fovAngleY += 0.01f;
		viewProjection_.fovAngleY = MaxNum(3.14f, viewProjection_.fovAngleY);
	}//下キーで視野角が狭まる
	else if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.fovAngleY -= 0.01f;
		viewProjection_.fovAngleY = MinNum(0.01f, viewProjection_.fovAngleY);
	}

	viewProjection_.UpdateMatrix();

	//デバッグ表示
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngle(Degree):%f", viewProjection_.fovAngleY * 180 / 3.14f);

	//クリップ距離変更処理
	//上下キーでニアクリップ距離を増減
	if (input_->PushKey(DIK_W))
	{
		viewProjection_.nearZ += 0.2f;
	}
	else if (input_->PushKey(DIK_S))
	{
		viewProjection_.nearZ -= 0.2f;
	}
	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ表示
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	/*PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, { 0,10,0 }, { 255,0,0,255 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,10,0 }, { 10,0,10 }, { 255,0,0,255 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 10,0,10 }, { 10,0,-10 }, { 255,0,0,255 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 10,0,-10 }, { 0,10,0 }, { 255,0,0,255 });*/

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_/*debugCamera_->GetViewProjection()*/, textureHandle_);
	}



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
