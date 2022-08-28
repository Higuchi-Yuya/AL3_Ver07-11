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
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("star/star_player.png");
	
	audio_ = Audio::GetInstance();

	// �����f�[�^�ǂݍ���
	shotSeSDHandle_ = audio_->LoadWave("SE/Shot.wav");

	// ���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle_ = TextureManager::Load("2DReticle.png");

	// �X�v���C�g����
	Vector2 pos = {640, 360};
	Vector4 color = {1, 1, 1, 1};
	Vector2 anker = {0.5f, 0.5f};
	Vector2 size = { 150,150 };
	sprite2DReticle_.reset(Sprite::Create(textureReticle_, pos, color, anker));
	sprite2DReticle_.get()->SetSize(size);

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���C�t�̃��Z�b�g
	life = 5;

	// �f�X�t���O�̃��Z�b�g
	isDead_ = false;

	//���[���h�ϊ���������
	worldTransform_.translation_.x = 0;
	worldTransform_.translation_.y = 0;
	worldTransform_.translation_.z = 50.0f;

	worldTransform_.Initialize();
	
	// 3D���e�B�N���̃��[���h�g�����X�t�H�[��������
	worldTransform3DReticle_.Initialize();

	// �v���C���[�̒e�����Z�b�g
	bullets_.clear();
	
}

void Player::Update() {
	if (isDead_ == false)
	{
		//�f�X�t���O�̗������e���폜
		bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

		//��]����
		Rotation();

		//�ړ�����
		Move();

		//�L�����N�^�[�̍U������

		Attack();

		//�e�X�V
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
			bullet->Update();
		}

		//�s��̍X�V����ѓ]��
		Trans_Update();
	}
	//�f�o�b�O�e�L�X�g
	Debug_Text();
}

void Player::Attack() {
	XINPUT_STATE joyState;

	// �Q�[���p�b�h���ڑ��Ȃ牽������������
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

			//�e�̑��x
			const float kBulletSpeed = 0.3f;
			Vector3 velocity;
			Affine_trans trans;

			Vector3 player_pos = GetWorldPosition();
			Vector3 reticle_pos = {
			  worldTransform3DReticle_.matWorld_.m[3][0],
			  worldTransform3DReticle_.matWorld_.m[3][1],
			  worldTransform3DReticle_.matWorld_.m[3][2]};

			// ���@����Ə��I�u�W�F�N�g�ւ̃x�N�g��
			velocity = reticle_pos - player_pos;
			velocity.normalize();
			velocity *= kBulletSpeed;

			//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
			// velocity = velocity * worldTransform_.matWorld_;

			//�e�𐶐����A������
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(model_, worldTransform_, velocity);

			//�e��o�^����
			bullets_.push_back(std::move(newBullet));

			timer = 40;
		}
		
	}
	
}

void Player::Draw(ViewProjection viewProjection) {
	if (isDead_ == false)
	{
		model_->Draw(worldTransform_, viewProjection, textureHandle_);

		// 3D���e�B�N����`��
		ReticleDraw(viewProjection);

		//�e�`��
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
			bullet->Draw(viewProjection);
		}
	}
}

Vector3 Player::GetWorldPosition() 
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
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

	// �Q�[���p�b�h�̏�Ԃ𓾂�ϐ��iXINPUT�j
	XINPUT_STATE joyState;

	// �W���C�X�e�B�b�N��Ԏ擾
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * character_speed_x;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * character_speed_y;

	}
	worldTransform_.translation_ += move;


	//�L�����N�^�[�̈ړ�����
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

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -MoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +MoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -MoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +MoveLimitY);
}

void Player::Rotation() 
{ 
	Vector3 rotate = {0, 0, 0};

	//�L�����N�^�[�̐���ړ�
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
	
	//�x�N�g���̉��Z
	trans->identity_matrix(worldTransform_.matWorld_);

	

	trans->Affine_Trans(
	  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
	  worldTransform_.translation_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	//worldTransform_.matWorld_ *= railCamera->GetWorldTransform().matWorld_;
	//worldTransform_.matWorld_.m[3][2] += 50.0f;
	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Player::Debug_Text() 
{
	//�f�o�b�O�e�L�X�g�\��
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
	//// ���@�̃��[���h���W����3D���e�B�N���̃��[���h���W���v�Z
	//{
	//	// ���@����3D���e�B�N���ւ̋���
	//	const float kDistancePlayerTo3DReticle = 40.0f;

	//	// ���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
	//	Vector3 offset = {0, 0, 1.0f};

	//	// ���@�̃��[���h�s��̉�]�𔽉f
	//	trans.Vec3conversion_W_Notincluded(offset, worldTransform_.matWorld_);

	//	// �x�N�g���̒����𐮂���
	//	offset.normalize() *= kDistancePlayerTo3DReticle;

	//	// 3D���e�B�N���̍��W��ݒ�
	//	Vector3 player_pos = GetWorldPosition();

	//	worldTransform3DReticle_.translation_ = offset + player_pos;

	//	// ���[���h�s��̍X�V�Ɠ]��
	//	trans.Affine_Trans(
	//	  worldTransform3DReticle_.matWorld_, worldTransform3DReticle_.scale_,
	//	  worldTransform3DReticle_.rotation_, worldTransform3DReticle_.translation_);

	//	worldTransform3DReticle_.TransferMatrix();
	//}

	//// 3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z
	//{
	//	Vector3 positionReticle = {
	//	  worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
	//	  worldTransform3DReticle_.matWorld_.m[3][2]};



	//	// �r���[�|�[�g�s��
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

	//	// �r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
	//	Matrix4 matViewProjectionViewport =
	//	  viewProjection.matView * viewProjection.matProjection * matViewport;

	//	// ���[���h���X�N���[�����W�ϊ��i������3D����2D�ɂȂ�j
	//	trans.Vec3conversion_W_Included(positionReticle, matViewProjectionViewport);

	//	// �X�v���C�g�̃��e�B�N���ɍ��W�ݒ�
	//	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	//}



	// �}�E�X�J�[�\���̃X�N���[�����W���烏�[���h���W���擾����3D���e�B�N���z�u
	{ 
		// �X�v���C�g�̌��ݍ��W���擾
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		Vector2 ReticleLimitX = { 50,1230 };
		Vector2 ReticleLimitY = { 40,680 };

		Vector2 ReticleLimitMin;
		Vector2 ReticleLimitMax;

		XINPUT_STATE joyState;

		// �W���C�X�e�B�b�N��Ԏ擾
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
			spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

			// �X�v���C�g�̍��W�ύX�𔽉f
			sprite2DReticle_->SetPosition(spritePosition);
		}

		

		//POINT mousePosition;
		// �}�E�X���W�i�X�N���[�����W���擾����j
		//GetCursorPos(&mousePosition);

		// �N���C�A���g�G���A���W�ɕϊ�����
		//HWND hwnd = WinApp::GetInstance()->GetHwnd();
		//ScreenToClient(hwnd, &mousePosition);

		// �}�E�X���W��2D���e�B�N���̃X�v���C�g�ɑ������
		sprite2DReticle_->SetPosition(Vector2(spritePosition.x, spritePosition.y));

		// �r���[�v���W�F�N�V�����r���[�|�[�g�����s��
		// �r���[�|�[�g�s��
		Matrix4 matViewport = 
		{
		  VpWidth / 2,0.0f,0.0f,0.0f, 
		 0.0f,-VpHeight / 2,0.0f,0.0f,         
		 0.0f, 0.0f, 1.0f, 0.0f,
		 VpWidth/ 2,VpHeight / 2,0.0f,1.0f};

		Matrix4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

		// �����s��̋t�s����v�Z����
		Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);

		// �X�N���[�����W
		Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
		Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

		// �X�N���[�����W�n���烏�[���h���W�n��
		trans.Vec3conversion_W_Included(posNear, matInverseVPV);
		trans.Vec3conversion_W_Included(posFar, matInverseVPV);

		// �}�E�X���C�̕���
		Vector3 mouseDirection = posFar - posNear;
		mouseDirection.normalize();

		// �J��������Ə��I�u�W�F�N�g�̋���
		const float kDistanceTestObject = 100.0f;

		worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;

		
		//// �����𒴂��Ȃ�����
		//worldTransform3DReticle_.translation_.x = min(worldTransform3DReticle_.translation_.x, ReticleLimitX.x);
		//worldTransform3DReticle_.translation_.x = max(worldTransform3DReticle_.translation_.x, ReticleLimitX.y);
		//worldTransform3DReticle_.translation_.y = min(worldTransform3DReticle_.translation_.y, ReticleLimitY.x);
		//worldTransform3DReticle_.translation_.y = max(worldTransform3DReticle_.translation_.y, ReticleLimitY.y);


		// ���[���h�s��̍X�V�Ɠ]��
		trans.Affine_Trans(
		  worldTransform3DReticle_.matWorld_, worldTransform3DReticle_.scale_,
		  worldTransform3DReticle_.rotation_, worldTransform3DReticle_.translation_);

		worldTransform3DReticle_.TransferMatrix();

		// �f�o�b�O�e�L�X�g
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
