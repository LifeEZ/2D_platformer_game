#include "Player.h"
#include <iostream>

Player::Player() {
}

Player::Player(Level& Level, const float& X, const float& Y)
: Entity(X, Y) {
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
	//if (m_State == hit) {
	//	m_HitTimer = 0;
	//}
	if (m_State != jump) {
		m_Animation.SetFrame("jump", 0);
	}
	if (m_State != walk) {
		m_Animation.SetFrame("walk", 0);
	}
	if (m_State != attack) {
		m_Animation.SetFrame("attack", 0);
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
	//if (!m_Key["Down"]) {
	//	if (m_State == duck)
	//		m_State = idle;
	//}
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
	}
	int a = rand() % 100;
	//if (a % 23 == 0)
	//	m_Control ? std::cout << "on\n" : std::cout << "off\n";
		//switch (m_State) {
		//case Player::walk:
		//	std::cout << "state: walk\n";
		//	break;
		//case Player::idle:
		//	std::cout << "state: idle\n";
		//	break;
		//case Player::jump:
		//	std::cout << "state: jump\n";
		//	break;
		//case Player::attack:
		//	std::cout << "state: attack\n";
		//	break;
		//case Player::hit:
		//	std::cout << "state: hit\n";
		//	break;
		//}
	m_Animation.Update(time, (m_Direction == left ? Animation::AnimDirection::left : Animation::AnimDirection::right));

	m_X += m_dx * time;
	CheckCollisionWithMap(m_dx, 0);
	m_dy += 0.001f * time;
	m_Y += m_dy * time;
	CheckCollisionWithMap(0, m_dy);
	UpdateHitTrigger();
	//if (a % 97 == 0)
	//	std::cout << "X " << m_HitTrigger.left << " Y " << m_HitTrigger.top << " witdth "<< m_HitTrigger.width << " height " << m_HitTrigger.height << "PlayerX + W" << m_X + m_W << "\n";
	if (m_Health <= 0)
		m_Alive = false;

	if (m_Alive)
		SetCoordForView(m_X, m_Y);

	for (size_t i = 0; i < enteties.size(); i++) {
		if (GetRect().intersects(enteties.at(i)->GetRect()) && !m_Hit) {
			DamageBy(10);
			m_dy = -0.3f;
			m_OnGround = false;
			(enteties.at(i)->GetPosX() - m_X) > 0 ? m_dx = -GetSpeed(): m_dx = GetSpeed();
			m_Hit = true;
		}
	}
	if (m_State == attack && m_Attacked && !m_Hit && m_HitTriggerOn) {
		for (size_t i = 0; i < enteties.size(); i++) {
			if (m_HitTrigger.intersects(enteties.at(i)->GetRect())) {
				enteties.at(i)->DamageBy(10);
				if ((m_X - enteties.at(i)->GetPosX()) > 0)
					enteties.at(i)->SetDeltaX(-enteties.at(i)->GetSpeed());
				else
					enteties.at(i)->SetDeltaX(enteties.at(i)->GetSpeed());
				enteties.at(i)->m_Hit = true;
			}
		}
	}

	if (m_Hit) {
		m_Control = false;
		float tmpTime = time;
		m_HitTimer += tmpTime;
		if (m_HitTimer >= 700) {
			m_Control = true;
		}
		if (m_HitTimer >= 1700) {
			m_Hit = false;
			m_HitTimer = 0;
		}
	}

	if (m_State == attack) {
		if (!m_AttackTimer)
			m_Control = false;
		float tmpTime = time;
		m_AttackTimer += tmpTime/1.3f;
		//if (m_AttackTimer >= 500)
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
		}
	}
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

	return *TmpAnimation;
}
