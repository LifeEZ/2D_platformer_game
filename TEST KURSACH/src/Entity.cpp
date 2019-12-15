#include "Entity.h"

Entity::Entity() = default;

Entity::~Entity() = default;

Entity::Entity(const float& X, const float& Y) {
	m_X = X;
	m_Y = Y;
}

void Entity::SetSpeed(const float& Speed) {
	m_Speed = Speed;
}

const float& Entity::GetSpeed() const {
	return m_Speed;
}

const float& Entity::GetPosX() const {
	return m_X;
}

const int& Entity::GetWidth() const {
	return m_W;
}

const int& Entity::GetHeight() const {
	return m_H;
}

void Entity::SetPosX(const float& X) {
	m_X = X;
}

const float& Entity::GetPosY() const {
	return m_Y;
}

void Entity::SetPosY(const float& Y) {
	m_Y = Y;
}

const int& Entity::GetHealth() const {
	return m_Health;
}

void Entity::SetHealth(const int& Health) {
	m_Health = Health;
}

const float& Entity::GetDeltaX() const {
	return m_Dx;
}

void Entity::SetDeltaX(const float& Dx) {
	m_Dx = Dx;
}

const float& Entity::GetDeltaY() const {
	return m_Dy;
}

void Entity::SetDeltaY(const float& Dy) {
	m_Dy = Dy;
}

const std::string& Entity::GetName() const {
	return m_Name;
}

void Entity::DamageBy(const int& Damage) {
	m_Health -= Damage;
}

void Entity::HealBy(const int& Heal) {
	m_Health += Heal;
}

void Entity::SetControl(const bool& Control) {
	m_Control = Control;
}

void Entity::UpdateHitTrigger() {
	m_HitTrigger.top = m_Y;
	m_Direction == left ? m_HitTrigger.left = m_X - m_HitTrigger.width : m_HitTrigger.left = m_X + static_cast<float>(m_W);
}

sf::FloatRect Entity::GetRect() const {
	return {m_X, m_Y, static_cast<float>(m_W), static_cast<float>(m_H)};
}

void Entity::Draw(sf::RenderWindow& Window) {
	m_Animation.Draw(Window, m_X, m_Y);
}

void Entity::SetState(const StateObject& State) {
	m_State = State;
}

const  bool& Entity::Alive() const {
	return m_Alive;
}

const Entity::StateObject& Entity::GetState() const {
	return m_State;
}
