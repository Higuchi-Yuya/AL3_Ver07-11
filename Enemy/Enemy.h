#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
#include "EnemyBullet.h"

enum class Phase {
	Approach, //接近する
	Laeve,    //離脱する
};

class Player;

class GameScene;

class Enemy
{
  public:
	//敵の基本的な関数
	void Initialize(Model* model, Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	//接近フェーズ関連の関数
	void Approach_Initialize();
	void Approach_Update();

	//離脱フェーズ関連の関数
	void Laeve_Update();

	// 敵キャラに自キャラのアドレスを渡す
	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//弾の発射関数
	void Fire();

	// ワールド座標を取得する
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	bool IsDead() const { return isDead_; }

  public:
	//発射間隔
	static const int kFireInterval = 60;
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	float enemy_speed_x = 0.1f;
	float enemy_speed_y = 0.1f;
	float enemy_speed_z = 0.05f;
	
	//発射タイマー
	int32_t fireTimer_ = 60;

	//アフィン用
	Affine_trans* trans = nullptr;

	//デスフラグ
	bool isDead_ = false;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approach;

	// 自キャラ
	Player* player_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;
};
