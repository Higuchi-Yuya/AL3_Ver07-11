#include "Player.h"
#include "RailCamera.h"
#include <AxisIndicator.h>
//using namespace MathUtility;



//RailCamera* railCamera = nullptr;
float VpWidth = (float)WinApp::GetInstance()->kWindowWidth;
float VpHeight = (float)WinApp::GetInstance()->kWindowHeight;

Player::~Player() 
{
	
}

void Player::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("star/star_player.png");
	
	audio_ = Audio::GetInstance();

	// 音声データ読み込み
	shotSeSDHandle_ = audio_->LoadWave("SE/Shot.wav");

	// レティクル用テクスチャ取得
	uint32_t textureReticle_ = TextureManager::Load("2DReticle.png");

	// スプライト生成
	Vector2 pos = {640, 360};
	Vector4 color = {1, 1, 1, 1};
	Vector2 anker = {0.5f, 0.5f};
	Vector2 size = { 150,150 };
	sprite2DReticle_.reset(Sprite::Create(textureReticle_, pos, color, anker));
	sprite2DReticle_.get()->SetSize(size);

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ライフのリセット
	life = 5;

	// デスフラグのリセット
	isDead_ = false;

	//ワールド変換を初期化
	worldTransform_.translation_.x = 0;
	worldTransform_.translation_.y = 0;
	worldTransform_.translation_.z = 50.0f;

	worldTransform_.Initialize();
	
	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	// プレイヤーの弾をリセット
	bullets_.clear();
	
}

void Player::Update() {
	if (isDead_ == false)
	{
		//デスフラグの立った弾を削除
		bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

		//回転処理
		Rotation();

		//移動処理
		Move();

		//キャラクターの攻撃処理

		Attack();

		//弾更新
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
			bullet->Update();
		}

		//行列の更新および転送
		Trans_Update();
	}
	//デバッグテキスト
	Debug_Text();
}

void Player::Attack() {
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	
	if (joyState.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER)//input_->PushKey(DIK_Y)) {
	{
		timer--;
		shotSeFlag = true;
		if (timer<=0) {
			if (shotSeFlag == true)
			{
				shotSeVoiceHandle_ = audio_->PlayWave(shotSeSDHandle_,false,0.6);
				shotSeFlag = false;
			}

			//弾の速度
			const float kBulletSpeed = 0.3f;
			Vector3 velocity;
			Affine_trans trans;

			Vector3 player_pos = GetWorldPosition();
			Vector3 reticle_pos = {
			  worldTransform3DReticle_.matWorld_.m[3][0],
			  worldTransform3DReticle_.matWorld_.m[3][1],
			  worldTransform3DReticle_.matWorld_.m[3][2]};

			// 自機から照準オブジェクトへのベクトル
			velocity = reticle_pos - player_pos;
			velocity.normalize();
			velocity *= kBulletSpeed;

			//速度ベクトルを自機の向きに合わせて回転させる
			// velocity = velocity * worldTransform_.matWorld_;

			//弾を生成し、初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(model_, worldTransform_, velocity);

			//弾を登録する
			bullets_.push_back(std::move(newBullet));

			timer = 40;
		}
		
	}
	
}

void Player::Draw(ViewProjection viewProjection) {
	if (isDead_ == false)
	{
		model_->Draw(worldTransform_, viewProjection, textureHandle_);

		// 3Dレティクルを描画
		ReticleDraw(viewProjection);

		//弾描画
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
			bullet->Draw(viewProjection);
		}
	}
}

Vector3 Player::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() 
{
	life -= 1;
	if (life <= 0)
	{
		isDead_ = true;
	}
}

void Player::SetWorldTransformParent(WorldTransform *worldtransform) 
{
	worldTransform_.parent_ = worldtransform;
}

void Player::Move() 
{
	Vector3 move = {0, 0, 0};
	constexpr float MoveLimitX = 33;
	constexpr float MoveLimitY = 18;

	// ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * character_speed_x;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * character_speed_y;

	}
	worldTransform_.translation_ += move;


	//キャラクターの移動処理
	if (input_->PushKey(DIK_LEFT)) {
		move = {-character_speed_x, 0, 0};

		worldTransform_.translation_.x += move.x;

	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {character_speed_x, 0, 0};

		worldTransform_.translation_.x += move.x;

	} else if (input_->PushKey(DIK_UP)) {
		move = {0, character_speed_y, 0};

		worldTransform_.translation_.y += move.y;
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -character_speed_y, 0};

		worldTransform_.translation_.y += move.y;
	}

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -MoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +MoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -MoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +MoveLimitY);
}

void Player::Rotation() 
{ 
	Vector3 rotate = {0, 0, 0};

	//キャラクターの旋回移動
	if (input_->PushKey(DIK_U)) {
		rotate = { 0, 0, +rotation_speed_y };
		worldTransform_.rotation_ += rotate;
	}
	if (input_->PushKey(DIK_I)) {
		rotate = { 0, 0, -rotation_speed_y };
		worldTransform_.rotation_ += rotate;
	}
}

void Player::Trans_Update() 
{
	Affine_trans* trans = nullptr;
	
	//ベクトルの加算
	trans->identity_matrix(worldTransform_.matWorld_);

	

	trans->Affine_Trans(
	  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
	  worldTransform_.translation_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	//worldTransform_.matWorld_ *= railCamera->GetWorldTransform().matWorld_;
	//worldTransform_.matWorld_.m[3][2] += 50.0f;
	//行列の転送
	worldTransform_.TransferMatrix();
}

void Player::Debug_Text() 
{
	//デバッグテキスト表示
	//debugText_->SetPos(50, 100);
	////debugText_->Printf("Life:%d", life);
	//debugText_->Printf(
	//  "Pos:(%f,%f,%f)", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	//  worldTransform_.matWorld_.m[3][2]);
	
	//debugText_->SetPos(50, 300);
	//debugText_->Printf(
	//  "Pos:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	//  worldTransform_.rotation_.z);
}

void Player::ReticleUpdate(ViewProjection viewProjection) 
{
	Affine_trans trans;
	//// 自機のワールド座標から3Dレティクルのワールド座標を計算
	//{
	//	// 自機から3Dレティクルへの距離
	//	const float kDistancePlayerTo3DReticle = 40.0f;

	//	// 自機から3Dレティクルへのオフセット(Z+向き)
	//	Vector3 offset = {0, 0, 1.0f};

	//	// 自機のワールド行列の回転を反映
	//	trans.Vec3conversion_W_Notincluded(offset, worldTransform_.matWorld_);

	//	// ベクトルの長さを整える
	//	offset.normalize() *= kDistancePlayerTo3DReticle;

	//	// 3Dレティクルの座標を設定
	//	Vector3 player_pos = GetWorldPosition();

	//	worldTransform3DReticle_.translation_ = offset + player_pos;

	//	// ワールド行列の更新と転送
	//	trans.Affine_Trans(
	//	  worldTransform3DReticle_.matWorld_, worldTransform3DReticle_.scale_,
	//	  worldTransform3DReticle_.rotation_, worldTransform3DReticle_.translation_);

	//	worldTransform3DReticle_.TransferMatrix();
	//}

	//// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	//{
	//	Vector3 positionReticle = {
	//	  worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
	//	  worldTransform3DReticle_.matWorld_.m[3][2]};



	//	// ビューポート行列
	//	Matrix4 matViewport =
	//	{
	//	  (float)WinApp::GetInstance()->kWindowWidth / 2,
	//	  0.0f,
	//	  0.0f,
	//	  0.0f,
	//	  0.0f,
	//	  -(float)WinApp::GetInstance()->kWindowHeight / 2,
	//	  0.0f,
	//	  0.0f,
	//	 0.0f     , 0.0f       , 1.0f, 0.0f,
	//	  (float)WinApp::GetInstance()->kWindowWidth / 2,
	//	  (float)WinApp::GetInstance()->kWindowHeight / 2,
	//	  0.0f,
	//	  1.0f
	//	};

	//	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//	Matrix4 matViewProjectionViewport =
	//	  viewProjection.matView * viewProjection.matProjection * matViewport;

	//	// ワールド→スクリーン座標変換（ここで3Dから2Dになる）
	//	trans.Vec3conversion_W_Included(positionReticle, matViewProjectionViewport);

	//	// スプライトのレティクルに座標設定
	//	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	//}



	// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{ 
		// スプライトの現在座標を取得
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		Vector2 ReticleLimitX = { 50,1230 };
		Vector2 ReticleLimitY = { 40,680 };

		Vector2 ReticleLimitMin;
		Vector2 ReticleLimitMax;

		XINPUT_STATE joyState;

		// ジョイスティック状態取得
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
			spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

			// スプライトの座標変更を反映
			sprite2DReticle_->SetPosition(spritePosition);
		}

		

		//POINT mousePosition;
		// マウス座標（スクリーン座標を取得する）
		//GetCursorPos(&mousePosition);

		// クライアントエリア座標に変換する
		//HWND hwnd = WinApp::GetInstance()->GetHwnd();
		//ScreenToClient(hwnd, &mousePosition);

		// マウス座標を2Dレティクルのスプライトに代入する
		sprite2DReticle_->SetPosition(Vector2(spritePosition.x, spritePosition.y));

		// ビュープロジェクションビューポート合成行列
		// ビューポート行列
		Matrix4 matViewport = 
		{
		  VpWidth / 2,0.0f,0.0f,0.0f, 
		 0.0f,-VpHeight / 2,0.0f,0.0f,         
		 0.0f, 0.0f, 1.0f, 0.0f,
		 VpWidth/ 2,VpHeight / 2,0.0f,1.0f};

		Matrix4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

		// 合成行列の逆行列を計算する
		Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);

		// スクリーン座標
		Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
		Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

		// スクリーン座標系からワールド座標系へ
		trans.Vec3conversion_W_Included(posNear, matInverseVPV);
		trans.Vec3conversion_W_Included(posFar, matInverseVPV);

		// マウスレイの方向
		Vector3 mouseDirection = posFar - posNear;
		mouseDirection.normalize();

		// カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 100.0f;

		worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;

		
		//// 制限を超えない処理
		//worldTransform3DReticle_.translation_.x = min(worldTransform3DReticle_.translation_.x, ReticleLimitX.x);
		//worldTransform3DReticle_.translation_.x = max(worldTransform3DReticle_.translation_.x, ReticleLimitX.y);
		//worldTransform3DReticle_.translation_.y = min(worldTransform3DReticle_.translation_.y, ReticleLimitY.x);
		//worldTransform3DReticle_.translation_.y = max(worldTransform3DReticle_.translation_.y, ReticleLimitY.y);


		// ワールド行列の更新と転送
		trans.Affine_Trans(
		  worldTransform3DReticle_.matWorld_, worldTransform3DReticle_.scale_,
		  worldTransform3DReticle_.rotation_, worldTransform3DReticle_.translation_);

		worldTransform3DReticle_.TransferMatrix();

		// デバッグテキスト
		//debugText_->SetPos(20, 200);
		//debugText_->Printf("Mouse ScreenPos:(%f,%f)", spritePosition.x, spritePosition.y);
		//debugText_->SetPos(20, 220);
		//debugText_->Printf("Near:(%f,%f,%f)", posNear.x, posNear.y, posNear.z);
		//debugText_->SetPos(20, 240);
		//debugText_->Printf("Far:(%f,%f,%f)", posFar.x, posFar.y, posFar.z);*/
	}

	//debugText_->SetPos(20, 260);
	//debugText_->Printf(
	//  "MouseObject:(%f,%f,%f)", worldTransform3DReticle_.translation_.x,
	//  worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);

}

void Player::ReticleDraw(ViewProjection viewProjection) 
{
	//model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI() 
{ 
	sprite2DReticle_.get()->Draw();
}

void Player::SetIsDead(bool flag)
{
	isDead_ = flag;
}
