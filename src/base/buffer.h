#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <stdint.h>

#include "log/log.h"

// General purpose buffer. Fairly dangerous to work with but fun :)
class Buffer
{
public:
    Buffer() = default;

    Buffer(size_t size)
        : m_size(size)
    {
        m_size = size;
        m_data = new uint8_t[m_size];
        Zero();
    }

    ~Buffer() { delete[] m_data; }

    // Not the best to pass float for example by const &, but I dont know how to change it
    template <typename T>
    void Write(const T& data, size_t offset)
    {
        T* b_data = reinterpret_cast<T*>(m_data + offset);
        *b_data = data;
    }

    template <typename T>
    void Write(const T* data, uint32_t count, size_t offset)
    {
        size_t bytes = sizeof(T) * count;
        const uint8_t* b_data = reinterpret_cast<const uint8_t*>(data);
        memcpy(m_data + offset, b_data, bytes);
    }

    template <typename T>
    T& ReadAs(size_t at)
    {
        T *data = reinterpret_cast<T*>(m_data + at);
        return *data;
    }

    uint8_t* ReadBytes(size_t at)
    {
        return m_data + at;
    }

    void Zero()
    {
        memset(m_data, 0, m_size);
    }

    size_t Size() { return m_size; }

    void Resize(size_t size)
    {
        delete[] m_data;
        m_size = size;
        m_data = new uint8_t[m_size];
    }

private:
    size_t m_size = 0;
    uint8_t *m_data = nullptr;
};

#endif
