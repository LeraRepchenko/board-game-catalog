#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QListView>
#include <QStringListModel>
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

    // Поля формы
    QLineEdit   *m_titleEdit;
    QSpinBox    *m_playersSpin;
    QSpinBox    *m_timeSpin;
    QComboBox   *m_difficultyCombo;
    QTextEdit   *m_descriptionEdit;

    // Model-View
    QListView *m_gameList;
    QStringListModel *m_gameModel;
    QList<Game> m_games; // Хранилище данных

    // Кнопки
    QPushButton *m_addButton;
    QPushButton *m_clearButton;

    QTimer *m_autosaveTimer;
};

#endif // MAINWINDOW_H