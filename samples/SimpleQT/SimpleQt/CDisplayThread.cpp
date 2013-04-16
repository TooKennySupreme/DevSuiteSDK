#include "CDisplayThread.h"

CDisplayThread::CDisplayThread(AP_HANDLE            hApBase,
                               CTripleBuffer *      pTripleBuffer)
    : QThread(),
    m_pBuffer(NULL),
    m_nBufferSize(0),
    m_bPlaying(false),
    m_pTripleBuffer(pTripleBuffer),
    m_hApBase(hApBase)
{

}

CDisplayThread::~CDisplayThread()
{
    delete [] m_pBuffer;
}

void CDisplayThread::stop()
{
    m_bPlaying = false;
}

void CDisplayThread::run()
{
    if (m_pBuffer == NULL)
    {
        m_nBufferSize = ap_GrabFrame(m_hApBase, NULL, 0);
        m_pBuffer = new unsigned char [m_nBufferSize];
    }

    m_bPlaying = true;
    while (m_bPlaying)
    {
        //  Get the latest available sensor image data buffer
        if (!m_pTripleBuffer->Pull(&m_pBuffer))
        {
            //  There is no new image available so we release a little bit of processing time
            msleep(1);
            continue;
        }

        //  Convert the image data from the camera to RGB
        ap_u32  rgbWidth, rgbHeight, rgbBitDepth;
        unsigned char *rgb;
        rgb = ap_ColorPipe(m_hApBase, m_pBuffer,
                           m_nBufferSize, &rgbWidth, &rgbHeight, &rgbBitDepth);
        if (rgb == NULL)
        {
            msleep(30);
            continue;
        }

        //  Send the RGB data to the display
        QImage *image = new QImage(rgb, rgbWidth, rgbHeight, QImage::Format_RGB32);
        emit drawPix(image);

        //m_displayFrameCount++;
    }
}
