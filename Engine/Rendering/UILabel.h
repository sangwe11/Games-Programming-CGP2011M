#ifndef _UILABEL_H
#define _UILABEL_H

#include "../Core/Maths.h"
#include "../EntitySystem/Entity.h"

namespace Engine
{
	class UILabel : public EntitySystem::Component<UILabel>
	{
	public:
		UILabel(const std::string &text, const std::string &font, const unsigned int &fontSize, const glm::vec4 &fontColor, const glm::vec2 &screenPosition);

		const std::string &getText() { return text; }
		const std::string &getFont() { return font; }
		const unsigned int &getFontSize() { return fontSize; }
		const glm::vec4 &getFontColor() { return fontColor; }
		const glm::vec2 &getScreenPosition() { return screenPosition; }

		void setText(const std::string &text) { this->text = text; }
		void setFontSize(const unsigned int &fontSize) { this->fontSize = fontSize; }
		void setFontColor(const glm::vec4 &fontColor) { this->fontColor = fontColor; }
		void setScreenPosition(const glm::vec2 &screenPosition) { this->screenPosition = screenPosition; }

	private:
		std::string text;
		std::string font;
		unsigned int fontSize;
		glm::vec4 fontColor;
		glm::vec2 screenPosition;
	};
}

#endif