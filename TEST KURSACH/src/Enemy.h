#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
	~Enemy();
	Enemy();
	Enemy(Level& Level, const float& X, const float& Y, const int& Damage, const int& Health);
	void ChoosePath(Entity& Player);
	void UpdateState();
	void Move(const float& Time, Entity& Player);
	void CheckCollisionWithMap(const float& Dx, const float& Dy) override;
	void Update(const float& Time, std::vector<Entity*>& Entities, Entity& Player) override;
	AnimationManager& LoadAnimations() override;
protected:
	float m_CurrentPath = 0.f;
	float m_PrevPath = 0.f;
	sf::FloatRect m_ObjectForPath = { 0.f,0.f,0.f,0.f };
	size_t m_ChosenObjectIndex = 0;
	struct PathBorders {
		float LeftPoint = 0.f;
		float RightPoint = 0.f;
	} m_PathBorders;
	bool m_Aggro = false;
	bool m_Attack = false;
	Direction m_PrevDirection = left;
	float m_IdleTimer = 0.f;
	float m_PreAttackTimer = 0.f;
	float m_PostAttackTimer = 0.f;
	float m_PrevDx = 0.f;
};