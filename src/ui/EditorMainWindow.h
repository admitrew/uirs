#pragma once

#include <QMainWindow>
#include <QString>
#include <QStringList>

class QGraphicsView;
class QAction;
class MapScene;

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorMainWindow(QWidget* parent = nullptr);

private:
    void createMenus();
    void createFileMenu();
    void createToolsMenu();

    void openTh2File();
    void saveTh2FileAs();
    void loadTh2File(const QString& filePath);
    void fitSceneToView();

    void setSelectMode();
    void setAddPointMode(const QString& pointType);
    void setAddLineMode(const QString& lineType);

    MapScene* m_scene;
    QGraphicsView* m_view;

    QStringList m_headerLines;
    QString m_scrapLine;
    QString m_endScrapLine;
    QStringList m_areaBlocks;
};