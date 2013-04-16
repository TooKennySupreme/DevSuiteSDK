#ifndef CCAMERATHREAD_H
#define CCAMERATHREAD_H

#include <QThread>
#include "apbase.h"
#include "TripleBuffer.h"

class CCameraThread : public QThread
{
    Q_OBJECT

public:
    CCameraThread(AP_HANDLE        hApBase,
                  CTripleBuffer *  pTripleBuffer);
    ~CCameraThread();

    unsigned char *     m_pBuffer;
    size_t              m_nBufferSize;
    bool                m_bPlaying;
    CTripleBuffer *     m_pTripleBuffer;
    AP_HANDLE           m_hApBase;

    void                run();
    void                stop();

private:
    
};

#endif // CCAMERATHREAD_H
