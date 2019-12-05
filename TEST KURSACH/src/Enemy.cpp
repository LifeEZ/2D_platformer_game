#include "Enemy.h"

Enemy::~Enemy() {
}

Enemy::Enemy(Level& Level, const float& X, const float& Y)
	: Entity(X, Y) {
	m_W = 16;
	m_H = 64;
	m_Animation = LoadAnimations();
	m_Objects = Level.GetObjects("solid");
	SetSpeed(0.1f);
	SetDeltaX(0.1f);
}

Enemy::Enemy() {
}


void Enemy::ChoosePath(Entity& player) {
	if (player.GetPosY() != GetPosY()) {
		if (m_Aggro)
			m_Aggro = false;
		for (size_t i = 0; i < m_Objects.size(); i++) {
			if (!((m_Y + m_H) == m_Objects.at(i).rect.top)) {
				continue;
			}
			if (!((m_X <= m_Objects.at(i).rect.left + m_Objects.at(i).rect.width) && (m_X >= m_Objects.at(i).rect.left))) {
				continue;
			}
			m_PathBorders.LeftPoint = m_Objects.at(i).rect.left + 32;
			m_PathBorders.RightPoint = m_Objects.at(i).rect.left + m_Objects.at(i).rect.width - 32;
			m_ObjectForPath = m_Objects.at(i).rect;
			m_ChosenObjectIndex = i;
		}
	}
	for (size_t i = 0; i < m_Objects.size(); i++) {
		if (!((player.GetPosY() + player.GetHeight()) == m_Objects.at(i).rect.top)) {
			continue;
		}
		if (!((player.GetPosX() <= m_Objects.at(i).rect.left + m_Objects.at(i).rect.width) && (player.GetPosX() >= m_Objects.at(i).rect.left))) {
			continue;
		}
		float PlayerObjectLeft = m_Objects.at(i).rect.left;
		float PlayerObjectRight = m_Objects.at(i).rect.left + m_Objects.at(i).rect.width;
		if ((PlayerObjectLeft == m_PathBorders.LeftPoint - 32) && (PlayerObjectRight == m_PathBorders.RightPoint + 32)) {
			m_Aggro = true;
		}
	}
}

void Enemy::UpdateState() {
	if (GetDeltaY() < 0)
		m_OnGround = false;
	if (!GetDeltaX()) {
		SetState(idle);
	}
	SetState(walk);
}

void Enemy::Move(const float& time, Entity& player) {
	if (!m_Aggro) {
		if (m_ObjectForPath.width > 0) {
			if (GetPosX() + GetWidth() >= m_PathBorders.RightPoint) {
				m_Direction = left;
				SetDeltaX(-GetSpeed());
				return;
			}
			if (GetPosX() <= m_PathBorders.LeftPoint) {
				m_Direction = right;
				SetDeltaX(GetSpeed());
				return;
			}
		}
	}
	if (m_Aggro) {
		if (m_HitTrigger.intersects(player.GetRect())) {
			m_Prev_dx = GetDeltaX();
			m_dx = 0;
			SetState(attack);
			return;
		}
		if (player.GetPosX() > GetPosX()) {
			SetDeltaX(GetSpeed());
			m_Direction = right;
		}
		else {
			SetDeltaX(-GetSpeed());
			m_Direction = left;
		}
	}
}

void Enemy::CheckCollisionWithMap(const float& dx, const float& dy) {
	for (unsigned int i = 0; i < m_Objects.size(); i++) {
		if (GetRect().intersects(m_Objects.at(i).rect)) {
			if (dy > 0) {
				m_Y = m_Objects.at(i).rect.top - m_H;
				m_OnGround = true;
				m_dy = 0;
			}
			if (dy < 0) {
				m_Y = m_Objects.at(i).rect.top + m_Objects.at(i).rect.height;
				m_dy = 0;
			}
			if (dx > 0) {
				m_X = m_Objects.at(i).rect.left - m_W;
				m_dx = 0;
				continue;
			}
			if (dx < 0) {
				m_X = m_Objects.at(i).rect.left + m_Objects.at(i).rect.width;
				m_dx = 0;
				continue;
			}
		}
	}
}

void Enemy::Update(const float& time, std::vector<Entity*>& enteties, Entity& player) {
	int a = rand() % 100;
	if (a % 97 == 0) {
		std::cout << "Left " << m_PathBorders.LeftPoint << " Right " << m_PathBorders.RightPoint << "\n";
		std::cout << "X " << m_X << " Y " << m_Y << "\n";
	}
	ChoosePath(player);
	if (m_Control) {
		UpdateState();
		Move(time, player);
		if (GetDeltaX() > 0 && m_Direction == left) {
			m_Direction = right;
		}
		if (GetDeltaX() < 0 && m_Direction == right) {
			m_Direction = left;
		}
	}
	switch (m_State) {
	case walk:
		if (m_Animation.GetName() != "walk")
			m_Animation.Set("walk");
		break;

	case idle:
		if (m_Animation.GetName() != "idle")
			m_Animation.Set("idle");
		break;

	case attack:
		if (m_Animation.GetName() != "attack")
			m_Animation.Set("attack");
		break;
	}
	m_Animation.Update(time, (m_Direction == left ? Animation::AnimDirection::left : Animation::AnimDirection::right));

	m_X += m_dx * time;
	CheckCollisionWithMap(m_dx, 0);
	m_dy += 0.0010f * time;
	m_Y += m_dy * time;
	CheckCollisionWithMap(0, m_dy);
	UpdateHitTrigger();

	if (m_Health <= 0)
		m_Alive = false;

	if (m_Hit) {
		if (!m_HitTimer)
			m_Control = false;
		m_HitTimer += time;
		if (m_HitTimer >= 2000) {
			m_Control = true;
			m_Hit = false;
			m_HitTimer = 0;
		}
	}


	if (m_State == attack) {
		if (!m_AttackTimer)
			m_Control = false;
		m_AttackTimer += time;
		if (m_AttackTimer >= 2000) {
			m_Control = true;
			m_AttackTimer = 0;
			m_Aggro = false;
			SetDeltaX(m_Prev_dx);
		}
	}
}

AnimationManager& Enemy::LoadAnimations() {
	AnimationManager* TmpAnimation;
	TmpAnimation = new AnimationManager();


	TmpAnimation->CreateAnimation("idle", "idle_swordsman", 0, 0, 64, 64, 4, 64, 0.002f);
	TmpAnimation->SetOffsetX("idle", 16, 32);
	TmpAnimation->CreateAnimation("walk", "walk_swordsman", 0, 0, 64, 64, 4, 64, 0.005f);
	TmpAnimation->SetOffsetX("walk", 16, 32);
	TmpAnimation->CreateAnimation("attack", "attack_swordsman", 0, 0, 112, 96, 8, 112, 0.005f);
	TmpAnimation->SetOffsetX("attack", 48, 48);
	TmpAnimation->SetOffsetY("attack", 32);

	return *TmpAnimation;
}
