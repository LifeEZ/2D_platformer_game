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
		if (m_Aggro) {
			m_Aggro = false;
		}
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
	else {
		for (size_t i = 0; i < m_Objects.size(); i++) {

			if (!((player.GetPosY() + player.GetHeight()) == m_Objects.at(i).rect.top)) {
				continue;
			}
			if (!((player.GetPosX() -32 <= m_Objects.at(i).rect.left + m_Objects.at(i).rect.width) && (player.GetPosX() +32>= m_Objects.at(i).rect.left))) {
				continue;
			}
			float PlayerObjectLeft = m_Objects.at(i).rect.left;
			float PlayerObjectRight = m_Objects.at(i).rect.left + m_Objects.at(i).rect.width;
			if ((PlayerObjectLeft <= m_X) && (PlayerObjectRight >= m_X)) {
				m_Aggro = true;
			}
		}
	}
}

void Enemy::UpdateState() {
	if (GetDeltaY() < 0)
		m_OnGround = false;
	if (!GetDeltaX()) {
		SetState(idle);
	}
	else 
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
			if(m_dx)
				m_Prev_dx = m_dx;
			if (m_PreAttackTimer >= 400) {
				m_Attack = false;
			}
			SetDeltaX(0);
			if (GetState() != hit && GetState() != attack) {
				SetState(idle);
			}
			if (!m_Attack)
				m_Attack = true;
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
	if (m_State != attack) {
		m_Animation.SetFrame("attack", 0);
	}
	if (m_State != hit) {
		m_Animation.SetFrame("hit", 0);
	}
	ChoosePath(player);
	if (m_Control && !m_Attack) {
		UpdateState();
		Move(time, player);
	}
	if (GetDeltaX() > 0 && m_Direction == left) {
		m_Direction = right;
	}
	if (GetDeltaX() < 0 && m_Direction == right) {
		m_Direction = left;
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
	case hit:
		if (m_Animation.GetName() != "hit")
			m_Animation.Set("hit");
		break;
	}
	int a = rand() % 100;
	if(a % 5 == 0)
		switch (m_State) {
		case Entity::walk:
			std::cout << "walk\n";
			break;
		case Entity::idle:
			std::cout << "idle\n";
			break;
		case Entity::jump:
			std::cout << "jump\n";
			break;
		case Entity::attack:
			std::cout << "attack\n";
			break;
		case Entity::hit:
			std::cout << "hit\n";
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

	if (m_State == attack && m_Aggro && m_HitTriggerOn) {
		if (m_HitTrigger.intersects(player.GetRect())) {
			if (player.GetState() != hit && player.GetState() != dead) {
				player.DamageBy(50);
			}
			player.SetDeltaX(0.f);
			player.SetControl(false);
			if(player.GetState() != dead)
				player.SetState(hit);
		}
	}

	if (m_State == hit) {
		if (m_Aggro)
			m_Aggro = false;
		m_Attack = false;
		m_Control = false;
		float tmpTime = time;
		m_HitTimer += tmpTime;
		if (m_HitTimer >= 1000) {
			m_Control = true;
			m_HitTimer = 0;
		}
	}


	if (m_Attack) {
		float tmpTime = time;
		m_PreAttackTimer += tmpTime;
		if (m_PreAttackTimer >= 400) {
			if (m_State != hit) {
				SetState(attack);
				m_Attack = false;
			}
		}
	}
	if (m_State == attack) {
		float tmpTime = time;
		if (!m_AttackTimer)
			m_Control = false;
		m_AttackTimer += tmpTime;
		if (m_AttackTimer >= 1000)
			m_HitTriggerOn = true;
		if (m_AttackTimer >= 2000) {
			m_HitTriggerOn = false;
			m_Control = true;
			m_AttackTimer = 0;
			m_Aggro = false;
			SetDeltaX(m_Prev_dx);
			m_PreAttackTimer = 0;
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
	TmpAnimation->SetOffsetX("attack", 64, 32);
	TmpAnimation->SetOffsetY("attack", 32);
	TmpAnimation->CreateAnimation("hit", "hit_swordsman", 0, 0, 64, 64, 3, 64, 0.005f);
	TmpAnimation->SetOffsetX("hit", 16, 32);

	return *TmpAnimation;
}
