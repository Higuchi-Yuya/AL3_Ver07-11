#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
#include "EnemyBullet.h"

enum class Phase {
	Approach, //�ڋ߂���
	Laeve,    //���E����
	Provocation, //���U��Ȃ���ڋ�
};

enum class Pattern { 
	Straight , //�����܂������ɐڋ߂���
	Provocation, //���U�铮���Ő���Ȃ���߂Â�
};

class Player;

class GameScene;

class Enemy
{
  public:
	//�G�Ŏg���֗��֐�
	float Rad(float range);
	Vector3 Leap(const Vector3& start, const Vector3& end, float t);

	//�G�̊�{�I�Ȋ֐�
	void Initialize(Model* model, Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	

	//�ڋ߃t�F�[�Y�֘A�̊֐�
	void Approach_Initialize();
	void Approach_Update();

	//���E�t�F�[�Y�֘A�̊֐�
	void Laeve_Update();

	//��U��ڋ߃t�F�[�Y�֘A�̊֐�
	void Provocation_Update();

	// �G�L�����Ɏ��L�����̃A�h���X��n��
	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// �G�L�����̍s���p�^�[�����Z�b�g
	void SetEnemyPattern(Pattern pattern) { pattern_ = pattern; }



	//�e�̔��ˊ֐�
	void Fire();
	void WayFire();
	void HomingFire();

	// ���[���h���W���擾����
	Vector3 GetWorldPosition();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// �e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	bool IsDead() const { return isDead_; }

  public:
	//���ˊԊu
	static const int kFireInterval = 80;
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	float enemy_speed_x = 0.1f;
	float enemy_speed_y = 0.1f;
	float enemy_speed_z = 0.05f;
	
	// ��]���x
	Vector3 enemy_rotation_speed = { 0.005,0.005,0.002 };

	//���˃^�C�}�[
	int32_t fireTimer_ = 180;

	//�A�t�B���p
	Affine_trans* trans = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	//��]�t���O
	bool rotate_flag = true;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	// �s���p�^�[��
	Pattern pattern_;

	// �ړ������p�ϐ�
	float enemy_initpos_z = 0;
	float enemy_limit_z = 80;

	// ���L����
	Player* player_ = nullptr;

	// �Q�[���V�[��
	GameScene* gameScene_ = nullptr;
};
