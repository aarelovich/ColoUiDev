#include "colouikeyboard.h"

QFont ColoUiKeyboard::font;

ColoUiKeyboard::ColoUiKeyboard(qreal w, qreal he)
{
    qreal hh = 0.3*he;
    boundingBox = QRectF(0,0,w,hh);
    qreal ux = w/20.0;
    qreal uy = hh/10.0;

    Key key;
    qint32 y;
    qint32 h = 2;
    keyboardRowHeight = h*uy;

    //************* FIRST ROW *****************
    QVector<Key> row;
    y = 0;
    key = Key();               key.setDimension( 0,y,2,h,ux,uy); key.setStringsForStates("0","@",""); row << key;
    key = Key();               key.setDimension( 2,y,2,h,ux,uy); key.setStringsForStates("1","_",""); row << key;
    key = Key();               key.setDimension( 4,y,2,h,ux,uy); key.setStringsForStates("2","-",""); row << key;
    key = Key();               key.setDimension( 6,y,2,h,ux,uy); key.setStringsForStates("3","+",""); row << key;
    key = Key(KT_TEXT,KT_UP);  key.setDimension( 8,y,2,h,ux,uy); key.setStringsForStates("4","*",""); row << key;
    key = Key(KT_TEXT,KT_UP);  key.setDimension(10,y,2,h,ux,uy); key.setStringsForStates("5","=",""); row << key;
    key = Key();               key.setDimension(12,y,2,h,ux,uy); key.setStringsForStates("6","/",""); row << key;
    key = Key();               key.setDimension(14,y,2,h,ux,uy); key.setStringsForStates("7","#",""); row << key;
    key = Key();               key.setDimension(16,y,2,h,ux,uy); key.setStringsForStates("8","$",""); row << key;
    key = Key();               key.setDimension(18,y,2,h,ux,uy); key.setStringsForStates("9","%",""); row << key;

    keyboard << row;

    //************* SECOND ROW *****************
    row.clear();
    y = 2;
    key = Key(KT_TEXT,KT_LEFT);   key.setDimension( 0,y,2,h,ux,uy); key.setStringsForStates("q","Q",""); row << key;
    key = Key(KT_TEXT,KT_LEFT);   key.setDimension( 2,y,2,h,ux,uy); key.setStringsForStates("w","W",""); row << key;
    key = Key();                  key.setDimension( 4,y,2,h,ux,uy); key.setStringsForStates("e","E",""); row << key;
    key = Key();                  key.setDimension( 6,y,2,h,ux,uy); key.setStringsForStates("r","R",""); row << key;
    key = Key(KT_TEXT,KT_DOWN);   key.setDimension( 8,y,2,h,ux,uy); key.setStringsForStates("t","T",""); row << key;
    key = Key(KT_TEXT,KT_DOWN);   key.setDimension(10,y,2,h,ux,uy); key.setStringsForStates("y","Y",""); row << key;
    key = Key();                  key.setDimension(12,y,2,h,ux,uy); key.setStringsForStates("u","U",""); row << key;
    key = Key();                  key.setDimension(14,y,2,h,ux,uy); key.setStringsForStates("i","I",""); row << key;
    key = Key(KT_TEXT,KT_RIGHT);  key.setDimension(16,y,2,h,ux,uy); key.setStringsForStates("o","O",""); row << key;
    key = Key(KT_TEXT,KT_RIGHT);  key.setDimension(18,y,2,h,ux,uy); key.setStringsForStates("p","P",""); row << key;
    keyboard << row;

    //************* THIRD ROW *****************
    row.clear();
    y = 4;
    key = Key();  key.setDimension( 1,y,2,h,ux,uy); key.setStringsForStates("a","A","<"); row << key;
    key = Key();  key.setDimension( 3,y,2,h,ux,uy); key.setStringsForStates("s","S","["); row << key;
    key = Key();  key.setDimension( 5,y,2,h,ux,uy); key.setStringsForStates("d","D","]"); row << key;
    key = Key();  key.setDimension( 7,y,2,h,ux,uy); key.setStringsForStates("f","F","{"); row << key;
    key = Key();  key.setDimension( 9,y,2,h,ux,uy); key.setStringsForStates("g","G","}"); row << key;
    key = Key();  key.setDimension(11,y,2,h,ux,uy); key.setStringsForStates("h","H","~"); row << key;
    key = Key();  key.setDimension(13,y,2,h,ux,uy); key.setStringsForStates("j","J","'"); row << key;
    key = Key();  key.setDimension(15,y,2,h,ux,uy); key.setStringsForStates("k","K","^"); row << key;
    key = Key();  key.setDimension(17,y,2,h,ux,uy); key.setStringsForStates("l","L",">"); row << key;
    keyboard << row;


    //************* FOURTH ROW *****************
    row.clear();
    y = 6;
    key = Key(KT_BACKSPACE);  key.setDimension(0,y,3,h,ux,uy); row << key;
    key = Key();  key.setDimension( 3,y,2,h,ux,uy); key.setStringsForStates("z","Z","|"); row << key;
    key = Key();  key.setDimension( 5,y,2,h,ux,uy); key.setStringsForStates("x","X","\\"); row << key;
    key = Key();  key.setDimension( 7,y,2,h,ux,uy); key.setStringsForStates("c","C","&"); row << key;
    key = Key();  key.setDimension( 9,y,2,h,ux,uy); key.setStringsForStates("v","V","("); row << key;
    key = Key();  key.setDimension(11,y,2,h,ux,uy); key.setStringsForStates("b","B",")"); row << key;
    key = Key();  key.setDimension(13,y,2,h,ux,uy); key.setStringsForStates("n","N",";"); row << key;
    key = Key();  key.setDimension(15,y,2,h,ux,uy); key.setStringsForStates("m","M",":"); row << key;
    key = Key(KT_ENTER); key.setDimension(17,y,3,h,ux,uy); row << key;
    keyboard << row;

    //************* FIFTH ROW *****************
    row.clear();
    y = 8;
    key = Key(KT_SHIFT);  key.setDimension(0,y,2,h,ux,uy);  key.setToggle(true); row << key;
    key = Key(KT_ALT);  key.setDimension(2,y,2,h,ux,uy);  key.setToggle(true); row << key;
    key = Key();  key.setDimension(4,y,2,h,ux,uy); key.setStringsForStates(".",",","\""); row << key;
    key = Key();  key.setDimension(6,y,6,h,ux,uy); key.setStringsForStates(" "," "," "); row << key;
    key = Key();  key.setDimension(12,y,2,h,ux,uy); key.setStringsForStates("!","?",""); row << key;
    key = Key(KT_COPY); key.setDimension(14,y,2,h,ux,uy);  row << key;
    key = Key(KT_PASTE); key.setDimension(16,y,2,h,ux,uy);  row << key;
    key = Key(KT_HIDE); key.setDimension(18,y,2,h,ux,uy);  row << key;
    keyboard << row;

    font.setPixelSize(2*uy*43.0/60.0); // Empirical constant that will guarantee that the key fits in the keyboard in the same way independant of defined screen size.


}

void ColoUiKeyboard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setFont(font);

    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(boundingBox);

    for (qint32 i = 0; i < keyboard.size(); i++){
        for (qint32 j = 0; j < keyboard.at(i).size(); j++){
            keyboard.at(i).at(j).drawKey(painter);
        }
    }

}


void ColoUiKeyboard::mousePressEvent(QGraphicsSceneMouseEvent *e){

    QPointF p = e->pos();

    qint32 row = p.y()/keyboardRowHeight;
    if ((row < 0) && (row >= keyboard.size())){
        rowPressed = -1;
    }

    rowPressed = row;
    keyPressed = -1;

    // Finding which key
    for (qint32 i = 0; i < keyboard.at(row).size(); i++){
        if (keyboard.at(row).at(i).inKey(p)){
            keyPressed = i;
            break;
        }
    }

    // Invalid column pressed
    if (keyPressed == -1){
        rowPressed = -1;
        return;
    }

    // Pressing the key
    keyboard[rowPressed][keyPressed].press();
    update();

}

void ColoUiKeyboard::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    if (rowPressed >= 0){

        keyboard[rowPressed][keyPressed].release();
        ColoUiKeyType kt = keyboard.at(rowPressed).at(keyPressed).getKeyType();
        KeyState ks = keyboard.first().first().getKeyState(); // State is always the same for all keys.
        QString key;

        switch (kt) {
        case KT_ALT:
            if (ks == KS_SHIFT){ // This CAN't go to ALT mode so...
                keyboard[rowPressed][keyPressed].press();
            }
            else{
                if (ks == KS_ALT) switchKeyboardToState(KS_NORMAL);
                else switchKeyboardToState(KS_ALT);
            }
            break;
        case KT_SHIFT:
            if (ks == KS_ALT){ // This CAN't go to SHIFT mode so...
                keyboard[rowPressed][keyPressed].press();
            }
            else{
                if (ks == KS_SHIFT) switchKeyboardToState(KS_NORMAL);
                else switchKeyboardToState(KS_SHIFT);
            }
            break;
        case KT_TEXT:
            key = keyboard.at(rowPressed).at(keyPressed).getKeyContent();
            if (ks == KS_SHIFT){
                switchKeyboardToState(KS_NORMAL);
            }
            textView->softKeyboardInterface(KT_TEXT,key);
            break;
        default:
            textView->softKeyboardInterface(kt,"");
            break;
        }


        update();
    }
}

void ColoUiKeyboard::switchKeyboardToState(KeyState st){
    for (qint32 i = 0; i < keyboard.size(); i++){
        for (qint32 j = 0; j < keyboard.at(i).size(); j++){
            keyboard[i][j].switchToState(st);
        }
    }
}

void ColoUiKeyboard::loadFont(){
    // Importing the font.
    int idb  = QFontDatabase::addApplicationFont(":/assets/unispace_b.ttf");
    QFontDatabase::addApplicationFont(":/assets/unispace_reg.ttf");
    QString family = QString(QFontDatabase::applicationFontFamilies(idb).at(0));
    font = QFont(family,9);
}

//**************************** KEY CLASS *****************************

const QColor Key::background         = QColor("#1a1a1a");
const QColor Key::altbackground      = QColor("#8a8a8a");
const QColor Key::textAndBorderColor = QColor("#FFFFFF");
const QColor Key::altTextColor       = QColor("#000000");

Key::Key(ColoUiKeyType kt, ColoUiKeyType altKt){
    keyState = KS_NORMAL;
    normal = "";
    shift = "";
    alternate = "";
    useIcon = false;
    useIconAlt = false;
    isPressed = false;
    isToggle = false;
    keyType = kt;
    if (altKt == KT_NOTYPE) alternateKeyType = kt;
    else alternateKeyType = altKt;
}

ColoUiKeyType Key::getKeyType() const{
    if (keyState == KS_ALT){
        return alternateKeyType;
    }
    else return keyType;
}

void Key::setDimension(qint32 x, qint32 y, qint32 w, qint32 h, qreal ux, qreal uy){

    key = QRectF(x*ux,y*uy,w*ux,h*uy);

    switch (keyType){
    case KT_BACKSPACE:
        useIcon = true;
        icon = Key::backspacePath(w*ux,h*uy);
        icon.translate(x*ux,y*uy);
        break;
    case KT_ENTER:
        useIcon = true;
        icon = Key::returnPath(w*ux,h*uy);
        icon.translate(x*ux,y*uy);
        break;
    case KT_SHIFT:
        useIcon = true;
        icon = Key::shiftPath(w*ux,h*uy);
        icon.translate(x*ux,y*uy);
        break;
    case KT_ALT:
        useIcon = true;
        icon = Key::altPath(w*ux,h*uy);
        icon.translate(x*ux,y*uy);
        break;
    case KT_COPY:
        useIcon = true;
        icon = Key::copyPath(w*ux,h*uy);
        icon.translate(x*ux,y*uy);
        break;
    case KT_PASTE:
        useIcon = true;
        icon = Key::pastePath(w*ux,h*uy);
        icon.translate(x*ux,y*uy);
        break;
    case KT_HIDE:
        useIcon = true;
        icon = Key::hidePath(w*ux,h*uy);
        icon.translate(x*ux,y*uy);
        break;
    default:
        break;
    }

    switch (alternateKeyType){
    case KT_UP:
        useIconAlt = true;
        iconAlt = Key::upPath(w*ux,h*uy);
        iconAlt.translate(x*ux,y*uy);
        break;
    case KT_DOWN:
        useIconAlt = true;
        iconAlt = Key::downPath(w*ux,h*uy);
        iconAlt.translate(x*ux,y*uy);
        break;
    case KT_RIGHT:
        useIconAlt = true;
        iconAlt = Key::rightPath(w*ux,h*uy);
        iconAlt.translate(x*ux,y*uy);
        break;
    case KT_LEFT:
        useIconAlt = true;
        iconAlt = Key::leftPath(w*ux,h*uy);
        iconAlt.translate(x*ux,y*uy);
        break;
    default:
        break;
    }

}

void Key::press(){
    if (isToggle){
        isPressed =! isPressed;
    }
    else{
        isPressed = true;
    }
}

void Key::release(){
    if (!isToggle){
        isPressed = false;
    }
}

void Key::drawKey(QPainter *painter) const {

    QPen pen(textAndBorderColor);
    pen.setWidth(0);
    painter->setPen(pen);

    if (!isPressed) painter->setBrush(QBrush(background));
    else painter->setBrush(QBrush(altbackground));

    painter->drawRect(key);

    if (useIcon){
        if (isPressed) painter->fillPath(icon,QBrush(altTextColor));
        else painter->fillPath(icon,QBrush(textAndBorderColor));
    }
    else if ((useIconAlt) && (keyState == KS_ALT)){
        if (isPressed) painter->fillPath(iconAlt,QBrush(altTextColor));
        else painter->fillPath(iconAlt,QBrush(textAndBorderColor));
    }
    else{
        QString text;
        switch (keyState){
        case KS_ALT:
            text = alternate;
            break;
        case KS_NORMAL:
            text = normal;
            break;
        case KS_SHIFT:
            text = shift;
            break;
        }

        if (!text.isEmpty()){

            if (isPressed) pen.setColor(altTextColor);
            else pen.setColor(textAndBorderColor);

            painter->drawText(key,Qt::AlignCenter,text);

        }
    }
}

void Key::switchToState(KeyState nstate){
    switch (keyState){
    case KS_ALT:
        if (nstate == KS_SHIFT) return;
    case KS_SHIFT:
        if (nstate == KS_ALT) return;
    case KS_NORMAL:
        break;
    }
    keyState = nstate;

    // Hack to eliminate shift after just one key stroke.
    if (this->keyType == KT_SHIFT){
        if (nstate == KS_NORMAL) isPressed = false;
    }
}

QString Key::getKeyContent() const{

    switch (keyState){
    case KS_ALT:
        return alternate;
    case KS_NORMAL:
        return normal;
    case KS_SHIFT:
        return shift;
    }

    return "";
}

QPainterPath Key::backspacePath(qreal w, qreal h){

    QPainterPath p;
    p.moveTo(0.9*w,0.7*h);
    p.lineTo(0.4*w,0.7*h);
    p.lineTo(0.4*w,0.9*h);
    p.lineTo(0.1*w,0.5*h);
    p.lineTo(0.4*w,0.1*h);
    p.lineTo(0.4*w,0.3*h);
    p.lineTo(0.9*w,0.3*h);
    p.lineTo(0.9*w,0.7*h);

    return p;

}

QPainterPath Key::returnPath(qreal w, qreal h){

    QPainterPath p;
    p.moveTo(0.7*w,0.1*h);

    p.lineTo(0.9*w,0.1*h);
    p.lineTo(0.9*w,0.8*h);
    p.lineTo(0.3*w,0.8*h);
    p.lineTo(0.3*w,0.9*h);
    p.lineTo(0.1*w,0.6*h);
    p.lineTo(0.3*w,0.3*h);
    p.lineTo(0.3*w,0.4*h);
    p.lineTo(0.7*w,0.4*h);
    p.lineTo(0.7*w,0.1*h);

    return p;

}

QPainterPath Key::shiftPath(qreal w, qreal h){

    QPainterPath p;
    p.moveTo(0.3*w,0.9*h);

    p.lineTo(0.7*w,0.9*h);
    p.lineTo(0.7*w,0.5*h);
    p.lineTo(0.9*w,0.5*h);
    p.lineTo(0.5*w,0.1*h);
    p.lineTo(0.1*w,0.5*h);
    p.lineTo(0.3*w,0.5*h);
    p.lineTo(0.3*w,0.9*h);

    return p;

}

QPainterPath Key::altPath(qreal w, qreal h){

    QPainterPath p;
    p.moveTo(0.1*w,0.1*h);

    p.addRoundedRect(0.1*w,0.1*h,0.8*w,0.8*h,0.1*w,0.1*h);

    // Row 1
    p.addRect(0.15*w,0.15*h,0.7*w,0.2*h);

    // Row 2
    p.addRect(0.15*w,0.4*h,0.15*w,0.2*h);
    p.addRect(0.7*w,0.4*h,0.15*w,0.2*h);
    p.addRect(0.4*w,0.4*h,0.2*w,0.2*h);

    // Row 3
    p.addRect(0.15*w,0.65*h,0.2*w,0.2*h);
    p.addRect(0.65*w,0.65*h,0.2*w,0.2*h);
    p.addRect(0.3*w,0.65*h,0.3*w,0.2*h);


    return p;

}


QPainterPath Key::copyPath(qreal w, qreal h){

    QPainterPath p;
    p.moveTo(0.1*w,0.1*h);

    p.addRect(0.1*w,0.3*h,0.6*w,0.6*h);

    p.moveTo(0.3*w,0.3*h);
    p.lineTo(0.3*w,0.1*h);
    p.lineTo(0.9*w,0.1*h);
    p.lineTo(0.9*w,0.7*h);
    p.lineTo(0.7*w,0.7*h);

    p.addRect(0.2*w,0.6*h,0.4*w,0.1*h);


    return p;

}


QPainterPath Key::pastePath(qreal w, qreal h){

    QPainterPath p;
    p.moveTo(0.1*w,0.2*h);

    p.moveTo(0.2*w,0.2*h);
    p.lineTo(0.2*w,0.1*h);
    p.lineTo(0.7*w,0.1*h);
    p.lineTo(0.7*w,0.2*h);
    p.lineTo(0.8*w,0.2*h);
    p.lineTo(0.8*w,0.4*h);
    p.lineTo(0.9*w,0.4*h);
    p.lineTo(0.9*w,0.9*h);
    p.lineTo(0.4*w,0.9*h);
    p.lineTo(0.4*w,0.8*h);
    p.lineTo(0.1*w,0.8*h);
    p.lineTo(0.1*w,0.2*h);

    p.addRect(0.2*w,0.2*h,0.5*w,5);
    p.addRect(0.4*w,0.4*h,5,0.5*h);
    p.addRect(0.4*w,0.4*h,0.5*w,5);

    return p;

}


QPainterPath Key::hidePath(qreal w, qreal h){

    QPainterPath p;
    p.moveTo(0.1*w,0.3*h);

    p.lineTo(0.3*w,0.1*h);
    p.lineTo(0.5*w,0.3*h);
    p.lineTo(0.7*w,0.1*h);
    p.lineTo(0.9*w,0.3*h);
    p.lineTo(0.7*w,0.5*h);
    p.lineTo(0.9*w,0.7*h);
    p.lineTo(0.7*w,0.9*h);
    p.lineTo(0.5*w,0.7*h);
    p.lineTo(0.3*w,0.9*h);
    p.lineTo(0.1*w,0.7*h);
    p.lineTo(0.3*w,0.5*h);
    p.lineTo(0.1*w,0.3*h);

    return p;

}

QPainterPath Key::upPath(qreal w, qreal h){
    QPainterPath p;
    p.moveTo(0.1*w,0.9*h);
    p.lineTo(0.9*w,0.9*h);
    p.lineTo(0.5*w,0.1*h);
    p.lineTo(0.1*w,0.9*h);
    return p;
}

QPainterPath Key::downPath(qreal w, qreal h){
    QPainterPath p;
    p.moveTo(0.1*w,0.1*h);
    p.lineTo(0.9*w,0.1*h);
    p.lineTo(0.5*w,0.9*h);
    p.lineTo(0.1*w,0.1*h);
    return p;
}

QPainterPath Key::rightPath(qreal w, qreal h){
    QPainterPath p;
    p.moveTo(0.1*w,0.1*h);
    p.lineTo(0.1*w,0.9*h);
    p.lineTo(0.9*w,0.5*h);
    p.lineTo(0.1*w,0.1*h);
    return p;
}

QPainterPath Key::leftPath(qreal w, qreal h){
    QPainterPath p;
    p.moveTo(0.9*w,0.1*h);
    p.lineTo(0.9*w,0.9*h);
    p.lineTo(0.1*w,0.5*h);
    p.lineTo(0.9*w,0.1*h);
    return p;
}
