#include "Enemy.h"

Enemy::~Enemy() = default;

Enemy::Enemy(Level& Level, const float& X, const float& Y, const int& Damage, const int& Health)
	: Entity(X, Y) {
	m_Damage = Damage;
	m_Health = Health;
	m_W = 16;
	m_H = 64;
	m_Animation = Enemy::LoadAnimations();
	m_Objects = Level.GetObjects("solid");
	m_Name = "swordsman";
	SetSpeed(0.1f);
	SetDeltaX(0.1f);
}

Enemy::Enemy() = default;


void Enemy::ChoosePath(Entity& Player) {
	if (Player.GetPosY() != GetPosY()) {
		if (m_Aggro) {
			m_Aggro = false;
		}
		for (size_t i = 0; i < m_Objects.size(); i++) {
			if (!((m_Y + static_cast<float>(m_H)) == m_Objects.at(i).Rect.top)) {
				continue;
			}
			if (!((m_X <= m_Objects.at(i).Rect.left + m_Objects.at(i).Rect.width) && (m_X >= m_Objects.at(i).Rect.left))) {
				continue;
			}
			m_PathBorders.LeftPoint = m_Objects.at(i).Rect.left + 32;
			m_PathBorders.RightPoint = m_Objects.at(i).Rect.left + m_Objects.at(i).Rect.width - 32;
			m_ObjectForPath = m_Objects.at(i).Rect;
			m_ChosenObjectIndex = i;
		}
	}
	else {
		for (auto& object : m_Objects) {
			if (!((Player.GetPosY() + static_cast<float>(Player.GetHeight())) == object.Rect.top)) {
				continue;
			}
			if (!((Player.GetPosX() - 32 <= object.Rect.left + object.Rect.width) && (Player.GetPosX() + 32 >=
				object.Rect.left))) {
				continue;
			}
			const float player_object_left = object.Rect.left;
			const float player_object_right = object.Rect.left + object.Rect.width;
			if ((player_object_left <= m_X) && (player_object_right >= m_X)) {
				m_Aggro = true;
			}
		}
	}
}

void Enemy::UpdateState() {
	if (GetDeltaY() < 0)
		m_OnGround = false;
	if (GetDeltaX() == 0) {
		SetState(idle);
	}
	else
		SetState(walk);
}

void Enemy::Move(const float& Time, Entity& Player) {
	if (!m_Aggro) {
		if (m_ObjectForPath.width > 0) {
			if (GetPosX() + static_cast<float>(GetWidth()) >= m_PathBorders.RightPoint) {
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
		if (m_HitTrigger.intersects(Player.GetRect())) {
			if (m_Dx != 0)
				m_PrevDx = m_Dx;
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
		if (Player.GetPosX() > GetPosX()) {
			SetDeltaX(GetSpeed());
			m_Direction = right;
		}
		else {
			SetDeltaX(-GetSpeed());
			m_Direction = left;
		}
	}
}

void Enemy::CheckCollisionWithMap(const float& Dx, const float& Dy) {
	for (auto& object : m_Objects) {
		if (GetRect().intersects(object.Rect)) {
			if (Dy > 0) {
				m_Y = object.Rect.top - static_cast<float>(m_H);
				m_OnGround = true;
				m_Dy = 0;
			}
			if (Dy < 0) {
				m_Y = object.Rect.top + object.Rect.height;
				m_Dy = 0;
			}
			if (Dx > 0) {
				m_X = object.Rect.left - static_cast<float>(m_W);
				m_Dx = 0;
				continue;
			}
			if (Dx < 0) {
				m_X = object.Rect.left + object.Rect.width;
				m_Dx = 0;
				continue;
			}
		}
	}
}

void Enemy::Update(const float& Time, std::vector<Entity*>& Entities, Entity& Player) {
	if (m_State != attack) {
		m_Animation.SetFrame("attack", 0);
	}
	if (m_State != hit) {
		m_Animation.SetFrame("hit", 0);
	}
	ChoosePath(Player);
	if (m_Control && !m_Attack) {
		UpdateState();
		Move(Time, Player);
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
	case dead:
		if (m_Animation.GetName() != "dead")
			m_Animation.Set("dead");
		break;
	case jump: break;
	}
	if (m_Animation.GetCurrentFrame("dead") < 4) {
		m_Animation.Update(Time, (m_Direction == left ? Animation::AnimDirection::left : Animation::AnimDirection::right));
	}

	m_X += m_Dx * Time;
	CheckCollisionWithMap(m_Dx, 0);
	m_Dy += 0.0010f * Time;
	m_Y += m_Dy * Time;
	CheckCollisionWithMap(0, m_Dy);
	UpdateHitTrigger();

	if (m_Health <= 0 && m_State != dead) {
		SetState(dead);
		m_Dx = 0.f;
		m_Control = false;
	}
	if (m_State == dead) {
		const float tmp_time = Time;
		m_DeathTimer += tmp_time;
		if (m_DeathTimer >= 3000) {
			m_Alive = false;
		}
	}

	if (m_State == attack && m_Aggro && m_HitTriggerOn) {
		if (m_HitTrigger.intersects(Player.GetRect())) {
			if (Player.GetState() != hit && Player.GetState() != dead) {
				Player.DamageBy(m_Damage);
			}
			Player.SetDeltaX(0.f);
			Player.SetControl(false);
			if (Player.GetState() != dead)
				Player.SetState(hit);
		}
	}

	if (m_State == hit) {
		if (m_Aggro)
			m_Aggro = false;
		m_Attack = false;
		m_Control = false;
		const float tmp_time = Time;
		m_HitTimer += tmp_time;
		if (m_HitTimer >= 1000) {
			m_Control = true;
			m_HitTimer = 0;
		}
	}
	if (m_Attack) {
		const float tmp_time = Time;
		m_PreAttackTimer += tmp_time;
		if (m_PreAttackTimer >= 400) {
			if (m_State != hit) {
				SetState(attack);
				m_Attack = false;
			}
		}
	}
	if (m_State == attack) {
		const float tmp_time = Time;
		if (m_AttackTimer == 0)
			m_Control = false;
		m_AttackTimer += tmp_time;
		if (m_AttackTimer >= 1000)
			m_HitTriggerOn = true;
		if (m_AttackTimer >= 2000) {
			m_HitTriggerOn = false;
			m_Control = true;
			m_AttackTimer = 0;
			m_Aggro = false;
			SetDeltaX(m_PrevDx);
			m_PreAttackTimer = 0;
		}
	}
}

AnimationManager& Enemy::LoadAnimations() {
	AnimationManager* tmp_animation = new AnimationManager();


	tmp_animation->CreateAnimation("idle", "Swordsman/idle", 0, 0, 64, 64, 4, 64, 0.002f);
	tmp_animation->SetOffsetX("idle", 16, 32);
	tmp_animation->CreateAnimation("walk", "Swordsman/walk", 0, 0, 64, 64, 4, 64, 0.005f);
	tmp_animation->SetOffsetX("walk", 16, 32);
	tmp_animation->CreateAnimation("attack", "Swordsman/attack", 0, 0, 112, 96, 8, 112, 0.005f);
	tmp_animation->SetOffsetX("attack", 64, 32);
	tmp_animation->SetOffsetY("attack", 32);
	tmp_animation->CreateAnimation("hit", "Swordsman/hit", 0, 0, 64, 64, 3, 64, 0.005f);
	tmp_animation->SetOffsetX("hit", 16, 32);
	tmp_animation->CreateAnimation("dead", "Swordsman/dead", 0, 0, 80, 64, 5, 80, 0.005f);
	tmp_animation->SetOffsetX("dead", 32, 32);

	return *tmp_animation;
}
