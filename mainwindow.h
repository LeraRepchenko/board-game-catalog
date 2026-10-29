#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QListWidget>
#include <QList>
#include "game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onAddGame();
    void onDelete();
    void onSave();
    void onSettings();
    void onClearForm();
    void onExit();
    void onAutosave();

private:
    void setupUi();
    void setupMenu();
    void setupToolBar();
    void setupStatusBar();
    void setupAutosave();
    void updateGameList();

    QLineEdit   *m_titleEdit;
    QSpinBox    *m_playersSpin;
    QSpinBox    *m_timeSpin;
    QComboBox   *m_difficultyCombo;
    QTextEdit   *m_descriptionEdit;

    QListWidget *m_gameList;
    QList<Game> m_games;

    QPushButton *m_addButton;
    QPushButton *m_clearButton;

    QTimer *m_autosaveTimer;
};

#endif // MAINWINDOW_H