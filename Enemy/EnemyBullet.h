#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
class EnemyBullet 
{
  public:
	void Initialize(Model* model, const WorldTransform worldTransform, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	// ワールド弾座標を取得
	Vector3 GetBulletWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

	//アフィン用
	Affine_trans* trans = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};