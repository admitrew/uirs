class EditorToolBar : public QToolBar {
    Q_OBJECT
public:
    explicit EditorToolBar(QWidget* parent = nullptr);

    ToolMode mode() const;
    PointType currentPointType() const;
    LineType currentLineType() const;

signals:
    void toolChanged();

private:
    QComboBox* pointBox;
    QComboBox* lineBox;
    QAction* pointAction;
    QAction* lineAction;

    ToolMode m_mode = ToolMode::None;
};
