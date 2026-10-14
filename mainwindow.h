#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>

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

private:
    void setupUi();
    void setupMenu();
    void setupToolBar();
    void setupStatusBar();

    QLineEdit   *m_titleEdit;
    QSpinBox    *m_playersSpin;
    QSpinBox    *m_timeSpin;
    QComboBox   *m_difficultyCombo;
    QTextEdit   *m_descriptionEdit;
    QPushButton *m_addButton;
    QPushButton *m_clearButton;
};

#endif // MAINWINDOW_H