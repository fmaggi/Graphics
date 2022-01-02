#ifndef VertexArray_H
#define VertexArray_H

typedef unsigned int uint32_t;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind();
private:
    uint32_t m_ID;
};

#endif
