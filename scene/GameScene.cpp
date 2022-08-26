#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

#include <algorithm>
#include <iostream>

const float Pi = 3.141593f;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
	delete player_;
	delete skydome_;
	delete modelSkydome_;
	delete modelPlayer_;
	delete railcamera_;
}

Vector3 GameScene::Get_Reference_point(Vector3& vertex) { return vertex; }

double GameScene::Rad(double degree) { return degree * Pi / 180; }

void GameScene::scale(Vector3& vertex, Vector3& reference_point, float expansion_rate_x, float expansion_rate_y, float expansion_rate_z) {
	float w = 0;
	//拡大縮小
	vertex -= reference_point;
	vertex.x = (vertex.x * expansion_rate_x) + (vertex.y * 0) + (0 * vertex.z) + w;
	vertex.y = (vertex.x * 0) + (vertex.y * expansion_rate_y) + (0 * vertex.z) + w;
	vertex.z = (vertex.x * 0) + (vertex.y * 0) + (vertex.z * expansion_rate_z) + w;
	vertex += reference_point;
}

void GameScene::translation(Vector3& vertex, float Tx, float Ty, float Tz) {
	float w = 1;
	//移動
	vertex.x = (vertex.x * 1) + (vertex.y * 0) + (vertex.z * 0) + (Tx * w);
	vertex.y = (vertex.x * 0) + (vertex.y * 1) + (vertex.z * 0) + (Ty * w);
	vertex.z = (vertex.x * 0) + (vertex.y * 0) + (vertex.z * 1) + (Tz * w);
}

void GameScene::rotate(Vector3& vertex, Vector3& reference_point, float frequency_x, float frequency_y, float frequency_z) {
	float w = 0;
	float h_x, h_y, h_z;

	// xの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * 1) + (h_y * 0) + (h_z * 0) + (0 * w);
	vertex.y = (h_x * 0) + (h_y * cos(Rad(frequency_x))) + (h_z * sin(Rad(frequency_x))) + (0 * w);
	vertex.z = (h_x * 0) + (h_y * -sin(Rad(frequency_x))) + (h_z * cos(Rad(frequency_x))) + (0 * w);
	vertex += reference_point;

	// yの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * cos(Rad(frequency_y))) + (h_y * 0) + (h_z * -sin(Rad(frequency_y))) + (0 * w);
	vertex.y = (h_x * 0) + (h_y * 1) + (h_z * 0) + (0 * w);
	vertex.z = (h_x * sin(Rad(frequency_y))) + (h_y * 0) + (h_z * cos(Rad(frequency_y))) + (0 * w);
	vertex += reference_point;

	// zの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * cos(Rad(frequency_z))) + (h_y * sin(Rad(frequency_z))) + (h_z * 0) + (0 * w);
	vertex.y = (h_x * -sin(Rad(frequency_z))) + (h_y * cos(Rad(frequency_z))) + (h_z * 0) + (0 * w);
	vertex.z = (h_x * 0) + (h_y * 0) + (h_z * 1) + (0 * w);
	vertex += reference_point;
}

void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	float radiusA = 1.5f, radiusB = 1.5f;

	// 敵リストの取得
	const std::list<std::unique_ptr<Enemy>>& enemys = enemys_;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemyBullets_;

#pragma region 自キャラと敵弾の当たり判定

	radiusA = 2.0f, radiusB = 1.6f;
	// 自キャラのワールド座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		// 敵弾のワールド座標
		posB = bullet->GetBulletWorldPosition();

		// 座標Aと座標Bの距離を求める
		float result_pos_x = (posB.x - posA.x) * (posB.x - posA.x);
		float result_pos_y = (posB.y - posA.y) * (posB.y - posA.y);
		float result_pos_z = (posB.z - posA.z) * (posB.z - posA.z);
		float result_pos = result_pos_x + result_pos_y + result_pos_z;
		// 半径
		float result_radius = (radiusA + radiusB) * (radiusA + radiusB);

		if (result_pos <= result_radius)
		{
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	radiusA = 1.6f, radiusB = 2.0f;

	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		if (!enemy->IsDead())
		{
			// 敵キャラのワールド座標
			posA = enemy->GetWorldPosition();

			// 自弾と敵キャラ全ての当たり判定
			for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
				// 自弾のワールド座標
				posB = bullet->GetBulletWorldPosition();

				// 座標Aと座標Bの距離を求める
				float result_pos_x = (posB.x - posA.x) * (posB.x - posA.x);
				float result_pos_y = (posB.y - posA.y) * (posB.y - posA.y);
				float result_pos_z = (posB.z - posA.z) * (posB.z - posA.z);
				float result_pos = result_pos_x + result_pos_y + result_pos_z;
				// 半径
				float result_radius = (radiusA + radiusB) * (radiusA + radiusB);



				if (result_pos <= result_radius) {
					// 敵キャラの衝突時コールバックを呼び出す
					enemy->OnCollision();
					// 自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();

					// 敵が死んだ時にスコアにプラスする
					enemy_deadcount++;
				}

			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	radiusA = 1.6f, radiusB = 1.6f;

	// 自弾と敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerbullet : playerBullets) {
		// 自弾のワールド座標
		posA = playerbullet->GetBulletWorldPosition();
		for (const std::unique_ptr<EnemyBullet>& enemybullet : enemyBullets) {
			// 敵弾のワールド座標
			posB = enemybullet->GetBulletWorldPosition();

			// 座標Aと座標Bの距離を求める
			float result_pos_x = (posB.x - posA.x) * (posB.x - posA.x);
			float result_pos_y = (posB.y - posA.y) * (posB.y - posA.y);
			float result_pos_z = (posB.z - posA.z) * (posB.z - posA.z);
			float result_pos = result_pos_x + result_pos_y + result_pos_z;
			// 半径
			float result_radius = (radiusA + radiusB) * (radiusA + radiusB);

			if (result_pos <= result_radius) {
				// 自弾の衝突時コールバックを呼び出す
				playerbullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemybullet->OnCollision();
			}
		}
	}
#pragma endregion

}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet)
{
	// 弾リストに登録
	enemyBullets_.push_back(std::move(enemyBullet));
}

void GameScene::EnemyBulletFlagDead()
{
	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
}

void GameScene::EnemyBulletUpdate()
{
	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}
}

void GameScene::EnemyBulletDraw()
{
	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(railcamera_->GetViewProjection());
	}
}

void GameScene::EnemySpawn(Vector3& position)
{
	// 敵を生成し、初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(modelPlayer_, position);

	// 敵をリストに登録
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::ClearEnemyPopData()
{
	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
}

void GameScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (waitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			waitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行るループ
	while (getline(enemyPopCommands, line)) {
		//１行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			Vector3 pos = { x, y, z };
			EnemySpawn(pos);

			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->SetPlayer(player_);
				enemy->SetGameScene(this);
				
			}
		}

		else if (word.find("PATTERN") == 0)
		{
			//敵の行動パターン
			getline(line_stream, word, ',');
			int pattern = (int)std::atof(word.c_str());

			switch (pattern)
			{
			case 0:
				enemys_.back()->SetEnemyPattern(Pattern::Straight);
				
				break;
			case 1:
				enemys_.back()->SetEnemyPattern(Pattern::Provocation);
				
				break;
			}
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::GameSceneInitialize()
{
	// カメラオブジェクトの初期化
	cameraWorldTransform_.Initialize();
	cameraWorldTransform_.translation_.z = -360.0f;

	// レールカメラの初期化
	railcamera_->Initialize(cameraWorldTransform_);

	//自キャラの初期化
	player_->Initialize(modelPlayer_);
	player_->SetWorldTransformParent(railcamera_->GetWorldTransform());
	//player_->SetIsDead(false);

	//敵の初期化
	enemys_.clear();
	enemyBullets_.clear();
	ClearEnemyPopData();
	LoadEnemyPopData();
}

void GameScene::Initialize() {

	

	// レティクルのテクスチャ
	TextureManager::Load("2DReticle_500.png");

	debugCamera_ = new DebugCamera(1200, 720);

	//３Dモデルの生成
	model_ = Model::Create();

	modelPlayer_ = Model::CreateFromOBJ("star", true);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//自キャラの生成
	player_ = new Player();

	//天球の生成
	skydome_ = new Skydome();

	// レールカメラの生成
	railcamera_ = new RailCamera();

	GameSceneInitialize();

	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	// スコアの描画の初期化
	for (int i = 0; i < max_score_num; i++)
	{
		scoreSprite_[i].Initialize();
	}

	////カメラ視点座標を設定
	// viewProjection_.eye = {Eye_x, Eye_y, Eye_z};

	////カメラ注視点座標を設定
	// viewProjection_.target = {Target_x, Target_y, Target_z};
	//
	////カメラ上方向ベクトルを設定 (右上45度指定)
	// viewProjection_.up = {cosf(Pi / 4.0f), sinf(Pi / 4.0f), 0.0f};

	////カメラ垂直方向視野角を設定
	// viewProjection_.fovAngleY = 50.0f * (Pi / 180);

	////アスペクト比を設定
	// viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;

	////ファークリップ距離を設定
	//viewProjection_.farZ = 12.0f;

	//ビュープロジェクションの初期化
	//viewProjection_.Initialize();


	//軸方向表示の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//乱数範囲の指定
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();



#pragma region グリット線の描画の設定
	//グリット線の描画位置設定

	for (int i = 0; i < 9; i++) {
		translation(grit_line_x_s[i], grit_x, 0, -20);
		grit_x += 5;
	}
	grit_x = -20;
	for (int i = 0; i < 9; i++) {
		translation(grit_line_x_e[i], grit_x, 0, 20);
		grit_x += 5;
	}

	for (int i = 0; i < 9; i++) {
		translation(grit_line_z_s[i], -20, 0, grit_z);
		grit_z += 5;
	}
	grit_z = -20;
	for (int i = 0; i < 9; i++) {
		translation(grit_line_z_e[i], 20, 0, grit_z);
		grit_z += 5;
	}
#pragma endregion

	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

#pragma region 天球の処理
	skydome_->Update();
#pragma endregion

#pragma region レールカメラの処理
	railcamera_->Update();
#pragma endregion
	debugText_->SetPos(100, 150);
	debugText_->Printf("Flag:%d", gamescene_flag);


	// ---------------スタートシーン-------------//
	if (startscene_flag == true)
	{
		railcamera_->SetcameraMoveFlag(false);



		// ゲームパッドの状態を得る変数（XINPUT）
		XINPUT_STATE joyState;

		// ゲームパッド未接続なら何もせず抜ける
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}
		
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			startscene_flag = false;
			GameSceneInitialize();
			gamescene_flag = true;
		}
		
	}


	// ----------------ゲームシーン--------------//
	if (gamescene_flag == true)
	{
		railcamera_->SetcameraMoveFlag(true);

		

	#pragma region キャラクター移動処理
		player_->Update();

		player_->ReticleUpdate(railcamera_->GetViewProjection());

		UpdateEnemyPopCommands();
		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemy->Update();
		}
	#pragma endregion

	#pragma region 敵の弾の更新処理
		EnemyBulletFlagDead();
		EnemyBulletUpdate();
	#pragma endregion

	#ifdef _DEBUG
		if (input_->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = true;
		}
	#endif

		////カメラの処理
		//if (isDebugCameraActive_) {
		//	debugCamera_->Update();
		//	viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		//	viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//	viewProjection_.TransferMatrix();
		//}
		//else {
		//	viewProjection_.UpdateMatrix();
		//	viewProjection_.TransferMatrix();
		//}

		//当たり判定
		CheckAllCollisions();

		//スコアの更新
		score = 500 * enemy_deadcount;

		debugText_->SetPos(100, 120);
		debugText_->Printf("Score:%d", score); 
	#pragma region レールカメラの処理
			railcamera_->Update();
	#pragma endregion

	#pragma region キャラクター移動処理
		player_->Update();

		player_->ReticleUpdate(railcamera_->GetViewProjection());

		UpdateEnemyPopCommands();
		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemy->Update();
		}
	#pragma endregion

	#pragma region 天球の処理
		skydome_->Update();
	#pragma endregion

	#pragma region 敵の弾の更新処理
		EnemyBulletFlagDead();
		EnemyBulletUpdate();
	#pragma endregion


	#ifdef _DEBUG
		if (input_->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = true;
		}
	#endif

		////カメラの処理
		//if (isDebugCameraActive_) {
		//	debugCamera_->Update();
		//	viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		//	viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//	viewProjection_.TransferMatrix();
		//}
		//else {
		//	viewProjection_.UpdateMatrix();
		//	viewProjection_.TransferMatrix();
		//}

		//当たり判定
		CheckAllCollisions();

		//スコアの更新
		score = 500 * enemy_deadcount;

		debugText_->SetPos(100, 170);
		debugText_->Printf("pos:%f", player_->GetWorldPosition().z);

		if (player_->IsDead() == true || player_->GetWorldPosition().z >= 50)
		{
			gamescene_flag = false;
		
			resultscene_flag = true;
		}
	
	}

	//------------リザルトシーン---------------//
	if (resultscene_flag == true)
	{
		railcamera_->SetcameraMoveFlag(false);

		debugText_->SetPos(100, 120);

		debugText_->Printf("Score:%d", score);

		// ゲームパッドの状態を得る変数（XINPUT）
		XINPUT_STATE joyState;

		// ゲームパッド未接続なら何もせず抜ける
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			resultscene_flag = false;
			// カメラオブジェクトの初期化
			cameraWorldTransform_.Initialize();
			cameraWorldTransform_.translation_.z = -360.0f;
			// レールカメラの初期化
			railcamera_->Initialize(cameraWorldTransform_);
			startscene_flag = true;
		}
	}
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

	// 天球の描画
	skydome_->Draw(railcamera_->GetViewProjection());

	if (gamescene_flag == true)
	{
		//自キャラの描画
		player_->Draw(railcamera_->GetViewProjection());

		//敵キャラの描画
		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemy->Draw(railcamera_->GetViewProjection());
		}
		EnemyBulletDraw();

		
	}
	



	//ライン描画が参照するビュープロジェクションを指定する

	//グリット線の描画
	/*for (int i = 0; i < 9; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  grit_line_x_s[i], grit_line_x_e[i], grit_color_x);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  grit_line_z_s[i], grit_line_z_e[i], grit_color_z);
	}*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	if (gamescene_flag == true)
	{
		player_->DrawUI();

		
	}
	div_score = 100000;
	for (int i = 0; i < max_score_num; i++)
	{
		score_num[i] = score % div_score / (div_score / 10);
		div_score = div_score / 10;
		pos.x = 800 - ((1 - i) * 32);
		scoreSprite_[i].Draw(score_num[i],pos);
		
	}

	/*for (int i = 0; i < max_score_num; i++)
	{
		pos_[i].x = 800 - (i * texsize.x);
		scoresprite_[i].get()->SetPosition(pos_[i]);
		scoresprite_[i].get()->Draw();
	}*/

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
