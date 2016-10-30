#ifndef COLOUIDROPDOWNLIST_H
#define COLOUIDROPDOWNLIST_H

#include "colouiitem.h"

class ColoUiDropdownList: public ColoUiElement
{
public:
    ColoUiDropdownList(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    QGraphicsItem *getPlyList() const {return plyList;}

    // Interface with plylist
    ColoUiConfiguration getItem(qint32 id) const {return plyList->getItem(id);}
    ColoUiConfiguration getCurrentItem() const {return plyList->getCurrentItem();}
    qint32 getCurrentItemIndex() const {return plyList->getCurrentItemPosition();}
    void setCurrentIndex(qint32 id) {plyList->setCurrentIndex(id);}
    bool isThereASelection() const {return plyList->selectionEmpty();}
    void addItem(ColoUiConfiguration id){plyList->addItem(id);}
    void addItem(QString text){plyList->addItem(text);}
    void clearSelection(){plyList->clearSelection();}
    void clearItems(){plyList->clearItems();}
    void removeItem(qint32 index){plyList->removeItem(index);}
    void itemChanged(qint32 currentItem);
    void toggleList(bool unfold);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);

private:

    // The button part and each of the items configuration
    ColoUiConfiguration main;

    ColoUiItem::ItemState colorState;

    class PlyList: public QGraphicsItem
    {
    public:

        PlyList(ColoUiDropdownList *p);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const { return boundingBox; }

        void configure(qint32 n, qreal w, qreal h, QColor tColor);

        ColoUiConfiguration getItem(qint32 id) const;
        void addItem(ColoUiConfiguration id);

        // Adds an item by using the last configuration added as a template.
        // If configuration is empty it creates a standard configuration.
        void addItem(QString text);

        ColoUiConfiguration getCurrentItem() const;
        qint32 getCurrentItemPosition() const  {return currentIndex;}

        void clearSelection(); // This resets to the non selection simple text

        // Eliminates all items
        void clearItems();

        // Remove a single item
        void removeItem(qint32 index);

        // Checking if there is something selected.
        bool selectionEmpty() const {return currentIndex == -1;}

        // Checking if there is something to to show
        bool itemsEmpty() const {return items.isEmpty();}

        // Set current index
        void setCurrentIndex(qint32 id);

        QPainterPath getDropDownIndicatorPath() const {return dropDownIndicator;}

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *e);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
        void wheelEvent(QGraphicsSceneWheelEvent *e);
        void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);

    private:

        ColoUiDropdownList *parent;

        QVector<ColoUiConfiguration>  items;

        qreal itemW;
        qreal itemH;
        qint32 itemsToShow;
        QColor textColor;

        QRectF boundingBox;

        // Currently selected item and where to start drawing.
        qint32 currentIndex;
        qint32 itemToStartDrawing;
        qint32 hoverItem;

        // Arrow drawing variables.
        qreal directionIndicatorHeight;
        QRectF centerArrow;
        QRectF arrowLeft;
        QRectF arrowRight;
        QPainterPath dropDownIndicator;

        // Scrolling variables
        qreal lastY;
        qreal accDeltaY;

        void updateItemToDraw(bool up);
        void updateBoundingBox();

        typedef enum {TT_UP, TT_DOWN, TT_AT_45} TriangleType;
        QPainterPath drawArrow(QRectF r, TriangleType tt);

    };

    PlyList *plyList;


};

#endif // COLOUIDROPDOWNLIST_H
