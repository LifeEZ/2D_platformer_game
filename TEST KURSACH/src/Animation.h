#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

class Animation {
public:
	enum AnimDirection { right, left };
	struct Offsets {
		float LeftDirOffset;
		float RightDirOffset;
	};
private:
	std::vector<sf::IntRect> m_Frames, m_FlipedFrames;
	float m_Speed = 0.1f;
	bool m_IsPlaying = true;
	float m_CurrentFrame = 0.f;
	Offsets m_DirOffsets = { 0.f,0.f };
	float m_OffsetY = 0.f;
	float m_OffsetX = m_DirOffsets.RightDirOffset;
	AnimDirection m_Direction = right;
	sf::Texture* m_Texture = nullptr;
	sf::Sprite* m_Sprite = nullptr;
public:
	float GetOffsetX();
	float GetOffsetY();
	void SetOffsetX(const float& LeftOffset, const float& RightOffset);
	void SetOffsetY(const float& Offset);
	void UpdateOffsetDir();
	int GetCurrentFrame() const;
	void SetCurrentFrame(const int& Frame);
	int GetFramesAmount() const;
	AnimDirection GetDirection() const;
	void SetDirection(const Animation::AnimDirection& Direction);
	void Rotate();
	sf::Sprite* GetSprite();
	void Pause();
	void Play();
	Animation(const std::string& ImgDirectory, const int& X, const int& Y, const int& W, const int& H, const unsigned int& FramesCount, const int& FramesStep, const float& PlaySpeed = 0.01f);
	Animation();
	virtual ~Animation();
	void Tick(const float& time);
};