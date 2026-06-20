#pragma once

#include <QMainWindow>

class QGraphicsView;
class MapScene;

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorMainWindow(QWidget* parent = nullptr);

private:
    MapScene* m_scene;
    QGraphicsView* m_view;
};