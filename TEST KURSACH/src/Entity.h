#pragma once
#include "AnimationManager.h"
#include "SFML/Graphics.hpp"
#include "..//level.h"
#include <vector>

class Entity {
public:
	enum StateObject { walk, idle, jump, attack, hit };
	enum Direction { left, right };
protected:
	AnimationManager m_Animation;
	StateObject m_State = idle;
	std::vector<Object> m_Objects;
	sf::FloatRect m_HitTrigger = { 0, 0, 16.f, 64.f };
	float m_X = 0.f, m_Y = 0.f;
	float m_dx = 0.f, m_dy = 0.f;
	int m_W = 32, m_H = 64;
	float m_Speed = 0.15f;
	bool m_Control = true;
	int m_Health = 100;
	bool m_Alive = true;
	bool m_OnGround = false;
	Direction m_Direction = right;
	std::string name = "name";
	float m_HitTimer = 0.f;
	float m_AttackTimer = 0.f;
public:
	bool m_Hit = false;
	Entity();
	Entity(const float& X, const float& Y);
	virtual ~Entity();
	virtual AnimationManager& LoadAnimations() = 0;
	void SetSpeed(const float& Speed);
	const float& GetSpeed();
	const float& GetPosX();
	const int& GetWidth();
	const int& GetHeight();
	void SetPosX(const float& X);
	const float& GetPosY();
	void SetPosY(const float& Y);
	const int& GetHealth();
	void SetHealth(const int& Health);
	const float& GetDeltaX();
	void SetDeltaX(const float& dx);
	const float& GetDeltaY();
	void SetDeltaY(const float& dy);
	void DamageBy(const int& Damage);
	void HealBy(const int& Heal);
	void SetState(const StateObject& State);
	const StateObject& GetState();
	void UpdateHitTrigger();
	virtual void Update(const float& time, std::vector<Entity*>& enteties, Entity& player) = 0;
	virtual void CheckCollisionWithMap(const float& dx, const float& dy) = 0;
	sf::FloatRect GetRect();
	void Draw(sf::RenderWindow& Window);
};
