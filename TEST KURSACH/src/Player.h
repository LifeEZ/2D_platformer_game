#pragma once
#include "Entity.h"
#include "HealthBar.h"

class Player final : public Entity {
protected:
	sf::View m_View;
	HealthBar* m_HealthBar = nullptr;
	std::pair<bool, size_t> m_LvlEnd = { false, 0 };
	bool m_Attacked = false;
	bool m_Jumped = false;
	std::map<std::string, bool> m_Key;
	struct CameraBounds { int Left, Right, Up, Down; } m_CameraBounds = { 0,0,0,0 };
public:
	Player();
	Player(Level& Lvl, const float& X, const float& Y, const int& Damage, const int& Health);
	~Player();
	void Update(const float& Time, std::vector<Entity*>& Entities, Entity& Player) override;
	void Control();
	void CheckCollisionWithMap(const float& Dx, const float& Dy) override;
	sf::View& GetView();
	std::pair<bool, size_t> LvlEnd() const;
	HealthBar& GetHealthBar() const;

	AnimationManager& LoadAnimations() override;

	void CalculateBorders(const Level& Lvl);
	void SetCoordsForView(const float& X, const float& Y);
};