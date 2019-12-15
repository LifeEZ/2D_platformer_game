#include "AnimationManager.h"

AnimationManager::AnimationManager() = default;

AnimationManager::~AnimationManager() = default;

std::string AnimationManager::GetName() const {
	return m_CurrentAnimation;
}

int AnimationManager::GetCurrentFrame(const std::string& name) {
	return m_AnimationList.at(name).GetCurrentFrame();
}

int AnimationManager::GetFramesAmount(const std::string& name) {
	return m_AnimationList.at(name).GetFramesAmount();
}

void AnimationManager::CreateAnimation(const std::string& Name, const std::string& ImgDirectory, const int& X, const int& Y, const int& W, const int& H, 
	const int& FramesCount, const int& FramesStep, const float& PlaySpeed) {
	m_AnimationList.insert(std::pair<std::string, Animation>(Name, Animation(ImgDirectory, X, Y, W, H, FramesCount, FramesStep, PlaySpeed)));
	m_CurrentAnimation = Name;
}

void AnimationManager::Draw(sf::RenderWindow& Window, const float& X, const float& Y) {
	m_AnimationList.at(m_CurrentAnimation).GetSprite()->setPosition(X - GetOffsetX(m_CurrentAnimation),Y - GetOffsetY(m_CurrentAnimation));
	Window.draw((*m_AnimationList[m_CurrentAnimation].GetSprite()));
}

void AnimationManager::Set(const std::string& Name) {
	m_CurrentAnimation = Name;
}

Animation::AnimDirection AnimationManager::GetDirection() const {
	return m_AnimationList.at(m_CurrentAnimation).GetDirection();
}

void AnimationManager::SetDirection(const Animation::AnimDirection& Direction) {
	m_AnimationList.at(m_CurrentAnimation).SetDirection(Direction);
}

void AnimationManager::SetFrame(const std::string& name, const int& Frame) {
	m_AnimationList.at(name).SetCurrentFrame(0);
}

float AnimationManager::GetOffsetX(const std::string& name) {
	return m_AnimationList.at(name).GetOffsetX();
}

float AnimationManager::GetOffsetY(const std::string& name) {
	return m_AnimationList.at(name).GetOffsetY();
}

void AnimationManager::SetOffsetX(const std::string& name, const float& LeftOffset, const float& RightOffset) {
	m_AnimationList.at(name).SetOffsetX(LeftOffset, RightOffset);
}

void AnimationManager::SetOffsetY(const std::string& name, const float& Offset) {
	m_AnimationList.at(name).SetOffsetY(Offset);
}

void AnimationManager::UpdateOffsetDir(const std::string& name) {
	m_AnimationList.at(name).UpdateOffsetDir();
}

void AnimationManager::Rotate() {
	m_AnimationList.at(m_CurrentAnimation).Rotate();
}

void AnimationManager::Update(const float& Time, const Animation::AnimDirection& Direction) {
	m_AnimationList.at(m_CurrentAnimation).SetDirection(Direction);
	m_AnimationList.at(m_CurrentAnimation).UpdateOffsetDir();
	m_AnimationList.at(m_CurrentAnimation).Tick(Time);
}

void AnimationManager::Pause() {
	m_AnimationList.at(m_CurrentAnimation).Pause();
}

void AnimationManager::Play() {
	m_AnimationList.at(m_CurrentAnimation).Play();
}

void AnimationManager::Play(const std::string& Name) { 
	m_AnimationList.at(Name).Play();
}