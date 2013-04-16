#ifndef INFODLG_H
#define INFODLG_H

#include <QDialog>
#include "ui_infodlg.h"
#include "apbase.h"

class SimpleQt;

class InfoDlg : public QDialog
{
    Q_OBJECT

public:
    InfoDlg(SimpleQt* parent = 0);
    ~InfoDlg();

    void    setCamera(AP_HANDLE hApBase);

private slots:
    void    onGet();
    void    onSet();

private:
    Ui::InfoDlg ui;
    AP_HANDLE       m_hApBase;
    SimpleQt *      m_pMainWnd;
};

#endif // INFODLG_H
