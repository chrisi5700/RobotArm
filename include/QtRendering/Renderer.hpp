//
// Created by chris on 12/17/25.
//

#ifndef OPENGL_TEST_RENDERER_HPP
#define OPENGL_TEST_RENDERER_HPP
#include <glad/glad.h>
#include "Drawable.hpp"
#include <vector>
#include <memory>


class Renderer
{

public:
	Renderer();
	void draw(float time);
	void update_aspect_ratio(int width, int height);
	~Renderer();
private:
	std::vector<std::unique_ptr<Drawable>> m_drawables;
	GLint m_program;
	GLint m_view_loc;
	GLint m_proj_loc;
	GLint m_light_pos_loc;
	GLint m_view_pos_loc;
	float m_aspect_ratio = 16.0f / 9.0f;
};


#endif // OPENGL_TEST_RENDERER_HPP
