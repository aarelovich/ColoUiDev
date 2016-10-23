#include "colouiconfiguration.h"

ColoUiConfiguration::ColoUiConfiguration()
{
    QStringList textcolor = QStringList() << "#000000";
    QStringList textAltcolor = QStringList() << "#888888";
    QStringList bcolor = QStringList() << "#AAAAAA";
    QStringList bAltcolor = QStringList() << "#FFFFFF";

    config[CPR_SHAPE] = CPA_RECT;
    this->setGradient(CPR_TEXT_COLOR,textcolor,CPA_GRAD_NONE);
    this->setGradient(CPR_ALTERNATIVE_TEXT_COLOR,textAltcolor,CPA_GRAD_NONE);
    this->setGradient(CPR_BACKGROUND_COLOR,bcolor,CPA_GRAD_NONE);
    this->setGradient(CPR_ALTERNATIVE_BACKGROUND_COLOR,bAltcolor,CPA_GRAD_NONE);
    this->setGradient(CPR_BORDER_COLOR,textcolor,CPA_GRAD_NONE);
    this->setGradient(CPR_SCROLLBAR_BACKGROUND,textcolor,CPA_GRAD_NONE);
    this->setGradient(CPR_SCROLL_SLIDER,bAltcolor,CPA_GRAD_NONE);
    config[CPR_VALUES_RELATIVE] = true;
    config[CPR_READ_ONLY] = false;
    config[CPR_USE_HTML] = false;
    config[CPR_COVER_CHAR] = false;
    config[CPR_DISABLE_BACKGROUND] = false;
    config[CPR_SHOW_VALUE] = true;
    config[CPR_USE_VIRTUAL_KEYBOARD] = false;
    config[CPR_ENABLE_COVER_CHAR] = false;
    config[CPR_TEXT] =  "";
    config[CPR_BORDER_WIDTH] = 0;
    config[CPR_V_SCROLLBAR] = false;
    config[CPR_SLIDER_SPREAD] = 10;
    config[CPR_ICON_POSITION] = CPA_LEFT;

}

void ColoUiConfiguration::setFont(QFont f){
    QVariantHash v;
    v[INTERNAL_FONT_FAMILY] = f.family();
    v[INTERNAL_FONT_BOLD] = f.bold();
    v[INTERNAL_FONT_ITALIC] = f.italic();

    quint16 psize;
    if (f.pixelSize() == 1){
        psize = f.pointSize();
    }
    else{
        psize = f.pixelSize();
    }

    v[INTERNAL_FONT_SIZE] = psize;
    config[CPR_FONT] = v;

}

QFont ColoUiConfiguration::getFont() const {
    QVariantHash v = config.value(CPR_FONT).toHash();
    QFont f(v.value(INTERNAL_FONT_FAMILY).toString());
    f.setPixelSize(v.value(INTERNAL_FONT_SIZE,1).toInt());
    f.setBold(v.value(INTERNAL_FONT_BOLD).toBool());
    f.setItalic(v.value(INTERNAL_FONT_ITALIC).toBool());
    return f;
}

void ColoUiConfiguration::set(QString propertyName, QVariant value){
    config[propertyName] = value;
}

void ColoUiConfiguration::setColor(QString propertyName, QString color){
    QStringList l;
    l << color;
    setGradient(propertyName,l,CPA_GRAD_NONE);
}

void ColoUiConfiguration::setGradient(QString propertyName, QStringList colors, qint32 gtype){

    QVariantHash s;
    s[INTERNAL_COLOR_LIST] = colors;
    s[INTERNAL_GRAD_TYPE] = gtype;

    config[propertyName] = s;
}

void ColoUiConfiguration::removeProperty(QString name){
    if (config.contains(name)){
        config.remove(name);
    }
}

QVariantHash ColoUiConfiguration::getGradient(QString propertyName) const {
    if (config.contains(propertyName)){
        if (config.value(propertyName).type() == QVariant::Hash){
           return config.value(propertyName).toHash();
        }
    }
    QVariantHash c;
    c[INTERNAL_COLOR_LIST] = QStringList() << "#000000";
    c[INTERNAL_GRAD_TYPE] = CPA_GRAD_NONE;
    return c;

}

QString ColoUiConfiguration::getColor(QString propertyName) const{
    if (config.contains(propertyName)){
        if (config.value(propertyName).type() == QVariant::Hash){
           return config.value(propertyName).toHash().value(INTERNAL_COLOR_LIST).toStringList().first();
        }
    }
    return "#000000";
}

void ColoUiConfiguration::configureAsTransition(QString viewA, QString viewB, qint32 duration, qint32 steps){
    config.clear();
    config[CPR_TRANSITION_VIEW_A] = viewA;
    config[CPR_TRANSITION_VIEW_B] = viewB;
    config[CPR_TRANSITION_STEPS] = steps;
    config[CPR_TRANSITION_TIME] = duration;
}

QBrush ColoUiConfiguration::configureBrushForGradient(QVariantHash grad, QRectF box){

    QStringList colors;
    QBrush brush;

    switch(grad.value(INTERNAL_GRAD_TYPE).toUInt()){
    case CPA_GRAD_RADIAL:
        brush = QBrush(getRadialGradient(grad,box));
        break;
    case CPA_GRAD_NONE:
        colors = grad.value(INTERNAL_COLOR_LIST).toStringList();
        if (colors.isEmpty()){
            brush = QBrush(Qt::black);
        }
        else{
            brush = QBrush(QColor(colors.first()));
        }
        break;
    default: // This is a linear gradient.
        brush = QBrush(getLinearGradient(grad,box));        
        break;
    }

    return brush;

}

QVariantHash ColoUiConfiguration::lightenColors(QVariantHash grad){

    QStringList colors = grad.value(INTERNAL_COLOR_LIST).toStringList();

    for (qint32 i = 0; i < colors.size(); i++){
        if (colors.at(i) == "#000000"){ // Black does not work with lighter.
            colors[i] = "#888888";
        }
        else{
            QColor c(colors.at(i));
            c = c.lighter();
            colors[i] = c.name();
        }
    }

    grad[INTERNAL_COLOR_LIST] = colors;

    return grad;

}

QLinearGradient ColoUiConfiguration::getLinearGradient(QVariantHash colorInfo, QRectF box){

    QStringList l = colorInfo.value(INTERNAL_COLOR_LIST).toStringList();

    QColor c1,c2;

    if (l.size() < 2){
        if (l.isEmpty()){
            c1 = QColor(Qt::white);
            c2 = QColor(Qt::gray);
        }
        else{
            c1 = QColor(l.at(0));
            c2 = c1.lighter();
        }
    }
    else{
        c1 = QColor(l.first());
        c2 = QColor(l.last());
    }

    QLinearGradient g;

    QPointF p1, p2;

    switch (colorInfo.value(INTERNAL_GRAD_TYPE).toInt()){

    case CPA_GRAD_HORIZONTAL:
        p1.setX(box.left());
        p1.setY(box.height()/2 + box.top());
        p2.setX(box.right());
        p2.setY(p1.y());
        break;
    case CPA_GRAD_VERTICAL:
        p1.setX(box.width()/2+box.left());
        p1.setY(box.top());
        p2.setX(p1.x());
        p2.setY(box.bottom());
        break;
    default:
        p1 = box.topLeft();
        p2 = box.bottomRight();
        break;

    }

    //qDebug() << "Linear gradient with colors" << c1.name() << c2.name() << " on points " << p1 << p2;

    g = QLinearGradient(p1,p2);
    g.setColorAt(0,c1);
    g.setColorAt(1,c2);

    return g;

}

QRadialGradient ColoUiConfiguration::getRadialGradient(QVariantHash colorInfo, QRectF box){

    QStringList l = colorInfo.value(INTERNAL_COLOR_LIST).toStringList();

    QColor c1,c2;

    if (l.size() < 2){
        if (l.isEmpty()){
            c1 = QColor(Qt::white);
            c2 = QColor(Qt::gray);
        }
        else{
            c1 = QColor(l.at(0));
            c2 = c1.lighter();
        }        
    }
    else{
        c1 = QColor(l.first());
        c2 = QColor(l.last());
    }

    qreal rad = qMax(box.width(),box.height());
    QPointF c;
    c.setX(box.left() + box.width()/2);
    c.setY(box.top() + box.height()/2);

    QRadialGradient g = QRadialGradient(c,rad);   

    g.setColorAt(0,c1);
    g.setColorAt(1,c2);

    return g;
}

