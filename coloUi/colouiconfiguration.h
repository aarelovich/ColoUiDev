#ifndef COLOUICONFIG_H
#define COLOUICONFIG_H

#include "colouidefines.h"

class ColoUiConfiguration
{
public:
    ColoUiConfiguration();

    void set(QString propertyName, QVariant value);

    void setGradient(QString propertyName, QStringList colors, qint32 gtype);
    void setColor(QString propertyName, QString color);
    void setFont(QFont f);
    void configureAsTransition(QString viewA, QString viewB, qint32 duration, qint32 steps);

    // Getters.
    quint16 getUInt16(QString name) const {return config.value(name,0).toUInt();}
    qint32 getInt32(QString name) const {return config.value(name,0).toInt();}
    QFont getFont() const;
    QString getString(QString name) const {return config.value(name,"").toString();}
    QStringList getStringList(QString name) const {return config.value(name,QStringList()).toStringList();}
    QVariantHash getGradient(QString propertyName) const;
    QString getColor(QString propertyName) const;
    qreal getAirX() const;
    qreal getAirY() const;
    bool getBool(QString name) const  {return config.value(name,false).toBool();}

    void removeProperty(QString name);

    bool has(QString propertyName) const {return config.contains(propertyName);}

    static QBrush configureBrushForGradient(QVariantHash grad, QRectF box);
    static QVariantHash lightenColors(QVariantHash grad);
    static QLinearGradient getLinearGradient(QVariantHash colorInfo, QRectF box);
    static QRadialGradient getRadialGradient(QVariantHash colorInfo, QRectF box);

private:
    QVariantHash config;

};

#endif // COLOUICONFIG_H
