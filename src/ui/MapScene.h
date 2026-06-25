#pragma once

#include <QGraphicsScene>

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MapScene(QObject* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
};