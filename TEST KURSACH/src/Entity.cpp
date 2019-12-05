#include "Entity.h"

Entity::Entity() {
}

Entity::~Entity() {
}

Entity::Entity(const float& X, const float& Y) {
	m_X = X;
	m_Y = Y;
}

void Entity::SetSpeed(const float& Speed) {
	m_Speed = Speed;
}

const float& Entity::GetSpeed() {
	return m_Speed;
}

const float& Entity::GetPosX() {
	return m_X;
}

const int& Entity::GetWidth() {
	return m_W;
}

const int& Entity::GetHeight() {
	return m_H;
}

void Entity::SetPosX(const float& X) {
	m_X = X;
}

const float& Entity::GetPosY() {
	return m_Y;
}

void Entity::SetPosY(const float& Y) {
	m_Y = Y;
}

const int& Entity::GetHealth() {
	return m_Health;
}

void Entity::SetHealth(const int& Health) {
	m_Health = Health;
}

const float& Entity::GetDeltaX() {
	return m_dx;
}

void Entity::SetDeltaX(const float& dx) {
	m_dx = dx;
}

const float& Entity::GetDeltaY() {
	return m_dy;
}

void Entity::SetDeltaY(const float& dy) {
	m_dy = dy;
}

void Entity::DamageBy(const int& Damage) {
	m_Health -= Damage;
}

void Entity::HealBy(const int& Heal) {
	m_Health += Heal;
}

void Entity::UpdateHitTrigger() {
	m_HitTrigger.top = m_Y;
	m_Direction == left ? m_HitTrigger.left = m_X - m_HitTrigger.width : m_HitTrigger.left = m_X + m_W;
}

sf::FloatRect Entity::GetRect() {
	return sf::FloatRect(m_X, m_Y, static_cast<float>(m_W), static_cast<float>(m_H));
}

void Entity::Draw(sf::RenderWindow& Window) {
	m_Animation.Draw(Window, m_X, m_Y);
}

void Entity::SetState(const StateObject& State) {
	m_State = State;
}

const Entity::StateObject& Entity::GetState() {
	return m_State;
}
