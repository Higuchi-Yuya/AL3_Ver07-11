#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include <math.h>
#include <random>
#include <sstream>

#include "Enemy.h"
#include "Player.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "ScoreSprite.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	Vector3 Get_Reference_point(Vector3& vertex);

	double Rad(double degree);

	void scale(Vector3& vertex, Vector3& reference_point, float expansion_rate_x, float expansion_rate_y, float expansion_rate_z);

	void translation(Vector3& vertex, float Tx, float Ty, float Tz);

	void rotate(Vector3& vertex, Vector3& reference_point, float frequency_x, float frequency_y, float frequency_z);

	void CheckAllCollisions();

	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	void EnemyBulletFlagDead();

	void EnemyBulletUpdate();

	void EnemyBulletDraw();

	void EnemySpawn(Vector3& position);

	void LoadEnemyPopData();

	void ClearEnemyPopData();

	void UpdateEnemyPopCommands();

	void GameSceneInitialize();

	void Sprite_Initalize();

	void TitleSpriteDraw();

	void GameSceneSpriteDraw();

	void ResultSceneSpriteDraw();

	void SceneChangeUpdate();

	void SceneChangeDraw();

	void SoundInitalize();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	float grit_x = -20;
	float grit_z = -20;
	float Sx, Sy, Sz;
	float Rx[100], Ry[100], Rz[100];
	float Tx[100], Ty[100], Tz[100];

	float Eye_x, Eye_y, Eye_z = -50;
	float Target_x = 0, Target_y, Target_z;
	float viewAngle = 0.0f;

	float character_speed_x = 0.2f;

	float rotate_speed = 0.5f;

	//Vector3* Line = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;


	// スプライト

	// スコア
	int score = 1000;

	// スコア描画用変数
	int div_score = 100000;
	const int max_score_num = 5;
	int score_num[5] = { 0 };
	ScoreSprite scoreSprite_[5];
	Vector2 pos = { 900, 55 };

	// ライフ描画用変数
	const int max_life_num = 1;
	int life_num = 0;
	int life_ = 0;
	ScoreSprite lifeNumSprite;
	Vector2 lifeNumPos = { 140,57 };

	// リザルトスコア描画用変数
	ScoreSprite result_scoreSprite_[5];
	Vector2 resultScorePos = { 500, 330 };
	Vector2 resultScoreSize = { 36,56 };

	//3Dモデル
	Model* model_ = nullptr;

	Model* modelPlayer_ = nullptr;

	Model* modelSkydome_ = nullptr;

	// 呼び出し宣言
	Player* player_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemys_;

	Skydome* skydome_ = nullptr;

	RailCamera* railcamera_ = nullptr;

	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	Vector3 rotate_ = { 0, 0, 0 };
	Vector3 scale_ = { 1, 1, 1 };

	Vector3 grit_line_x_s[9];
	Vector3 grit_line_x_e[9];
	Vector3 grit_line_z_s[9];
	Vector3 grit_line_z_e[9];

	Vector4 grit_color_x = { 0, 0, 1, 1 };
	Vector4 grit_color_z = { 1, 0, 0, 1 };

	// 敵の位置リスト
	Vector3 enemypos[2] = {
	  {20,  3, 10},
	  {-20, 3, 10}
	};

	//ワールドトランスフォーム
	WorldTransform cameraWorldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// 敵の弾リスト
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 敵発生待機用変数
	bool waitFlag = true;
	int waitTimer = 120;


	// 敵死亡カウント用変数
	int enemy_deadcount = 0;


	// シーン分けのフラグ
	bool startscene_flag = true;
	bool gamescene_flag = false;
	bool resultscene_flag = false;

	// スプライト用の変数
	std::unique_ptr<Sprite> title_name_;
	std::unique_ptr<Sprite> title_button_;
	std::unique_ptr<Sprite> flame_;
	std::unique_ptr<Sprite> LifeF_;
	std::unique_ptr<Sprite> ScoreF_;
	std::unique_ptr<Sprite> result_flame_;
	std::unique_ptr<Sprite> result_font_;
	std::unique_ptr<Sprite> result_score_font_;
	std::unique_ptr<Sprite> result_button_;

	// スプライトのテクスチャ
	uint32_t titleNameTextureHandle_ = 0;
	uint32_t titleButtonTextureHandle_ = 0;
	uint32_t flameTextureHandle_ = 0;
	uint32_t scoreF_TextureHandle_ = 0; // ○○Fはフォントで、文字
	uint32_t LifeF_TextureHandle_ = 0;
	uint32_t result_flame_TextureHandle_ = 0;
	uint32_t result_font_TextureHandle_ = 0;
	uint32_t result_score_font_TextureHandle_ = 0;
	uint32_t result_button_TextureHandle_ = 0;

	Vector2 pos__ = { 0,0 };
	Vector2 anker = { 0,0 };
	Vector4 color = { 1,1,1,1 };

	// ゲームシーンチェンジ用変数等
	std::unique_ptr<Sprite> scene_change_up_;
	std::unique_ptr<Sprite> scene_change_down_;

	uint32_t scene_change_TextureHandle_ = 0;

	Vector2 sceneChangeUpPos = { 0,-360 };
	Vector2 sceneChangeDownPos = { 0,720 };

	Vector2 scUpDestinationPos = { 0,-360 };
	Vector2 scDownDestinationPos = { 360,720 };

	
	bool scCloseFlag = false;
	bool scOpenFlag = false;

	int sceneChangeSpeed = 4;

	int sceneChangeTimer = 120;

	// 音声ハンドル
	uint32_t titleBgmSDHandle_ = 0;
	uint32_t gameSceneBgmSDHandle_ = 0;
	uint32_t resultSceneBgmSDHandle_ = 0;
	

	// 再生保存用ハンドル
	uint32_t titleBgmVoiceHandle_ = 0;
	uint32_t gameSceneBgmVoiceHandle_ = 0;
	uint32_t resultSceneBgmVoiceHandle_ = 0;
	

	// 音声フラグ
	bool titleBgmFlag = true;
	bool gameSceneBgmFlag = false;
	bool resultSceneBgmFlag = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
