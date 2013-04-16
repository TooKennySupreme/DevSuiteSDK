#include "simpleqt.h"
#include "multiplechoice.h"
#include <QMessageBox>


//
//  ApBase calls back to the application for some things, mainly user
//  interaction with ini scripts and Python scripts.
//

//   This handles the PROMPT= ini command, a multiple-choice question
int APBASE_DECL MultipleChoice_Callback(void *pContext,
    const char *szMessage,
    const char *szChoices)
{
    SimpleQt *pSimpleQt = (SimpleQt *)pContext;

    MultipleChoice dlg(pSimpleQt);
    dlg.SetPrompt(szMessage);
    const char *p = szChoices;
    while (p[0] && dlg.count() < 15)
    {
        dlg.AddChoice(p);
        p += strlen(p) + 1;
    }
    dlg.HideButtons();
    if (dlg.exec() == QDialog::Accepted)
        return dlg.choice();
    return -1;
}

//   This handles error messages
int APBASE_DECL ErrorMessage_Callback(void *pContext,
    const char *szMessage,
    unsigned int mbType)
{
    SimpleQt *pSimpleQt = (SimpleQt *)pContext;

    //  Translate ApBase enum to Qt enums
    QMessageBox::StandardButtons qMbType = QMessageBox::Ok;
    QMessageBox::Icon            qMbIcon = QMessageBox::Information;
    switch (mbType & 0xF)
    {
    case AP_MSGTYPE_OK:
        qMbIcon = QMessageBox::Information;
        qMbType = QMessageBox::Ok;
        break;
    case AP_MSGTYPE_OKCANCEL:
        qMbIcon = QMessageBox::Warning;
        qMbType = QMessageBox::Ok | QMessageBox::Cancel;
        break;
    case AP_MSGTYPE_ABORTRETRYIGNORE:
        qMbIcon = QMessageBox::Warning;
        qMbType = QMessageBox::Abort | QMessageBox::Retry | QMessageBox::Ignore;
        break;
    case AP_MSGTYPE_YESNOCANCEL:
        qMbIcon = QMessageBox::Question;
        qMbType = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
        break;
    case AP_MSGTYPE_YESNO:
        qMbIcon = QMessageBox::Question;
        qMbType = QMessageBox::Yes | QMessageBox::No;
        break;
    case AP_MSGTYPE_RETRYCANCEL:
        qMbIcon = QMessageBox::Warning;
        qMbType = QMessageBox::Retry | QMessageBox::Cancel;
        break;
    case AP_MSGTYPE_CANCELTRYCONTINUE:
        qMbIcon = QMessageBox::Warning;
        qMbType = QMessageBox::Cancel | QMessageBox::Retry | QMessageBox::Ignore;
        break;
    }

    //  Show message
    QMessageBox mb(qMbIcon, "SimpleQt", szMessage, qMbType, pSimpleQt);
    int retVal = mb.exec();

    //  Translate Qt enum to ApBase enum
    switch (retVal)
    {
    default:
    case     QMessageBox::Ok      : retVal = AP_MSG_OK       ; break;
    case     QMessageBox::Cancel  : retVal = AP_MSG_CANCEL   ; break;
    case     QMessageBox::Abort   : retVal = AP_MSG_ABORT    ; break;
    case     QMessageBox::Retry   : retVal = AP_MSG_RETRY    ; break;
    case     QMessageBox::Ignore  : retVal = AP_MSG_IGNORE   ; break;
    case     QMessageBox::Yes     : retVal = AP_MSG_YES      ; break;
    case     QMessageBox::No      : retVal = AP_MSG_NO       ; break;
    case     QMessageBox::Close   : retVal = AP_MSG_CLOSE    ; break;
    case     QMessageBox::Help    : retVal = AP_MSG_HELP     ; break;
    }

    return retVal;
}
