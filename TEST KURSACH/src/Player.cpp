#include "Player.h"
#include <iostream>

Player::Player() {
	m_HealthBar = new HealthBar();
}

Player::Player(Level& Lvl, const float& X, const float& Y, const int& Damage, const int& Health)
: Entity(X, Y) {
	CalculateBorders(Lvl);
	m_Damage = Damage;
	m_Health = Health;
	m_HealthBar = new HealthBar();
	m_Animation = LoadAnimations();
	m_Key.insert(std::pair<std::string, bool>("R", false));
	m_Key.insert(std::pair<std::string, bool>("L", false));
	m_Key.insert(std::pair<std::string, bool>("U", false));
	m_Key.insert(std::pair<std::string, bool>("A", false));
	m_Objects = Lvl.GetAllObjects();
}

Player::~Player() = default;

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
	const bool jumping = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	if (!m_Jumped && jumping) {
		m_Key.at("U") = true;
	}
	m_Jumped = jumping;
	const bool attacking = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	if (!m_Attacked && attacking) {
		m_Key.at("A") = true;
	}
	m_Attacked = attacking;

	if (m_Key.at("A")) {
		if (m_State != jump && m_State != attack) {
			m_Dx = 0;
			m_Control = false;
			m_State = attack;
		}
	}
	bool two_keys_pressed = false;
	if (m_Key.at("R") && m_Key.at("L")) {
		m_Dx = 0;
		if (m_State == walk)
			m_State = idle;
		two_keys_pressed = true;
	}
	if (m_Key.at("R") && !two_keys_pressed) {
		m_Direction = right;
		if(m_State != attack)
			m_Dx = m_Speed;
		if (m_State == idle)
			m_State = walk;
	}
	if (m_Key.at("L") && !two_keys_pressed) {
		m_Direction = left;
		if (m_State != attack)
			m_Dx = -m_Speed;
		if (m_State == idle)
			m_State = walk;
	}

	if (m_Key.at("U")) {
		if (m_State != jump && m_State != attack && m_OnGround) {
			m_Dy = -0.5f;
			m_State = jump;
			m_OnGround = false;
		}
	}
	if (!(m_Key.at("R") || m_Key.at("L"))) {
		m_Dx = 0;
		if (m_State == walk)
			m_State = idle;
	}
	m_Key.at("R") = false;
	m_Key.at("L") = false;
	m_Key.at("U") = false;
	m_Key.at("A") = false;
}

void Player::Update(const float& Time, std::vector<Entity*>& Entities, Entity& Player) {
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
		m_Animation.Update(Time, (m_Direction == left ? Animation::AnimDirection::left : Animation::AnimDirection::right));
	}

	m_X += m_Dx * Time;
	CheckCollisionWithMap(m_Dx, 0);
	m_Dy += 0.001f * Time;
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
			m_LvlEnd.first = true;
			m_LvlEnd.second = 0;
		}
	}
	SetCoordsForView(m_X, m_Y);
	//if (!m_Immune) {
	//	for (size_t i = 0; i < entities.size(); i++) {
	//		if (GetRect().intersects(entities.at(i)->GetRect())) {
	//			if (player.GetState() != hit) {
	//				DamageBy(10);
	//			}
	//			(entities.at(i)->GetPosX() - m_X) > 0 ? m_dx = -GetSpeed() : m_dx = GetSpeed();
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
		for (auto& entity : Entities) {
			if (m_HitTrigger.intersects(entity->GetRect()) && (entity->GetState() != hit) && (entity->GetState() != dead)) {
				entity->DamageBy(m_Damage);
				entity->SetDeltaX(0.f);
				entity->SetState(hit);
				entity->SetControl(false);
			}
		}
	}

	if (m_State == hit) {
		m_Control = false;
		const float tmp_time = Time;
		m_HitTimer += tmp_time;

		if (m_HitTimer >= 500) {
			m_Control = true;
		}
		if (m_HitTimer >= 2000) {
			m_HitTimer = 0;
		}
	}

	if (m_State == attack) {
		if (m_AttackTimer == 0)
			m_Control = false;
		const float tmp_time = Time;
		m_AttackTimer += tmp_time / 1.3f;
		if (m_AttackTimer >= 500)
			m_HitTriggerOn = true;
		if (m_AttackTimer >= 1200) {
			m_HitTriggerOn = false;
			m_Control = true;
			m_AttackTimer = 0;
		}
	}
}

void Player::CheckCollisionWithMap(const float& Dx, const float& Dy) {
	for (auto& object : m_Objects) {
		if (GetRect().intersects(object.Rect)) {
			if (object.Name == "solid") {
				if (Dy > 0) {
					m_Y = object.Rect.top - static_cast<float>(m_H);
					m_Dy = 0;
					if(!m_OnGround)
						if(m_State != hit)
							m_State = idle;
					m_OnGround = true;
				}
				if (Dy < 0) {
					m_Y = object.Rect.top + object.Rect.height;
					m_Dy = 0;
				}
				if (Dx > 0) {
					m_X = object.Rect.left - static_cast<float>(m_W);
					m_Dx = 0;
				}
				if (Dx < 0) {
					m_X = object.Rect.left + object.Rect.width;
					m_Dx = 0;
				}
			}
			if (object.Name == "end_level") {
				m_LvlEnd.first = true;
				m_LvlEnd.second = 1;
			}
		}
	}
}

sf::View& Player::GetView(){
	return m_View;
}

std::pair<bool, size_t> Player::LvlEnd() const {
	return m_LvlEnd;
}

HealthBar& Player::GetHealthBar() const {
	return *m_HealthBar;
}

AnimationManager& Player::LoadAnimations() {
	AnimationManager* tmp_animation = new AnimationManager();

	tmp_animation->CreateAnimation("walk", "Player/walk", 0, 0, 64, 64, 11, 64);
	tmp_animation->SetOffsetX("walk", 16, 16);
	tmp_animation->CreateAnimation("jump", "Player/jump", 0, 0, 48, 64, 14, 48);
	tmp_animation->SetOffsetX("jump", 0, 16);
	tmp_animation->CreateAnimation("idle", "Player/idle", 0, 0, 64, 64, 6, 64, 0.003f);
	tmp_animation->SetOffsetX("idle", 16, 16);
	tmp_animation->CreateAnimation("attack", "Player/attack", 0, 0, 112, 67, 7, 112, 0.005f);
	tmp_animation->SetOffsetX("attack", 48, 32);
	tmp_animation->SetOffsetY("attack", 3);
	tmp_animation->CreateAnimation("hit", "Player/hit", 0, 0, 80, 64, 4, 80, 0.005f);
	tmp_animation->SetOffsetX("hit", 16, 32);
	tmp_animation->CreateAnimation("dead", "Player/dead", 0, 0, 80, 64, 6, 80, 0.005f);
	tmp_animation->SetOffsetX("dead", 32, 16);

	return *tmp_animation;
}

void Player::CalculateBorders(const Level& Lvl) {
	m_CameraBounds.Up = 720 / 2;
	m_CameraBounds.Down = Lvl.GetHeight() * 16 - 720 / 2;
	m_CameraBounds.Left = 1280 / 2 - 100;
	m_CameraBounds.Right = Lvl.GetWidth() * 16 - 1280 / 2 - 100;
}

void Player::SetCoordsForView(const float& X, const float& Y) {
		float temp_x = X, temp_y = Y;
		if (X < static_cast<float>(m_CameraBounds.Left)) temp_x = static_cast<float>(m_CameraBounds.Left);
		if (X > static_cast<float>(m_CameraBounds.Right)) temp_x = static_cast<float>(m_CameraBounds.Right);
		if (Y < static_cast<float>(m_CameraBounds.Up)) temp_y = static_cast<float>(m_CameraBounds.Up);
		if (Y > static_cast<float>(m_CameraBounds.Down)) temp_y = static_cast<float>(m_CameraBounds.Down);
		m_View.setCenter(temp_x + 100, temp_y);
}
