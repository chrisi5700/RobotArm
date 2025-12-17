//
// Created by chris on 12/17/25.
//

#ifndef OPENGL_TEST_DRAWABLE_HPP
#define OPENGL_TEST_DRAWABLE_HPP

class Drawable
{
public:
	virtual void draw(float time) = 0;
	virtual ~Drawable() = default;
};

#endif // OPENGL_TEST_DRAWABLE_HPP
