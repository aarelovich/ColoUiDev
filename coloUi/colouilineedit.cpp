#include "colouilineedit.h"

ColoUiLineEdit::ColoUiLineEdit(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    fm = nullptr;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    acceptedInput = QRegExp("[\\w\\+\\-\\.~!@#$%=_\\^&\\*\\(\\);:\\\\\\/\\|\\<\\>\\\"\\'?,\\{\\}\\[\\]]");
    this->type = CUI_LINE_EDIT;
    editingEnabled = false;

}

void ColoUiLineEdit::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    if (fm != nullptr){
        delete fm;
    }
    fm = new QFontMetricsF(config.getFont());
    yoffset = this->h - fm->height();

    effectiveWidth = 0.95*(this->w - config.getUInt16(CPR_X_OFFSET));
    colCursor = 0;
    editingEnabled = false;

    setText(config.getString(CPR_TEXT));

    if (!config.getBool(CPR_USE_HTML)){
        if (!config.getBool(CPR_READ_ONLY)){
            this->setCursor(QCursor(Qt::IBeamCursor));
        }
        else{
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    else{
        this->setCursor(QCursor(Qt::ArrowCursor));
    }

}


void ColoUiLineEdit::setText(QString text){

    colStart = 0;
    colEnd = 1;


    if (text.isEmpty()){
        config.set(CPR_TEXT,"");
        colStart = 0;
        colEnd = 1;
        colCursor = 0;
    }

    for (qint32 i = 1; i < text.size(); i++){
        if (fm->width(text.mid(0,i)) > effectiveWidth){
            colEnd = i;
            break;
        }
        colEnd++;
    }

    colCursor = 0;
    config.set(CPR_TEXT,text);

}

void ColoUiLineEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    Q_UNUSED(option)

    QFont f = config.getFont();
    painter->setFont(f);
    QPen pen;

    if (!config.getBool(CPR_DISABLE_BACKGROUND)){

        // Drawing background
        QBrush brush = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_BACKGROUND_COLOR),boundingBox);
        painter->setBrush(brush);

        pen.setWidth(config.getUInt16(CPR_BORDER_WIDTH));
        pen.setColor(QColor(config.getColor(CPR_BORDER_COLOR)));

        painter->setPen(pen);
        painter->drawRect(0,0,this->w,this->h);
    }

    if (config.getBool(CPR_USE_HTML)){  // HTML means read only too.
        QTextDocument td;
        td.setHtml(config.getString(CPR_TEXT));
        td.setTextWidth(this->w);
        td.documentLayout()->draw(painter,QAbstractTextDocumentLayout::PaintContext());
    }
    else{
        // Drawing plain text
        QString text = config.getString(CPR_TEXT);
        QString temp = text.mid(colStart,colCursor-colStart);
        qreal xcursor = fm->width(temp) + config.getUInt16(CPR_X_OFFSET);
        //qWarning() << "colCursor " << colCursor << " and col start is " << colStart << "xcursor" << xcursor << "for"  << temp;
        text = text.mid(colStart,colEnd-colStart+1);

        if (config.getBool(CPR_ENABLE_COVER_CHAR)){
            QString cover;
            cover.fill('*',text.length());
            text = cover;
        }

        if (editingEnabled){
            QPen oldpen = painter->pen();
            QPen pen;
            pen.setColor(QColor(config.getColor(CPR_CURSOR_COLOR)));
            pen.setWidth(3);
            painter->setPen(pen);

            QPointF p1(xcursor,0.05*this->h);
            QPointF p2 (xcursor,0.95*this->h);
            painter->drawLine(p1,p2);
            painter->setPen(oldpen);

        }
        pen.setColor(QColor(config.getColor(CPR_TEXT_COLOR)));
        pen.setWidth(0);
        painter->setPen(pen);
        painter->drawText(config.getUInt16(CPR_X_OFFSET) ,yoffset,text);
    }

}


void ColoUiLineEdit::mousePressEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e);

    if (!config.getBool(CPR_USE_HTML) && !config.getBool(CPR_READ_ONLY)){

        if (config.getBool(CPR_USE_VIRTUAL_KEYBOARD)){
            if (!virtualKeyboardInUse){
                editingEnabled = true;
                //colCursor = 0; // Allways start at the beginning.
                virtualKeyboardInUse = true;
                signalInfo.type = ST_KEYBOARD_REQUEST;
                signalSender->sendSignal(signalInfo);
                update();
            }
        }
        else{
            //colCursor = 0;
            editingEnabled = true;
            update();
        }
    }
}

void ColoUiLineEdit::focusOutEvent(QFocusEvent *e){    
    Q_UNUSED(e);
    if (editingEnabled){
        if (!config.getBool(CPR_USE_VIRTUAL_KEYBOARD)){
            signalInfo.type = ST_TEXT_CHANGED;
            signalSender->sendSignal(signalInfo);            
        }
    }
    editingEnabled = false;
    update();
}

// -------------- Key Press Related Functions -----------------

void ColoUiLineEdit::softKeyboardInterface(ColoUiKeyType kt, QString data){
    QKeyEvent *e = ColoUiElement::createKeyEvent(kt,data);
    if (kt == KT_HIDE){
        signalInfo.type = ST_KEYBOARD_HIDE;
        signalSender->sendSignal(signalInfo);
        editingEnabled = false;
        update();
    }
    else{
        editingEnabled = true;
        this->keyPressEvent(e);
    }
}

void ColoUiLineEdit::keyPressEvent(QKeyEvent *e){

    // The only global exception is appending a text file
    if (e->modifiers() == Qt::ControlModifier){
        if (e->key() == Qt::Key_C){
            QApplication::clipboard()->setText(config.getString(CPR_TEXT));
            return;
        }
    }

    if (!editingEnabled) return;

    if (e->modifiers() == Qt::ControlModifier){
        if (e->key() == Qt::Key_V){

            // Copying only the first line.
            QString text = QApplication::clipboard()->text();
            QStringList lines = text.split(QRegExp("[\r\n]"));
            config.set(CPR_TEXT,lines.first());
            update();
            return;
        }
    }

    QString text = config.getString(CPR_TEXT);

    //qWarning() << "colStart colCursor colEnd" << colStart  << colCursor << colEnd;

    switch (e->key()){
    case Qt::Key_Left:
        if (colCursor > 0) colCursor--;
        correctLineColWindow(text);
        break;
    case Qt::Key_Return:
        if (config.getBool(CPR_USE_VIRTUAL_KEYBOARD)){
            signalInfo.type = ST_KEYBOARD_HIDE;
            signalSender->sendSignal(signalInfo);
            virtualKeyboardInUse = false;
        }
        editingEnabled = false;
        break;
    case Qt::Key_Right:
        if (colCursor < text.size()) colCursor++;
        correctLineColWindow(text);
        break;
    case Qt::Key_Backspace:
        if (colCursor > 0){
            text.remove(colCursor-1,1);
            colCursor--;
            correctLineColWindow(text);
        }
        break;
    case Qt::Key_Home:
        setText(text);
        break;
    case Qt::Key_End:
        setText(text);
        break;
    case Qt::Key_Space:
        text.insert(colCursor," ");
        colCursor++;
        correctLineColWindow(text);
        break;
    default:
        if (acceptedInput.exactMatch(e->text())){
            text.insert(colCursor,e->text());
            colCursor++;
            correctLineColWindow(text);

            signalInfo.type = ST_VALUE_CHANGED;
            signalSender->sendSignal(signalInfo);

        }
        break;
    }
    //qWarning() << "Updating: " << colStart << colCursor << colEnd;
    config.set(CPR_TEXT,text);
    update();
}

void ColoUiLineEdit::correctLineColWindow(QString text){

    qreal sW = fm->width(text.mid(colStart,colEnd-colStart));
    if (sW < effectiveWidth){
        colStart = 0;
        colEnd = text.size();
    }
    else{
        if (colCursor >= colEnd){
            if (colEnd < text.size()){
                colEnd++;
                colStart++;
            }
        }
        else if (colCursor <= colStart){
            if (colStart > 0){
                colEnd--;
                colStart--;
            }
        }
    }

}

