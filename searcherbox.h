#ifndef SEARCHERBOX_H
#define SEARCHERBOX_H

#include <QLineEdit>
#include <QListWidget>
#include "coloUi/colouidefines.h"

class SearcherBox : public QLineEdit
{
public:
    SearcherBox(QWidget *parent = nullptr);
    void setFont(QFont f){font = f;}
    void setKeywordBox(QListWidget *lw){listBox = lw;}
    void filter();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    QListWidget *listBox;
    QFont font;
    QColor pColor;
    QColor kColor;
    QColor rColor;
    QStringList parameters;
    const qint32 minSearchSize = 2;


};

#endif // SEARCHERBOX_H
