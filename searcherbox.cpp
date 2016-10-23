#include "searcherbox.h"

SearcherBox::SearcherBox(QWidget *parent):QLineEdit(parent)
{
    kColor = QColor("#ea00ff");
    pColor = QColor("#ff0051");
    rColor = QColor("#0252ff");
    parameters = ColoUiParameters.keys();
}

void SearcherBox::filter(){

    listBox->clear();

    QString searchFor = this->text();

    font.setBold(true);
    for (qint32 i = 0; i < ColoUiDeclarations.size(); i++){

        QString text = ColoUiDeclarations.at(i);

        if (searchFor.size() >= minSearchSize){
            if (!text.contains(searchFor,Qt::CaseInsensitive)) continue;
        }

        QListWidgetItem *item = new QListWidgetItem();
        item->setFont(font);
        item->setTextColor(kColor);
        item->setBackgroundColor(Qt::black);
        item->setText(text);
        listBox->addItem(item);
    }

    font.setBold(false);
    for (qint32 i = 0; i < ColoUiProperties.size(); i++){
        QString text = ColoUiProperties.at(i);

        if (searchFor.size() >= minSearchSize){
            if (!text.contains(searchFor,Qt::CaseInsensitive)) continue;
        }

        QListWidgetItem *item = new QListWidgetItem();
        item->setFont(font);
        item->setTextColor(pColor);
        item->setBackgroundColor(Qt::black);
        item->setText(text);
        listBox->addItem(item);
    }




    for (qint32 i = 0; i < parameters.size(); i++){
        QString text = parameters.at(i);

        if (searchFor.size() >= minSearchSize){
            if (!text.contains(searchFor,Qt::CaseInsensitive)) continue;
        }

        QListWidgetItem *item = new QListWidgetItem();
        item->setFont(font);
        item->setTextColor(rColor);
        item->setBackgroundColor(Qt::black);
        item->setText(text);
        listBox->addItem(item);
    }

}

void SearcherBox::keyPressEvent(QKeyEvent *e){
    QLineEdit::keyPressEvent(e);
    filter();
}
