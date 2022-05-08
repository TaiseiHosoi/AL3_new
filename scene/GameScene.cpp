#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include"DebugCamera.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete debugCamera_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	debugCamera_ = new DebugCamera(1280, 720);

	textureHandle_ = TextureManager::Load("mario.jpg");

	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//軸方向表示有効
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	
	//X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {1.0,1.0,1.0};
	//スケーリング行列を宣言
	Matrix4 matScale;

	matScale.m[0][0] = 2.0f;
	matScale.m[0][1] = 0.0f;
	matScale.m[0][2] = 0.0f;
	matScale.m[0][3] = 0.0f;

	matScale.m[1][0] = 0.0f;
	matScale.m[1][1] = 2.0f;
	matScale.m[1][2] = 2.0f;
	matScale.m[1][3] = 2.0f;

	matScale.m[2][0] = 0.0f;
	matScale.m[2][1] = 0.0f;
	matScale.m[2][2] = 2.0f;
	matScale.m[2][3] = 0.0f;

	matScale.m[3][0] = 0.0f;
	matScale.m[3][1] = 0.0f;
	matScale.m[3][2] = 0.0f;
	matScale.m[3][3] = 2.0f;

	

}

void GameScene::Update() { debugCamera_->Update(); }

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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex[c_[i][0]], vertex[c_[i][1]],Vector4(1,1,1,1));
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
