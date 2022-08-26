#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const WorldTransform worldTransform, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");


	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = {
	  worldTransform.matWorld_.m[3][0], 
	  worldTransform.matWorld_.m[3][1],
	  worldTransform.matWorld_.m[3][2]};


	worldTransform_.scale_.x = 0.6;
	worldTransform_.scale_.y = 0.6;
	worldTransform_.scale_.z = 0.6;

	// 打つ方向に向けてオブジェクトを回転させる
	
	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// Y軸周りに-θy回す回転行列を計算
	Matrix4 RotY;
	trans->rotateY(RotY, -worldTransform_.rotation_.y);
	// velosity_に回転行列を掛け算してvelosityZを求める
	Vector3 velosityZ=velocity_;
	trans->Vec3conversion_W_Notincluded(velosityZ, RotY);

	// X軸周り角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velosityZ.y, velosityZ.z);

}

void EnemyBullet::Update() 
{

	// 座標を移動させる(１フレーム分の移動量を足しこむ)
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

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetBulletWorldPosition() 
{ 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::OnCollision() { isDead_ = true; }
