#ifndef _METADB_SQLBUFFER_HPP_
#define _METADB_SQLBUFFER_HPP_

#include "MetaDb/CoreInclude.hpp"

#include <Metazion/Define.hpp>
#include <Metazion/Function.hpp>

template<typename T = char
, int STATICSIZE = 1024
, int ALIGNSIZE = 1024>
class SqlBuffer {
public:
    SqlBuffer() {}

    ~SqlBuffer() {
        if (m_dynamicBuffer != nullptr) {
            free(m_dynamicBuffer);
            m_dynamicBuffer = nullptr;
        }
    }

public:
    void Reset(int size) {
        const auto alignSize = NS_MZ::AlignLength(size, ALIGNSIZE);
        if (alignSize <= STATICSIZE) {
            m_buffer = m_staticBuffer;
        }
        else {
            if (alignSize > m_dynamicSize) {
                const auto alignLength = sizeof(T) * alignSize;
                void* buffer = nullptr;
                if (m_dynamicSize == 0) {
                    buffer = malloc(alignLength);
                }
                else {
                    buffer = realloc(m_dynamicBuffer, alignLength);
                }
                m_dynamicBuffer = (T*)buffer;
                m_dynamicSize = alignSize;
            }
            m_buffer = m_dynamicBuffer;
        }
        m_size = alignSize;
        memset(m_buffer, 0, m_size);
    }

    T* GetValue(int offset) {
        MZ_ASSERT_TRUE(offset >= 0 && offset < m_size);

        return &m_buffer[offset];
    }

    int GetSize() const {
        return m_size;
    }

private:
    T* m_buffer = nullptr;
    int m_size = 0;
    T m_staticBuffer[STATICSIZE] = {};
    T* m_dynamicBuffer = nullptr;
    int m_dynamicSize = 0;
};

#endif // _METADB_SQLBUFFER_HPP_
