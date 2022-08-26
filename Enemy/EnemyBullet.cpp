#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const WorldTransform worldTransform, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("red.png");


	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = {
	  worldTransform.matWorld_.m[3][0], 
	  worldTransform.matWorld_.m[3][1],
	  worldTransform.matWorld_.m[3][2]};


	worldTransform_.scale_.x = 0.6;
	worldTransform_.scale_.y = 0.6;
	worldTransform_.scale_.z = 0.6;

	// �ł����Ɍ����ăI�u�W�F�N�g����]������
	
	// Y������p�x(��y)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// Y�������-��y�񂷉�]�s����v�Z
	Matrix4 RotY;
	trans->rotateY(RotY, -worldTransform_.rotation_.y);
	// velosity_�ɉ�]�s����|���Z����velosityZ�����߂�
	Vector3 velosityZ=velocity_;
	trans->Vec3conversion_W_Notincluded(velosityZ, RotY);

	// X������p�x(��x)
	worldTransform_.rotation_.x = std::atan2(-velosityZ.y, velosityZ.z);

}

void EnemyBullet::Update() 
{

	// ���W���ړ�������(�P�t���[�����̈ړ��ʂ𑫂�����)
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
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::OnCollision() { isDead_ = true; }
