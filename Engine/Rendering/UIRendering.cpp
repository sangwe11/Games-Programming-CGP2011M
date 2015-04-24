#include "UIRendering.h"
#include "../EntitySystem/World.h"
#include "Shader.h"
#include "Display.h"
#include "UILabel.h"

namespace Engine
{
	UIFont::UIFont(FT_Face &face)
	{
		this->face = face;
	}

	bool UIFont::setGlyph(const char* character)
	{
		return (FT_Load_Char(face, *character, FT_LOAD_RENDER) == 0);
	}

	void UIRendering::initialise()
	{
		// Initialise Freetype
		if (FT_Init_FreeType(&ft))
			std::cout << "Could not init freetype library." << std::endl;

		// Load shader
		Shader &shader = manager->getSystem<Files>()->loadFile<Shader>("shaders/UI/text");

		// Create glyph vertex buffer
		glGenBuffers(1, &bufferGlyph);

		// Buffer data
		glBindBuffer(GL_ARRAY_BUFFER, bufferGlyph);
		glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

		// Generate vertex array object
		glGenVertexArrays(1, &vaObject);
		glBindVertexArray(vaObject);

		// Get attrib
		GLuint attrib;
		if (shader.getAttrib("vertex", attrib))
		{
			glEnableVertexAttribArray(attrib);
			glVertexAttribPointer(attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Unbind vaobject and buffer
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Add function to main
		addUpdateFunction(&UIRendering::uiRender, *this, 50);
	}

	void UIRendering::uninitialise()
	{
		glDeleteBuffers(1, &bufferGlyph);
		glDeleteVertexArrays(1, &vaObject);
	}

	void UIRendering::uiRender()
	{
		Shader &shader = manager->getSystem<Files>()->loadFile<Shader>("shaders/UI/text");

		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Disable depth test
		glDisable(GL_DEPTH_TEST);

		// Use the text rendering shader
		shader.use();

		// Set font texture location
		shader.setUniform("font", 0);

		// Get 1/2 screen size
		Display::Handle display = manager->getSystem<Display>();
		float sx = 2.0f / display->getWidth();
		float sy = 2.0f / display->getHeight();

		// Render UILabels
		for (UILabel::Handle &label : manager->getWorld().entities.getAllComponents<UILabel>(true))
		{
			// Set uniforms
			shader.setUniform("fontColor", label->getFontColor());

			// Render text
			renderText(label->getText().c_str(), label->getScreenPosition().x, label->getScreenPosition().y, sx, sy, label->getFont().c_str(), label->getFontSize());
		}

		// Disable blending
		glDisable(GL_BLEND);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
	}

	UIFont &UIRendering::loadFont(const std::string &font, const unsigned int &size)
	{
		// Use the loaded font
		if (fonts.find(font) != fonts.end())
		{
			// Set font size
			FT_Set_Pixel_Sizes(fonts[font]->get(), 0, size);

			return *fonts[font];
		}
		// Else load the font
		else
		{
			FT_Face face;

			// Load the font
			int error = FT_New_Face(ft, font.c_str(), 0, &face);

			// Assert the font loaded
			assert(error == 0);

			// Set font size
			FT_Set_Pixel_Sizes(face, 0, size);

			// Create UI font
			fonts[font] = std::unique_ptr<UIFont>(new UIFont(face));

			// Return
			return *fonts[font];
		}
	}

	void UIRendering::renderText(const char *text, float x, float y, float sx, float sy, std::string font, unsigned int size) {
		const char *p;

		GLuint glGlyph;

		// Load font
		UIFont &uiFont = loadFont(font, size);

		// Create glyph texture
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &glGlyph);
		glBindTexture(GL_TEXTURE_2D, glGlyph);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBindBuffer(GL_ARRAY_BUFFER, bufferGlyph);
		glBindVertexArray(vaObject);

		for (p = text; *p; p++)
		{
			// Set the current glyph
			if (!uiFont.setGlyph(p))
				continue;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, uiFont.get()->glyph->bitmap.width, uiFont.get()->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, uiFont.get()->glyph->bitmap.buffer);

			float x2 = x + uiFont.get()->glyph->bitmap_left * sx;
			float y2 = -y - uiFont.get()->glyph->bitmap_top * sy;
			float w = uiFont.get()->glyph->bitmap.width * sx;
			float h = uiFont.get()->glyph->bitmap.rows  * sy;

			GLfloat box[4][4] = {
				{ x2, -y2, 0, 0 },
				{ x2, -y2 - h, 0, 1 },
				{ x2 + w, -y2, 1, 0 },
				{ x2 + w, -y2 - h, 1, 1 },
			};

			// Buffer data
			glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(GL_FLOAT), box);

			// Draw
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			x += (uiFont.get()->glyph->advance.x >> 6) * sx;
			y += (uiFont.get()->glyph->advance.y >> 6) * sy;
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDeleteTextures(1, &glGlyph);
	}
}