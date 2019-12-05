#include "Animation.h"
#include <iostream>

Animation::~Animation() {
	//delete m_Texture;
	//delete m_Sprite;
}

Animation::Animation() {
	m_Texture = new sf::Texture();
	m_Sprite = new sf::Sprite();
}

float Animation::GetOffsetX() {
	return m_OffsetX;
}

float Animation::GetOffsetY() {
	return m_OffsetY;
}

void Animation::SetOffsetX(const float& LeftOffset, const float& RightOffset) {
	m_DirOffsets.LeftDirOffset = LeftOffset;
	m_DirOffsets.RightDirOffset = RightOffset;
}

void Animation::SetOffsetY(const float& Offset) {
	m_OffsetY = Offset;
}

void Animation::UpdateOffsetDir() {
	m_Direction == right ? m_OffsetX = m_DirOffsets.RightDirOffset : m_OffsetX = m_DirOffsets.LeftDirOffset;
}

int Animation::GetCurrentFrame() const {
	return static_cast<int>(m_CurrentFrame);
}

void Animation::SetCurrentFrame(const int& Frame) {
	m_CurrentFrame = static_cast<float>(Frame);
}

int Animation::GetFramesAmount() const {
	return m_Frames.size();
}

Animation::AnimDirection Animation::GetDirection() const {
	return m_Direction;
}

void Animation::SetDirection(const Animation::AnimDirection& Direction) {
	m_Direction = Direction;
}

void Animation::Rotate() {
	m_Direction == right ? m_Direction = left : m_Direction = right;
}

sf::Sprite* Animation::GetSprite() {
	return m_Sprite;
}

void Animation::Pause() {
	m_IsPlaying = false;
}

void Animation::Play() {
	m_IsPlaying = true;
}

Animation::Animation(const std::string& ImgDirectory, const int& X, const int& Y, const int& W, const int& H, const unsigned int& FramesCount, const int& FramesStep, const float& PlaySpeed)
	: m_Speed(PlaySpeed) {
	m_Texture = new sf::Texture();
	if (!m_Texture->loadFromFile("../res/Textures/" + ImgDirectory + ".png")) {
		std::cout << "Error loading file!" << std::endl;
	}
	m_Sprite = new sf::Sprite();
	(*m_Sprite).setTexture(*m_Texture);
	for (unsigned int i = 0; i < FramesCount; i++) {
		m_Frames.emplace_back(sf::IntRect(X + i * FramesStep, Y, W, H));
		m_FlipedFrames.emplace_back(sf::IntRect(X + i * FramesStep + W, Y, -W, H));
	}
}

void Animation::Tick(const float& time) {
	if (!m_IsPlaying)
		return;
	m_CurrentFrame += m_Speed * time;
	if (m_CurrentFrame >= m_Frames.size())
		m_CurrentFrame = 0;
	if (m_Direction == left) {
		try {
			(*m_Sprite).setTextureRect(m_FlipedFrames.at(static_cast<int>(m_CurrentFrame)));
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << " 3\n";
		}

	}
	else {
		try {
			(*m_Sprite).setTextureRect(m_Frames.at(static_cast<int>(m_CurrentFrame)));
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << " 4\n";
		}
	}
}