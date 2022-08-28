#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

float Pi = 3.141592;
float Enemy::Rad(float range)
{
	float range_ = range;
	float rad;
	rad = range_ * Pi / 180;

	return rad;
}

Vector3 Enemy::Leap(const Vector3& start, const Vector3& end, float t)
{
	return (1 - t) * start + t * end;
}

void Enemy::Initialize(Model* model ,Vector3& position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("purple.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//初期座標の設定
	worldTransform_.translation_ = {position.x, position.y, position.z};


	enemy_initpos_ = worldTransform_.translation_;
	//パターンを初期化
	//pattern_ = Pattern::Straight;
}

void Enemy::Update() 
{
	if (isDead_==false) 
	{
		//敵移動ベクトル
		Vector3 move = { 0, 0, 0 };
		switch (pattern_) {

		// パターン真っすぐ
		case Pattern::Straight:
			
			//フェーズ処理
			switch (phase_) {
			case Phase::Approach:
			default:
				Approach_Update();
				
				break;

			case Phase::Laeve:
				Laeve_Update();
				break;
			}

			trans->identity_matrix(worldTransform_.matWorld_);
			trans->Affine_Trans(
			  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
			  worldTransform_.translation_);

			worldTransform_.TransferMatrix();

			break;

		//パターン煽り
		case Pattern::ProvocationWay5:
			//フェーズ処理
			switch (phase_) {
			case Phase::Approach:
			default:
				ProvocationWay5_Update();

				break;

			case Phase::Laeve:
				Laeve_Update();
				break;
			}

			trans->identity_matrix(worldTransform_.matWorld_);
			trans->Affine_Trans(
				worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
				worldTransform_.translation_);

			worldTransform_.TransferMatrix();

			break;
		
		//パターン煽り(Way3)
		case Pattern::ProvocationWay3:
			//フェーズ処理
			switch (phase_) {
			case Phase::Approach:
			default:
				ProvocationWay3_Update();

				break;

			case Phase::Laeve:
				Laeve_Update();
				break;
			}

			trans->identity_matrix(worldTransform_.matWorld_);
			trans->Affine_Trans(
				worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
				worldTransform_.translation_);

			worldTransform_.TransferMatrix();

			break;

		//パターン煽り(Way5MoveX)
		case Pattern::ProvocationWay5MoveX:
			//フェーズ処理
			switch (phase_) {
			case Phase::Approach:
			default:
				ProvocationWay5MoveX_Update();

				break;

			case Phase::Laeve:
				Laeve_Update();
				break;
			}

			trans->identity_matrix(worldTransform_.matWorld_);
			trans->Affine_Trans(
				worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
				worldTransform_.translation_);

			worldTransform_.TransferMatrix();

			break;

		//パターン煽り(Way3MoveX)
		case Pattern::ProvocationWay3MoveX:
			//フェーズ処理
			switch (phase_) {
			case Phase::Approach:
			default:
				ProvocationWay3MoveX_Update();

				break;

			case Phase::Laeve:
				Laeve_Update();
				break;
			}

			trans->identity_matrix(worldTransform_.matWorld_);
			trans->Affine_Trans(
				worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
				worldTransform_.translation_);

			worldTransform_.TransferMatrix();

			break;
		}
		

	}

}
	



void Enemy::Draw(const ViewProjection& viewProjection) 
{
	if(isDead_==false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
	
}



void Enemy::Approach_Initialize() 
{

}


void Enemy::Approach_Update() 
{
	//敵移動ベクトル
	Vector3 move = {0, 0, 0};
	//移動(ベクトルを加算)
	move = {0, 0, -enemy_speed_z};
	worldTransform_.translation_.z += move.z;

	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < player_->GetWorldPosition().z + 50) {
		phase_ = Phase::Laeve;
	}

	//発射タイマーをデクリメント
	fireTimer_--;

	//規定時間に到達したら
	if (fireTimer_ <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Laeve_Update() 
{
	//敵移動ベクトル
	Vector3 move = {0, 0, 0};
	//移動(ベクトルを加算)
	move = {-enemy_speed_x, +enemy_speed_y, 0};
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
}

void Enemy::ProvocationWay5_Update()
{
	//敵移動ベクトル
	Vector3 move = { 0, 0, 0 };
	// 回転ベクトル
	Vector3 rotate = { 0,0,0 };
	
	//移動(ベクトルを加算)
	move = { 0, 0, -enemy_speed_z };
	worldTransform_.translation_.z += move.z;

	

	if (rotate_flag == true)
	{
		// 回転ベクトルを加算
		rotate = { 0,0,+enemy_rotation_speed.z };
		worldTransform_.rotation_ += rotate;
	}
	else if (rotate_flag == false)
	{
		// 回転ベクトルを加算
		rotate = { 0,0,-enemy_rotation_speed.z };
		worldTransform_.rotation_ += rotate;
	}
	if (worldTransform_.rotation_.z >= 0.4) {
		rotate_flag = false;
	}
	else if (worldTransform_.rotation_.z <= -0.4) {
		rotate_flag = true;
	}

	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < player_->GetWorldPosition().z + 50) {
		phase_ = Phase::Laeve;
	}

	//発射タイマーをデクリメント
	fireTimer_--;

	//規定時間に到達したら
	if (fireTimer_ <= 0) {
		//弾を発射
		Way5Fire();
		//発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::ProvocationWay3_Update()
{
	//敵移動ベクトル
	Vector3 move = { 0, 0, 0 };
	// 回転ベクトル
	Vector3 rotate = { 0,0,0 };

	//移動(ベクトルを加算)
	move = { 0, 0, -enemy_speed_z };
	worldTransform_.translation_.z += move.z;
	

	if (rotate_flag == true)
	{
		// 回転ベクトルを加算
		rotate = { 0,0,+enemy_rotation_speed.z };
		worldTransform_.rotation_ += rotate;
	}
	else if (rotate_flag == false)
	{
		// 回転ベクトルを加算
		rotate = { 0,0,-enemy_rotation_speed.z };
		worldTransform_.rotation_ += rotate;
	}
	if (worldTransform_.rotation_.z >= 0.4) {
		rotate_flag = false;
	}
	else if (worldTransform_.rotation_.z <= -0.4) {
		rotate_flag = true;
	}

	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < player_->GetWorldPosition().z + 50) {
		phase_ = Phase::Laeve;
	}

	//発射タイマーをデクリメント
	fireTimer_--;

	//規定時間に到達したら
	if (fireTimer_ <= 0) {
		//弾を発射
		Way3Fire();
		//発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::ProvocationWay5MoveX_Update()
{
	//敵移動ベクトル
	Vector3 move = { 0, 0, 0 };
	// 回転ベクトル
	Vector3 rotate = { 0,0,0 };

	//移動(ベクトルを加算)
	move = { 0, 0, -enemy_speed_z };
	worldTransform_.translation_.z += move.z;

	if (moveXFlag == true)
	{
		move = { +enemy_speed_x,0,0 };
		worldTransform_.translation_ += move;
	}
	else if (moveXFlag == false)
	{
		move = { -enemy_speed_x,0,0 };
		worldTransform_.translation_ += move;
	}
	if (worldTransform_.translation_.x >= enemy_initpos_.x + enemy_limit_.x)
	{
		moveXFlag = false;
	}
	else if (worldTransform_.translation_.x <= enemy_initpos_.x + -enemy_limit_.x)
	{
		moveXFlag = true;
	}

	if (rotate_flag == true)
	{
		// 回転ベクトルを加算
		rotate = { 0,0,+enemy_rotation_speed.z };
		worldTransform_.rotation_ += rotate;
	}
	else if (rotate_flag == false)
	{
		// 回転ベクトルを加算
		rotate = { 0,0,-enemy_rotation_speed.z };
		worldTransform_.rotation_ += rotate;
	}
	if (worldTransform_.rotation_.z >= 0.4) {
		rotate_flag = false;
	}
	else if (worldTransform_.rotation_.z <= -0.4) {
		rotate_flag = true;
	}

	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < player_->GetWorldPosition().z + 50) {
		phase_ = Phase::Laeve;
	}

	//発射タイマーをデクリメント
	fireTimer_--;

	//規定時間に到達したら
	if (fireTimer_ <= 0) {
		//弾を発射
		Way5Fire();
		//発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::ProvocationWay3MoveX_Update()
{
	//敵移動ベクトル
	Vector3 move = { 0, 0, 0 };
	// 回転ベクトル
	Vector3 rotate = { 0,0,0 };

	//移動(ベクトルを加算)
	move = { 0, 0, -enemy_speed_z };
	worldTransform_.translation_.z += move.z;

	if (moveXFlag == true)
	{
		move = { +enemy_speed_x,0,0 };
		worldTransform_.translation_ += move;
	}
	else if (moveXFlag == false)
	{
		move = { -enemy_speed_x,0,0 };
		worldTransform_.translation_ += move;
	}
	if (worldTransform_.translation_.x >= enemy_initpos_.x + enemy_limit_.x)
	{
		moveXFlag = false;
	}
	else if (worldTransform_.translation_.x <= enemy_initpos_.x + -enemy_limit_.x)
	{
		moveXFlag = true;
	}

	if (rotate_flag == true)
	{
		// 回転ベクトルを加算
		rotate = { 0,0,+enemy_rotation_speed.z };
		worldTransform_.rotation_ += rotate;
	}
	else if (rotate_flag == false)
	{
		// 回転ベクトルを加算
		rotate = { 0,0,-enemy_rotation_speed.z };
		worldTransform_.rotation_ += rotate;
	}
	if (worldTransform_.rotation_.z >= 0.4) {
		rotate_flag = false;
	}
	else if (worldTransform_.rotation_.z <= -0.4) {
		rotate_flag = true;
	}

	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < player_->GetWorldPosition().z + 50) {
		phase_ = Phase::Laeve;
	}

	//発射タイマーをデクリメント
	fireTimer_--;

	//規定時間に到達したら
	if (fireTimer_ <= 0) {
		//弾を発射
		Way3Fire();
		//発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}


void Enemy::Fire() {
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 0.2f;
	Affine_trans trans;
	Vector3 enemy_pos = GetWorldPosition();
	//Vector3 enemy_pos = worldTransform_.translation_;
	Vector3 player_pos = player_->GetWorldPosition();
	Vector3 velocity;

	velocity= player_pos - enemy_pos;

	velocity.normalize();
	velocity *= kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転させる
	//velocity = velocity * worldTransform_.matWorld_;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_, velocity);

	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::Way5Fire()
{
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 0.2f;
	Affine_trans trans;
	Vector3 enemy_pos = GetWorldPosition();
	//Vector3 enemy_pos = worldTransform_.translation_;
	const int bullet_size = 5;
	Vector3 player_pos = player_->GetWorldPosition();
	Vector3 velocity[bullet_size];
	Vector3 rotation[bullet_size] =
	{
		{0,0,0},
		{0,Rad(6),0},
		{0,Rad(-6),0},
		{0,Rad(12),0},
		{0,Rad(-12),0},
	};
	Matrix4 Rot[bullet_size];

	for (int i = 0; i < bullet_size; i++)
	{
		velocity[i] = player_pos - enemy_pos;
		trans.rotate(Rot[i], rotation[i]);

		trans.Vec3conversion_W_Notincluded(velocity[i], Rot[i]);
		velocity[i].normalize();
		velocity[i] *= kBulletSpeed;
	}

	//速度ベクトルを自機の向きに合わせて回転させる
	//velocity = velocity * worldTransform_.matWorld_;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet[bullet_size] =
	{
		std::make_unique<EnemyBullet>(),
		std::make_unique<EnemyBullet>(),
		std::make_unique<EnemyBullet>(),
		std::make_unique<EnemyBullet>(),
		std::make_unique<EnemyBullet>(),
	};
	for (int i = 0; i < bullet_size; i++)
	{
		newBullet[i]->Initialize(model_, worldTransform_, velocity[i]);

		//弾を登録する
		gameScene_->AddEnemyBullet(newBullet[i]);
	}
}

void Enemy::Way3Fire()
{
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 0.2f;
	Affine_trans trans;
	Vector3 enemy_pos = GetWorldPosition();
	//Vector3 enemy_pos = worldTransform_.translation_;
	const int bullet_size = 3;
	Vector3 player_pos = player_->GetWorldPosition();
	Vector3 velocity[bullet_size];
	Vector3 rotation[bullet_size] =
	{
		{0,0,0},
		{0,Rad(9),0},
		{0,Rad(-9),0},
	};
	Matrix4 Rot[bullet_size];

	for (int i = 0; i < bullet_size; i++)
	{
		velocity[i] = player_pos - enemy_pos;
		trans.rotate(Rot[i], rotation[i]);

		trans.Vec3conversion_W_Notincluded(velocity[i], Rot[i]);
		velocity[i].normalize();
		velocity[i] *= kBulletSpeed;
	}

	//速度ベクトルを自機の向きに合わせて回転させる
	//velocity = velocity * worldTransform_.matWorld_;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet[bullet_size] =
	{
		std::make_unique<EnemyBullet>(),
		std::make_unique<EnemyBullet>(),
		std::make_unique<EnemyBullet>(),
		
	};
	for (int i = 0; i < bullet_size; i++)
	{
		newBullet[i]->Initialize(model_, worldTransform_, velocity[i]);

		//弾を登録する
		gameScene_->AddEnemyBullet(newBullet[i]);
	}
}

void Enemy::HomingFire()
{
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 0.3f;
	Affine_trans trans;
	Vector3 enemy_pos = GetWorldPosition();
	//Vector3 enemy_pos = worldTransform_.translation_;
	Vector3 player_pos = player_->GetWorldPosition();
	Vector3 velocity;

	velocity = player_pos - enemy_pos;

	velocity.normalize();
	velocity *= kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転させる
	//velocity = velocity * worldTransform_.matWorld_;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_, velocity);

	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

Vector3 Enemy::GetWorldPosition() 
{ 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



void Enemy::OnCollision() 
{ isDead_ = true; }

