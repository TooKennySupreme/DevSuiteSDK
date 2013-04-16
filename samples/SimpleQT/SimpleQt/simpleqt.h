#ifndef SIMPLEQT_H
#define SIMPLEQT_H

#include <QtGui/QMainWindow>
#include <QDockWidget>
#include <QGraphicsView>
#include "apbase.h"
#include "ui_simpleqt.h"
#include "CCameraThread.h"
#include "CDisplayThread.h"
#include "infodlg.h"

class SimpleQt : public QMainWindow
{
    Q_OBJECT

public:
    SimpleQt(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~SimpleQt();

protected:
    void            createActions();
    void            createMenus();
    void            createToolbars();
    void            closeEvent(QCloseEvent *event);

public slots:
    void            about();
    void            toggleToolbar();
    void            toggleCtrlbar();
    void            onFirstStartup();
    void            CreateInfoDlg();
    void            onPlay();
    void            onStop();
    void            showPixmap(QImage *image);
    void            cameraThreadFinshed();
    void            displayThreadFinshed();

private:
    Ui::SimpleQtClass   ui;
    QDockWidget *       m_dockInfo;
    InfoDlg *           m_dlgInfo;
    QGraphicsPixmapItem *m_viewPixmap;
    QGraphicsScene *    m_viewScene;
    QGraphicsView *     m_viewWidget;
    QMenu *             m_fileMenu;
    QMenu *             m_viewMenu;
    QMenu *             m_helpMenu;
    QAction *           m_playAct;
    QAction *           m_stopAct;
    QAction *           m_exitAct;
    QAction *           m_viewToolbarAct;
    QAction *           m_viewCtrlbarAct;
    QAction *           m_aboutAct;

    AP_HANDLE           m_hApBase;
    CTripleBuffer       m_TripleBuffer;
    CCameraThread *     m_CameraThread;
    CDisplayThread *    m_DisplayThread;
    bool                m_bPlaying;
    ap_u32              m_nWidth;
    ap_u32              m_nHeight;
};

#endif // SIMPLEQT_H
