#pragma once

#include <QMainWindow>
#include <QString>
#include <QStringList>

class QGraphicsView;
class QAction;
class QComboBox;
class QDockWidget;
class QLabel;
class QLineEdit;
class QPushButton;
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
    void createToolBar();
    void createPropertiesPanel();

    void openTh2File();
    void saveTh2FileAs();
    void loadTh2File(const QString& filePath);
    void fitSceneToView();

    void setSelectMode();
    void setAddPointMode(const QString& pointType);
    void setAddLineMode(const QString& lineType);

    void updateToolBarState();

    void showPropertiesPanel();
    void updatePropertiesPanel();
    void clearPropertiesPanel();
    void applyPropertiesToSelectedItem();

    MapScene* m_scene;
    QGraphicsView* m_view;

    QAction* m_selectToolAction = nullptr;
    QAction* m_addPointToolAction = nullptr;
    QAction* m_addLineToolAction = nullptr;
    QAction* m_showPropertiesAction = nullptr;

    QComboBox* m_pointTypeCombo = nullptr;
    QComboBox* m_lineTypeCombo = nullptr;

    QDockWidget* m_propertiesDock = nullptr;
    QLabel* m_selectedObjectLabel = nullptr;
    QLineEdit* m_typeEdit = nullptr;
    QLineEdit* m_optionsEdit = nullptr;
    QPushButton* m_applyPropertiesButton = nullptr;

    QStringList m_headerLines;
    QString m_scrapLine;
    QString m_endScrapLine;
    QStringList m_areaBlocks;
};