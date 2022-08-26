#include "RailCamera.h"
#include "Affine_trans.h"


void RailCamera::Initialize(WorldTransform worldTransform) {
	// ワールドトランスフォームのデータを代入
	/*worldTransform_.Initialize();*/
	/*worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;*/
	worldTransform_ = worldTransform;
	debugText_ = DebugText::GetInstance();


	viewProjection_.farZ = 1000.0f;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
#pragma region ワールド行列の処理
	Affine_trans* trans = nullptr;
	Vector3 move = {0, 0, 0};
	Vector3 rotate = {0, 0, 0};
	
	if (cameraMoveFlag_ == true)
	{
		// カメラオブジェの移動処理
		move = { 0.0f, 0.0f, +moveSpeed_.z };
		worldTransform_.translation_ += move;
	}
	

	// カメラオブジェの回転処理
	rotate = {0.0f, +rotateSpeed_.y, 0.0f};
	//worldTransform_.rotation_ += rotate;

	// ワールド行列の再計算
	trans->identity_matrix(worldTransform_.matWorld_);
	trans->Affine_Trans(worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,worldTransform_.translation_);

	
#pragma endregion

#pragma region ビュープロジェクションの処理
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	// ワールド前方ベクトル
	Vector3 forward(0, 0, 1);

	// レールカメラの回転を反映
	trans->Vec3conversion_W_Notincluded(forward, worldTransform_.matWorld_);
	
	// 視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	
	// ワールド上方ベクトル
	Vector3 up(0, 1, 0);

	// レールカメラの回転を反映（レールカメラの上方ベクトル）
	trans->Vec3conversion_W_Notincluded(up,worldTransform_.matWorld_);
	viewProjection_.up = up;

	// ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

#pragma endregion

#pragma region デバッグテキスト
	////デバッグテキスト表示
	//debugText_->SetPos(50, 120);
	//debugText_->Printf(
	//  "Eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//debugText_->SetPos(50, 140);
	//debugText_->Printf(
	//  "Target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,viewProjection_.target.z);
	//debugText_->SetPos(50, 160);
	//debugText_->Printf("Forward:(%f,%f,%f)", forward.x, forward.y, forward.z);
	//debugText_->SetPos(50, 180);
	//debugText_->Printf("Up:(%f,%f,%f)", up.x, up.y, up.z);

	//debugText_->SetPos(50, 320);
	//debugText_->Printf(
	//  "postion:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	//  worldTransform_.translation_.z);
#pragma endregion
}

ViewProjection RailCamera::GetViewProjection() { return viewProjection_; }

WorldTransform* RailCamera::GetWorldTransform() { return &worldTransform_; }

void RailCamera::SetcameraMoveFlag(bool flag)
{
	cameraMoveFlag_ = flag;
}


