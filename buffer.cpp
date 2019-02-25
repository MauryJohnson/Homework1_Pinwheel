#include "buffer.h"
#include <GL/glew.h>
#include <iostream>


glBuffer::glBuffer(GLenum target)
{
    m_target = target;
    m_active = false;
    glGenBuffers(1, &m_buffer);
}

void glBuffer::setdata(const void *data, int numElements, size_t elementSize, GLenum mode)
{
    glBindBuffer(m_target, m_buffer);
    glBufferData(m_target, numElements * elementSize, data, mode);
    glBindBuffer(m_target, 0);
}

void glBuffer::bind(GLuint index, GLint valuePerVertex, GLenum variableType, GLsizei stride, int offset)
{
    m_active = true;
    m_index = index;
    glEnableVertexAttribArray(m_index);
    glBindBuffer(m_target, m_buffer);
    glVertexAttribPointer(
        m_index,
        valuePerVertex,     
        variableType,       
        GL_FALSE,           //normalized?
        stride,             
        (void*)offset       //buffer offset
    );
}

void glBuffer::unbind()
{   
    m_active = false;
    glBindBuffer(m_target, 0);
    glDisableVertexAttribArray(m_index);
}

GLuint glBuffer::getBufferID() const
{
    return m_buffer;
}


glBuffer::~glBuffer()
{
    if (!m_active){
        unbind();
    }
    glDeleteBuffers(1, &m_buffer);
}
