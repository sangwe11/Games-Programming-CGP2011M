#include "UILabel.h"

namespace Engine
{
	UILabel::UILabel(const std::string &text, const std::string &font, const unsigned int &fontSize, const glm::vec4 &fontColor, const glm::vec2 &screenPosition)
	{
		this->text = text;
		this->font = font;
		this->fontSize = fontSize;
		this->fontColor = fontColor;
		this->screenPosition = screenPosition;
	}
}