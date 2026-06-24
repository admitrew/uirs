#pragma once

#include <QMainWindow>
#include <QString>

class QGraphicsView;
class MapScene;

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorMainWindow(QWidget* parent = nullptr);

private:
    void createMenus();
    void openTh2File();
    void saveTh2FileAs();
    void loadTh2File(const QString& filePath);
    void fitSceneToView();

    MapScene* m_scene;
    QGraphicsView* m_view;
};