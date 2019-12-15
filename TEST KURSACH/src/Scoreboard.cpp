#include "Scoreboard.h"
#include <utility>

TextBox::TextBox(sf::Text Text) : m_Text(std::move(Text)) {
	m_NewText = m_Text.getString();
}

void TextBox::draw(sf::RenderTarget& Render, sf::RenderStates States) const {
	if (m_TextChanged) {
		const_cast<TextBox*>(this)->SetText(m_NewText);
		m_TextChanged = false;
	}
	Render.draw(m_Text, States);
}

void TextBox::Event(const sf::Event& event) {
	if (event.type == sf::Event::TextEntered) {
		//Text handling
		m_TextChanged = true;
		if (m_Text.getString().getSize() < 8 || event.text.unicode == 0x8) {
			switch (event.text.unicode) {
			case 0x8: //Backspace
				if (!m_NewText.empty()) m_NewText.erase(m_NewText.size() - 1);
				break;
			default:
				if (event.text.unicode >= L'a' && event.text.unicode <= L'z' || event.text.unicode >= L'A' &&
					event.text.unicode <= L'Z' || event.text.unicode >= L'0' && event.text.unicode <= L'9' ||
					event.text.unicode == L' ') {
					m_NewText += event.text.unicode;
				}
			}
		}
	}
}

void TextBox::SetText(const sf::String& String) const {
	m_Text.setString(String);
}

std::string TextBox::GetText() const {
	return m_Text.getString();
}

FocusController::FocusController(FocusObject* Obj) : m_Object(Obj) {
}

void FocusController::SetFocusObject(FocusObject* NewObj) {
	if (m_Object == NewObj)
		return;
	m_Object = NewObj;
}

FocusObject* FocusController::GetFocusObject() const {
	return m_Object;
}
