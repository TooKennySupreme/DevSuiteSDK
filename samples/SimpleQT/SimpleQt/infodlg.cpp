#include <stdio.h>
#include "simpleqt.h"
#include "infodlg.h"


InfoDlg::InfoDlg(SimpleQt *parent)
    : QDialog(parent),
    m_hApBase(NULL)
{
    ui.setupUi(this);
    m_pMainWnd = parent;

    connect(ui.pushButton_Get, SIGNAL(clicked()), this, SLOT(onGet()));
    connect(ui.pushButton_Set, SIGNAL(clicked()), this, SLOT(onSet()));
}

InfoDlg::~InfoDlg()
{

}

void InfoDlg::setCamera(AP_HANDLE hApBase)
{
    m_hApBase = hApBase;

    //  Fill in the combo-box with the register names
    ui.comboBox_Register->clear();
    int n = ap_NumRegisters(m_hApBase, NULL);
    for (int i = 0; i < n; ++i)
    {
        AP_HANDLE hReg = ap_Register(m_hApBase, NULL, i);
        ui.comboBox_Register->addItem(QString(ap_RegisterSymbol(hReg)));
        ap_DestroyRegister(hReg);
    }
    ui.comboBox_Register->setCurrentIndex(0);
    onGet();
}

void InfoDlg::onGet()
{
    ap_u32      nValue = 0;
    ap_GetSensorRegister(m_hApBase, ui.comboBox_Register->currentText().toAscii(), NULL, &nValue, false);

    ui.lineEdit_Value->setText(QString("0x%1").arg(uint(nValue), 4, 16, QLatin1Char('0')));
}

void InfoDlg::onSet()
{
    ap_u32      nSideEffects = 0;
    ap_u32      nOldValue = 0;
    ap_u32      nNewValue = 0;

    sscanf(ui.lineEdit_Value->text().toAscii(), "%i", &nNewValue);

    nSideEffects = ap_TestSetSensorRegister(m_hApBase, ui.comboBox_Register->
                                            currentText().toAscii(),
                                            nNewValue, 0, &nOldValue);
    if (nSideEffects & (AP_FLAG_REALLOC))
        m_pMainWnd->onStop(); // stop streaming if register write will change the image

    ap_SetSensorRegister(m_hApBase, ui.comboBox_Register->currentText().toAscii(),
                         NULL, nNewValue, NULL);
    m_pMainWnd->onPlay(); // resume streaming if it was stopped
}
