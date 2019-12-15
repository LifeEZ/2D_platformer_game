#pragma once
#include "Animation.h"

class AnimationManager {
public:
	void CreateAnimation(const std::string& Name, const std::string& ImgDirectory,
		const int& X, const int& Y, const int& W, const int& H, const int& FramesCount, const int& FramesStep, const float& PlaySpeed = 0.01f);

	AnimationManager();
	~AnimationManager();

	std::string GetName() const;
	int GetCurrentFrame(const std::string& name);
	int GetFramesAmount(const std::string& name);

	void Draw(sf::RenderWindow& Window, const float& X, const float& Y);
	void Set(const std::string& Name);

	Animation::AnimDirection GetDirection() const;
	void SetDirection(const Animation::AnimDirection& Direction);
	void SetFrame(const std::string& name, const int& Frame);

	float GetOffsetX(const std::string& name);
	float GetOffsetY(const std::string& name);
	void SetOffsetX(const std::string& name, const float& LeftOffset, const float& RightOffset);
	void SetOffsetY(const std::string& name, const float& Offset);
	void UpdateOffsetDir(const std::string& name);

	void Rotate();
	void Update(const float& Time, const Animation::AnimDirection& Direction);
	void Pause();
	void Play();
	void Play(const std::string& Name);
private:
	std::string m_CurrentAnimation;
	std::map<std::string, Animation> m_AnimationList;
};
