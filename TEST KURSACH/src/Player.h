#pragma once
#include "Entity.h"

class Player : public Entity {
protected:
	bool m_HitTriggerOn = false;
	bool m_Attacked = false;
	bool m_Jumped = false;
	int m_Score = 0;
	std::map<std::string, bool> m_Key;
public:
	Player();
	Player(Level& level, const float& X, const float& Y);
	~Player();
	void Update(const float& time, std::vector<Entity*>& enteties, Entity& player) override;
	void Control();
	void CheckCollisionWithMap(const float& dx, const float& dy) override;

	AnimationManager& LoadAnimations() override;

	sf::View view;
	sf::View& SetCoordForView(const float& X, const float& Y) {
		float tempX = X; float tempY = Y;

		if (X < 540) tempX = 540;
		if (X > 860) tempX = 860;
		//if (Y < 360) tempY = 360;
		if (Y > 920) tempY = 920;

		view.setCenter(tempX + 100, tempY);
		return view;
	}
};