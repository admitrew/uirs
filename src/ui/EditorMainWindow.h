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
class QCloseEvent;

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorMainWindow(QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void createMenus();
    void createFileMenu();
    void createToolsMenu();
    void createToolBar();
    void createPropertiesPanel();

    void openTh2File();
    bool saveCurrentFile();
    bool saveTh2FileAs();
    bool writeTh2File(const QString& filePath);

    bool maybeSaveCurrentFile();

    void loadTh2File(const QString& filePath);
    void fitSceneToView();
    void updateWindowTitle();

    void setSelectMode();
    void setAddPointMode(const QString& pointType);
    void setAddLineMode(const QString& lineType);

    void updateToolBarState();

    void showPropertiesPanel();
    void updatePropertiesPanel();
    void clearPropertiesPanel();
    void applyPropertiesToSelectedItem();

    void markModified();
    void setModified(bool modified);

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

    QString m_currentFilePath;

    bool m_isModified = false;
    bool m_isLoadingFile = false;
};