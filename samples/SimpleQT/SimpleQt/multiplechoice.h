#ifndef MULTIPLECHOICE_H
#define MULTIPLECHOICE_H

#include <QDialog>
#include <QRadioButton>
#include "ui_multiplechoice.h"

class MultipleChoice : public QDialog
{
    Q_OBJECT

public:
    MultipleChoice(QWidget *parent = 0);
    ~MultipleChoice();

    void        SetPrompt(const char *szPrompt);
    void        AddChoice(const char *szChoice);
    void        HideButtons();
    int         count();
    int         choice();

private slots:
    void        onSkip();
private:
    Ui::MultipleChoice ui;
    QList<QRadioButton *>   m_radioButtons;
    bool                    m_skip;
};

#endif // MULTIPLECHOICE_H
