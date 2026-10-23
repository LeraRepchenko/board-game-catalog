#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onAddGame();
    void onClearForm();
    void onExit();
    void onAutosave();
    void onSave();
    void onDelete();
    void onSettings();

private:
    void setupUi();
    void setupMenu();
    void setupToolBar();
    void setupStatusBar();
    void setupAutosave();

    // Поля формы
    QLineEdit   *m_titleEdit;
    QSpinBox    *m_playersSpin;
    QSpinBox    *m_timeSpin;
    QComboBox   *m_difficultyCombo;
    QTextEdit   *m_descriptionEdit;

    // Кнопки действий
    QPushButton *m_addButton;
    QPushButton *m_clearButton;

    QTimer *m_autosaveTimer;
};

#endif // MAINWINDOW_H