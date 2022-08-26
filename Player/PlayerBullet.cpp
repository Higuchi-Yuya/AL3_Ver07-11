#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const WorldTransform worldTransform, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = {
	  worldTransform.matWorld_.m[3][0], worldTransform.matWorld_.m[3][1],
	  worldTransform.matWorld_.m[3][2]};
	
	worldTransform_.scale_.x = 0.6;
	worldTransform_.scale_.y = 0.6;
	worldTransform_.scale_.z = 0.6;

}

void PlayerBullet::Update() 
{
	//座標を移動させる(１フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	trans->identity_matrix(worldTransform_.matWorld_);
	trans->Affine_Trans(
	  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
	  worldTransform_.translation_);
	
	worldTransform_.TransferMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

Vector3 PlayerBullet::GetBulletWorldPosition() 
{ 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::OnCollision() { isDead_ = true; }
