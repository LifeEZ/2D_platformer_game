#pragma once
#include <SFML/Graphics.hpp>

class FocusObject {
public:
	virtual ~FocusObject() = default;
	virtual void Event(const sf::Event&) = 0;
};

class TextBox : public sf::Drawable, public FocusObject {
private:
	mutable sf::Text m_Text;
	mutable std::string m_NewText;
	mutable bool m_TextChanged = false;
public:
	explicit TextBox(sf::Text Text);
	void draw(sf::RenderTarget& Render, sf::RenderStates States) const override;
	void Event(const sf::Event& event) override;
	void SetText(const sf::String& String) const;
	std::string GetText() const;
};

class FocusController {
private:
	FocusObject* m_Object;
public:
	explicit FocusController(FocusObject* Obj = nullptr);
	void SetFocusObject(FocusObject* NewObj);
	FocusObject* GetFocusObject() const;
};
