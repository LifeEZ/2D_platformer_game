#include "Player.h"
#include <iostream>

Player::Player() {
	m_HealthBar = new HealthBar();
}

Player::Player(Level& Level, const float& X, const float& Y)
: Entity(X, Y) {
	m_HealthBar = new HealthBar();
	m_Animation = LoadAnimations();
	m_Key.insert(std::pair<std::string, bool>("R", false));
	m_Key.insert(std::pair<std::string, bool>("L", false));
	m_Key.insert(std::pair<std::string, bool>("U", false));
	m_Key.insert(std::pair<std::string, bool>("A", false));
	m_Objects = Level.GetAllObjects();
}

Player::~Player() {
}

void Player::Control() {
	if (m_State != jump) {
		m_Animation.SetFrame("jump", 0);
	}
	if (m_State != walk) {
		m_Animation.SetFrame("walk", 0);
	}
	if (m_State != attack) {
		m_Animation.SetFrame("attack", 0);
	}
	if (m_State != hit) {
		m_Animation.SetFrame("hit", 0);
	}
	if (m_State == hit) {
		m_State = idle;
	}
	if (m_State == attack) {
		m_State = idle;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		m_Key.at("R") = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		m_Key.at("L") = true;
	}
	bool Jumping = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	if (!m_Jumped && Jumping) {
		m_Key.at("U") = true;
	}
	m_Jumped = Jumping;
	bool Attacking = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	if (!m_Attacked && Attacking) {
		m_Key.at("A") = true;
	}
	m_Attacked = Attacking;

	if (m_Key.at("A")) {
		if (m_State != jump && m_State != attack) {
			m_dx = 0;
			m_Control = false;
			m_State = attack;
		}
	}
	bool twoKeysPressed = false;
	if (m_Key.at("R") && m_Key.at("L")) {
		m_dx = 0;
		if (m_State == walk)
			m_State = idle;
		twoKeysPressed = true;
	}
	if (m_Key.at("R") && !twoKeysPressed) {
		m_Direction = right;
		if(m_State != attack)
			m_dx = m_Speed;
		if (m_State == idle)
			m_State = walk;
	}
	if (m_Key.at("L") && !twoKeysPressed) {
		m_Direction = left;
		if (m_State != attack)
			m_dx = -m_Speed;
		if (m_State == idle)
			m_State = walk;
	}

	if (m_Key.at("U")) {
		if (m_State != jump && m_State != attack && m_OnGround) {
			m_dy = -0.5f;
			m_State = jump;
			m_OnGround = false;
		}
	}
	if (!(m_Key.at("R") || m_Key.at("L"))) {
		m_dx = 0;
		if (m_State == walk)
			m_State = idle;
	}
	m_Key.at("R") = false;
	m_Key.at("L") = false;
	m_Key.at("U") = false;
	m_Key.at("A") = false;
}

void Player::Update(const float& time, std::vector<Entity*>& enteties, Entity& player) {
	if (m_Control) {
		Control();
	}
	switch (m_State) {
	case walk:
		if (m_Animation.GetName() != "walk")
			m_Animation.Set("walk");
		break;

	case jump:
		if (m_Animation.GetName() != "jump")
			m_Animation.Set("jump");
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
	}
	m_HealthBar->Update(m_Health);
	if (m_Animation.GetCurrentFrame("dead") < 5) {
		m_Animation.Update(time, (m_Direction == left ? Animation::AnimDirection::left : Animation::AnimDirection::right));
	}

	m_X += m_dx * time;
	CheckCollisionWithMap(m_dx, 0);
	m_dy += 0.001f * time;
	m_Y += m_dy * time;
	CheckCollisionWithMap(0, m_dy);
	UpdateHitTrigger();
	if (m_Health <= 0 && m_State != dead) {
		SetState(dead);
		m_dx = 0.f;
		m_Control = false;
	}
	if (m_State == dead) {
		float tmpTime = time;
		m_DeathTimer += tmpTime;
		if (m_DeathTimer >= 3000) {
			m_LvlEnd.first = true;
			m_LvlEnd.second = 0;
		}
	}
	SetCoordForView(m_X, m_Y);
	//if (!m_Immune) {
	//	for (size_t i = 0; i < enteties.size(); i++) {
	//		if (GetRect().intersects(enteties.at(i)->GetRect())) {
	//			if (player.GetState() != hit) {
	//				DamageBy(10);
	//			}
	//			(enteties.at(i)->GetPosX() - m_X) > 0 ? m_dx = -GetSpeed() : m_dx = GetSpeed();
	//			SetState(hit);
	//			m_Immune = true;
	//		}
	//	}
	//}
	//else {
	//	m_ImmuneTimer += time / 1.2f;
	//	if (m_ImmuneTimer >= 2000.f)
	//		m_Immune = false;
	//}
	//??????????????????
	if (m_State == attack && m_Attacked && m_HitTriggerOn) {
		for (size_t i = 0; i < enteties.size(); i++) {
			if (m_HitTrigger.intersects(enteties.at(i)->GetRect()) && (enteties.at(i)->GetState() != hit)) {
				enteties.at(i)->DamageBy(10);
				enteties.at(i)->SetDeltaX(0.f);
				enteties.at(i)->SetState(hit);
				enteties.at(i)->SetControl(false);
			}
		}
	}

	if (m_State == hit) {
		m_Control = false;
		float tmpTime = time;
		m_HitTimer += tmpTime;

		if (m_HitTimer >= 500) {
			m_Control = true;
		}
		if (m_HitTimer >= 2000) {
			m_HitTimer = 0;
		}
	}

	if (m_State == attack) {
		if (!m_AttackTimer)
			m_Control = false;
		float tmpTime = time;
		m_AttackTimer += tmpTime / 1.3f;
		if (m_AttackTimer >= 500)
			m_HitTriggerOn = true;
		if (m_AttackTimer >= 1200) {
			m_HitTriggerOn = false;
			m_Control = true;
			m_AttackTimer = 0;
		}
	}
}

void Player::CheckCollisionWithMap(const float& dx, const float& dy) {
	for (unsigned int i = 0; i < m_Objects.size(); i++) {
		if (GetRect().intersects(m_Objects[i].rect)) {
			if (m_Objects[i].name == "solid") {
				if (dy > 0) {
					m_Y = m_Objects[i].rect.top - m_H;
					m_dy = 0;
					if(!m_OnGround)
						if(m_State != hit)
							m_State = idle;
					m_OnGround = true;
				}
				if (dy < 0) {
					m_Y = m_Objects[i].rect.top + m_Objects[i].rect.height;
					m_dy = 0;
				}
				if (dx > 0) {
					m_X = m_Objects[i].rect.left - m_W;
					m_dx = 0;
				}
				if (dx < 0) {
					m_X = m_Objects[i].rect.left + m_Objects[i].rect.width;
					m_dx = 0;
				}
			}
			if (m_Objects[i].name == "end_level") {
				m_LvlEnd.first = true;
				m_LvlEnd.second = 1;
			}
		}
	}
}

std::pair<bool,size_t> Player::LvlEnd() {
	return m_LvlEnd;
}

HealthBar& Player::GetHealthBar() {
	return *m_HealthBar;
}

AnimationManager& Player::LoadAnimations() {
	AnimationManager* TmpAnimation;
	TmpAnimation = new AnimationManager();


	TmpAnimation->CreateAnimation("walk", "walk", 0, 0, 64, 64, 11, 64);
	TmpAnimation->SetOffsetX("walk", 16, 16);
	TmpAnimation->CreateAnimation("jump", "jump", 0, 0, 48, 64, 14, 48);
	TmpAnimation->SetOffsetX("jump", 0, 16);
	TmpAnimation->CreateAnimation("idle", "idle", 0, 0, 64, 64, 6, 64, 0.003f);
	TmpAnimation->SetOffsetX("idle", 16, 16);
	TmpAnimation->CreateAnimation("attack", "attack", 0, 0, 112, 67, 7, 112, 0.005f);
	TmpAnimation->SetOffsetX("attack", 48, 32);
	TmpAnimation->SetOffsetY("attack", 3);
	TmpAnimation->CreateAnimation("hit", "hitA", 0, 0, 80, 64, 4, 80, 0.005f);
	TmpAnimation->SetOffsetX("hit", 16, 32);
	TmpAnimation->CreateAnimation("dead", "dead", 0, 0, 80, 64, 6, 80, 0.005f);
	TmpAnimation->SetOffsetX("dead", 32, 16);

	return *TmpAnimation;
}
