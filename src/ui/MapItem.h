#pragma once

#include <QGraphicsItem>

// Абстрактный базовый класс для всех элементов карты
class MapItem : public QGraphicsItem
{
protected:
    bool m_highlighted = false;

public:
    explicit MapItem(QGraphicsItem* parent = nullptr)
        : QGraphicsItem(parent) {}

    virtual ~MapItem() = default;

    void setHighlighted(bool h) {
        m_highlighted = h;
        update();
    }   
};
