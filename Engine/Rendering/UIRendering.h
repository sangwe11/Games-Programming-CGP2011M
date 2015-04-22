#ifndef _UIRENDERING_H
#define _UIRENDERING_H

#include <GLEW/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../EntitySystem/System.h"

namespace Engine
{
	class UIRendering : public EntitySystem::System<UIRendering>
	{
	public:
		virtual void initialise();
		virtual void uninitialise();

		void uiRender();

	private:
		void renderText(const char *text, float x, float y, float sx, float sy, const char *font, unsigned int size);

		// Freetype library
		FT_Library ft;

		// vertex buffer and vaObject for drawing glyphs
		GLuint bufferGlyph;
		GLuint vaObject;
	};
}

#endif