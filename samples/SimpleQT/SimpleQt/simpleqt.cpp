#include "simpleqt.h"
#include "callbacks.h"
#include <QMessageBox>


SimpleQt::SimpleQt(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags),
    m_dockInfo(NULL),
    m_dlgInfo(NULL),
    m_viewPixmap(NULL),
    m_viewScene(NULL),
    m_viewWidget(NULL),
    m_hApBase(NULL),
    m_CameraThread(NULL),
    m_DisplayThread(NULL),
    m_bPlaying(false),
    m_nWidth(0),
    m_nHeight(0)
{
    ui.setupUi(this);
    createActions();
    createMenus();
    createToolbars();

    m_viewScene = new QGraphicsScene;
    m_viewWidget = new QGraphicsView(m_viewScene, this);
    //m_viewWidget->setViewport(new QGLWidget);
    setCentralWidget(m_viewWidget);
}

SimpleQt::~SimpleQt()
{
}

void SimpleQt::createActions()
{
    m_playAct = new QAction(QIcon(":/SimpleQt/Resources/media_play.ico"), tr("&Play"), this);
    m_playAct->setStatusTip(tr("Turn on image streaming"));
    m_playAct->setCheckable(true);
    connect(m_playAct, SIGNAL(triggered()), this, SLOT(onPlay()));

    m_stopAct = new QAction(QIcon(":/SimpleQt/Resources/media_stop.ico"), tr("&Stop"), this);
    m_stopAct->setStatusTip(tr("Turn off image streaming"));
    m_stopAct->setCheckable(true);
    connect(m_stopAct, SIGNAL(triggered()), this, SLOT(onStop()));

    m_exitAct = new QAction(QIcon(":/SimpleQt/Resources/exit.ico"), tr("E&xit"), this);
    m_exitAct->setShortcuts(QKeySequence::Quit);
    m_exitAct->setStatusTip(tr("Exit the application"));
    connect(m_exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    m_viewToolbarAct = new QAction(tr("Toolbar"), this);
    m_viewToolbarAct->setStatusTip(tr("Show or hide the toolbar"));
    m_viewToolbarAct->setCheckable(true);
    m_viewToolbarAct->setChecked(true);
    connect(m_viewToolbarAct, SIGNAL(triggered()), this, SLOT(toggleToolbar()));

    m_viewCtrlbarAct = new QAction(QIcon(":/SimpleQt/Resources/gear.ico"), tr("Control Bar"), this);
    m_viewCtrlbarAct->setStatusTip(tr("Show or hide the register control bar"));
    m_viewCtrlbarAct->setCheckable(true);
    m_viewCtrlbarAct->setChecked(true);
    connect(m_viewCtrlbarAct, SIGNAL(triggered()), this, SLOT(toggleCtrlbar()));

    m_aboutAct = new QAction(QIcon(":/SimpleQt/Resources/about.ico"), tr("&About"), this);
    m_aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void SimpleQt::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_exitAct);

    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_playAct);
    m_viewMenu->addAction(m_stopAct);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_viewToolbarAct);
    m_viewMenu->addAction(m_viewCtrlbarAct);

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAct);
}

void SimpleQt::createToolbars()
{
    ui.mainToolBar->addAction(m_playAct);
    ui.mainToolBar->addAction(m_stopAct);
    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(m_viewCtrlbarAct);
    ui.mainToolBar->addAction(m_aboutAct);
}

void SimpleQt::closeEvent(QCloseEvent *event)
{
    onStop();
    ap_Destroy(m_hApBase);
    ap_Finalize();
} 

void SimpleQt::about()
{
    QMessageBox::about(this, tr("About SimpleQt"),
                tr("The SimpleQt sample demonstrates a simple GUI "
                "application based on the Aptina <b>ApBase API</b>."));
}

void SimpleQt::toggleToolbar()
{
    if (ui.mainToolBar->isVisible())
        ui.mainToolBar->hide();
    else
        ui.mainToolBar->show();
    m_viewToolbarAct->setChecked(ui.mainToolBar->isVisible());
}

void SimpleQt::toggleCtrlbar()
{
    if (m_dockInfo->isVisible())
        m_dockInfo->hide();
    else
        m_dockInfo->show();
    m_viewCtrlbarAct->setChecked(m_dockInfo->isVisible());
}

void SimpleQt::CreateInfoDlg()
{
    m_dlgInfo = new InfoDlg(this);
    m_dlgInfo->setCamera(m_hApBase);
    m_dockInfo = new QDockWidget(tr("Control Bar"), this);
    m_dockInfo->setAllowedAreas(Qt::TopDockWidgetArea);
    m_dockInfo->setWidget(m_dlgInfo);
    addDockWidget(Qt::TopDockWidgetArea, m_dockInfo);
}

void SimpleQt::onFirstStartup()
{
    int retVal = ap_DeviceProbe(NULL);
    if (retVal == AP_CAMERA_SUCCESS)
    {
        m_hApBase = ap_Create(0);
        if (m_hApBase == NULL)
        {
             QMessageBox msgBox;
             msgBox.setText("Camera initialization error! SimpleQT will close.");
             msgBox.setIcon(QMessageBox::Critical);
             msgBox.exec();
             this->close();
             return;
        }
        ap_SetCallback_MultipleChoice(m_hApBase, MultipleChoice_Callback, this);
        ap_SetCallback_ErrorMessage(m_hApBase, ErrorMessage_Callback, this);
        ap_LoadIniPreset(m_hApBase, NULL, NULL);
        CreateInfoDlg();
        onPlay();
        return;
    } else
    {
         QMessageBox msgBox;
         msgBox.setText("Unable to either detect a sensor or find a matching SDAT file.\nSimpleQT will close.");
         msgBox.setIcon(QMessageBox::Critical);
         msgBox.exec();
         this->close();
    }
}

void SimpleQt::onPlay()
{
    if (m_hApBase == NULL)
        return;
    if (m_bPlaying)
        return;

    ap_GetImageFormat(m_hApBase, &m_nWidth, &m_nHeight, NULL, 0);
    if (m_viewPixmap == NULL)
        m_viewPixmap = m_viewScene->addPixmap(QPixmap(m_nWidth, m_nHeight));
    m_TripleBuffer.Alloc(ap_GrabFrame(m_hApBase, NULL, 0));
    m_CameraThread = new CCameraThread(m_hApBase, &m_TripleBuffer);
    m_DisplayThread = new CDisplayThread(m_hApBase, &m_TripleBuffer);
    connect(m_CameraThread, SIGNAL(finished()), this, SLOT(cameraThreadFinshed()));
    connect(m_DisplayThread, SIGNAL(finished()), this, SLOT(displayThreadFinshed()));
    connect(m_DisplayThread, SIGNAL(drawPix(QImage *)), this, SLOT(showPixmap(QImage *)),
            Qt::BlockingQueuedConnection);
    m_CameraThread->start();
    m_DisplayThread->start();
    m_bPlaying = true;
    m_playAct->setChecked(m_bPlaying);
    m_stopAct->setChecked(!m_bPlaying);
}

void SimpleQt::onStop()
{
    if (m_hApBase == NULL)
        return;
    if (!m_bPlaying)
        return;

    m_CameraThread->stop();
    m_DisplayThread->stop();
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    m_CameraThread->wait();
    m_DisplayThread->wait();
    delete m_CameraThread;
    delete m_DisplayThread;
}

void SimpleQt::cameraThreadFinshed()
{
}

void SimpleQt::displayThreadFinshed()
{
    m_bPlaying = false;
    m_playAct->setChecked(m_bPlaying);
    m_stopAct->setChecked(!m_bPlaying);
}

void SimpleQt::showPixmap(QImage *image)
{
    m_viewPixmap->setPixmap(QPixmap::fromImage(*image));
    delete image;
}
