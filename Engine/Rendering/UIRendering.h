#ifndef _UIRENDERING_H
#define _UIRENDERING_H

#include <GLEW/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../EntitySystem/System.h"

namespace Engine
{
	class UIFont
	{
	public:
		UIFont(FT_Face &face);

		FT_Face &get() { return face; }
		bool setGlyph(const char* character);

	private:
		FT_Face face;
	};

	class UIRendering : public EntitySystem::System<UIRendering>
	{
	public:
		virtual void initialise();
		virtual void uninitialise();

		void uiRender();

		UIFont &loadFont(const std::string &font, const unsigned int &size);

	private:
		void renderText(const char *text, float x, float y, float sx, float sy, std::string font, unsigned int size);

		// Freetype library
		FT_Library ft;

		// vertex buffer and vaObject for drawing glyphs
		GLuint bufferGlyph;
		GLuint vaObject;

		// Loaded fonts
		std::map<std::string, std::unique_ptr<UIFont>> fonts;
	};
}

#endif