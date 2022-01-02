#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <stdint.h>

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

    template<typename T>
    void Write(T& data, size_t offset)
    {
        size_t bytes = sizeof(T);
        uint8_t* b_data = reinterpret_cast<uint8_t*>(&data);
        memcpy(m_data + offset, b_data, bytes);
    }

    template<typename T>
    void Write(T* data, uint32_t count, size_t offset)
    {
        size_t bytes = sizeof(T) * count;
        uint8_t* b_data = reinterpret_cast<uint8_t*>(data);
        memcpy(m_data + offset, b_data, bytes);
    }

    template<typename T>
    T& Read(size_t at)
    {
        T* data = reinterpret_cast<T*>(m_data + at);
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
    uint8_t* m_data = nullptr;
};

#endif
