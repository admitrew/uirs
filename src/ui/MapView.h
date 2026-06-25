#pragma once

#include <QGraphicsView>
#include <QPoint>

class QWheelEvent;
class QMouseEvent;

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapView(QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool m_isPanning = false;
    QPoint m_lastPanPoint;
};