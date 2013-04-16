#pragma once
#include <QMutex>

//  Allows an image producer and an image consumer to swap
//  buffers efficiently
class CTripleBuffer
{
public:
    CTripleBuffer(void);
    ~CTripleBuffer(void);
protected:
    QMutex              m_lock;
    unsigned char *     m_pTripleBuffer;
    bool                m_bTripleBufferDirty;
public:
    void Push(unsigned char **pBuf);
    bool Pull(unsigned char **pBuf);
    bool IsEmpty();
    unsigned char * Alloc(size_t nBufferSize);
    void Free();
};
