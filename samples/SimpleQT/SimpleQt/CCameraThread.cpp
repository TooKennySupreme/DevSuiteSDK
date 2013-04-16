#include "CCameraThread.h"

CCameraThread::CCameraThread(AP_HANDLE        hApBase,
                             CTripleBuffer *  pTripleBuffer)
    : QThread(),
    m_pBuffer(NULL),
    m_nBufferSize(0),
    m_bPlaying(false),
    m_pTripleBuffer(pTripleBuffer),
    m_hApBase(hApBase)
{

}

CCameraThread::~CCameraThread()
{
    delete [] m_pBuffer;
}

void CCameraThread::stop()
{
    m_bPlaying = false;
}

void CCameraThread::run()
{
    if (m_pBuffer == NULL)
    {
        m_nBufferSize = ap_GrabFrame(m_hApBase, NULL, 0);
        m_pBuffer = new unsigned char [m_nBufferSize];
    }

    m_bPlaying = true;
    while (m_bPlaying)
    {
        //  Grab a frame (ignore bad frames)
        ap_u32 bytesReturned;
        bytesReturned = ap_GrabFrame(m_hApBase, m_pBuffer, m_nBufferSize);
        ap_s32 retVal = 0;//ap_GetLastError();
        if (retVal == AP_CAMERA_SUCCESS)
        {
            //  Store the new camera image into the triple buffer for processing
            m_pTripleBuffer->Push(&m_pBuffer);
            //m_cameraFrameCount++;
        }
    }
}
