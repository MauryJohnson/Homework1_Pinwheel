#pragma once
#include <GL/glew.h>

class glBuffer
{
public:

    //EXAMPLE
    /*
	glBuffer vbo(GL_ARRAY_BUFFER);
	vbo.setdata(color_buffer_data, GL_STATIC_DRAW);
	vbo.bind(0, 3);
	
	-----------------THIS REPLACES--------------------
	
	GLuint vbo;
	
	glGenBuffers(1, &vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
	);
----------------------------------------------------------------
    */

    glBuffer(GLenum target);
    //void setdata(const void *data, GLenum mode);
    void setdata(const void *data, int numElements, size_t elementSize, GLenum mode);
    void bind(GLuint index, GLint valuePerVertex, GLenum variableType = GL_FLOAT, GLsizei stride = 0, int offset = 0);
    void unbind();
    GLuint getBufferID() const;
    ~glBuffer();

private:
    bool m_active;
    GLuint m_buffer;
    GLuint m_index;
    GLenum m_target;
};
