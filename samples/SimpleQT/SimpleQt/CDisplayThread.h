#ifndef CDISPLAYTHREAD_H
#define CDISPLAYTHREAD_H

#include <QThread>
#include <QGraphicsPixmapItem>
#include "apbase.h"
#include "TripleBuffer.h"

class CDisplayThread : public QThread
{
    Q_OBJECT

public:
    CDisplayThread(AP_HANDLE            hApBase,
                   CTripleBuffer *      pTripleBuffer);
    ~CDisplayThread();

    unsigned char *         m_pBuffer;
    size_t                  m_nBufferSize;
    bool                    m_bPlaying;
    CTripleBuffer *         m_pTripleBuffer;
    AP_HANDLE               m_hApBase;

    void                run();
    void                stop();

signals:
    void                drawPix(QImage *image);
};

#endif // CDISPLAYTHREAD_H
