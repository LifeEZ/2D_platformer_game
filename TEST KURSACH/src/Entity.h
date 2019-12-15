#pragma once
#include "AnimationManager.h"
#include "SFML/Graphics.hpp"
#include "..//level.h"
#include <vector>

class Entity {
public:
	enum StateObject { walk, idle, jump, attack, hit, dead };
	enum Direction { left, right };
protected:
	AnimationManager m_Animation;
	StateObject m_State = idle;
	std::vector<Object> m_Objects;
	sf::FloatRect m_HitTrigger = { 0, 0, 16.f, 64.f };
	float m_X = 0.f, m_Y = 0.f;
	float m_Dx = 0.f, m_Dy = 0.f;
	int m_W = 32, m_H = 64;
	float m_Speed = 0.15f;
	int m_Damage = 50;
	int m_Health = 100;
	bool m_Alive = true;
	bool m_OnGround = false;
	bool m_HitTriggerOn = false;
	Direction m_Direction = right;
	std::string m_Name = "name";
	float m_HitTimer = 0.f;
	float m_AttackTimer = 0.f;
	bool m_Control = true;
	float m_DeathTimer = 0.f;
public:
	Entity();
	Entity(const float& X, const float& Y);
	virtual ~Entity();
	virtual AnimationManager& LoadAnimations() = 0;
	void SetSpeed(const float& Speed);
	const float& GetSpeed() const;
	const float& GetPosX() const;
	const int& GetWidth() const;
	const int& GetHeight() const;
	void SetPosX(const float& X);
	const float& GetPosY() const;
	void SetPosY(const float& Y);
	const int& GetHealth() const;
	void SetHealth(const int& Health);
	const float& GetDeltaX() const;
	void SetDeltaX(const float& Dx);
	const float& GetDeltaY() const;
	void SetDeltaY(const float& Dy);
	const std::string& GetName() const;
	void DamageBy(const int& Damage);
	void HealBy(const int& Heal);
	void SetControl(const bool& Control);
	void SetState(const StateObject& State);
	const bool& Alive() const;
	const StateObject& GetState() const;
	void UpdateHitTrigger();
	virtual void Update(const float& Time, std::vector<Entity*>& Entities, Entity& Player) = 0;
	virtual void CheckCollisionWithMap(const float& Dx, const float& Dy) = 0;
	sf::FloatRect GetRect() const;
	void Draw(sf::RenderWindow& Window);
};
