#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QMessageBox>
#include <QAction>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupMenu();
    setupToolBar();
    setupStatusBar();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    setWindowTitle("Каталог настольных игр");
    resize(640, 520);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *rootLayout = new QVBoxLayout(central);

    QGroupBox *formGroup = new QGroupBox("Добавить игру", central);
    QFormLayout *formLayout = new QFormLayout(formGroup);

    m_titleEdit = new QLineEdit(formGroup);
    m_titleEdit->setPlaceholderText("Название игры");
    formLayout->addRow("Название:", m_titleEdit);

    m_playersSpin = new QSpinBox(formGroup);
    m_playersSpin->setRange(1, 20);
    m_playersSpin->setValue(2);
    formLayout->addRow("Игроков:", m_playersSpin);

    m_timeSpin = new QSpinBox(formGroup);
    m_timeSpin->setRange(5, 600);
    m_timeSpin->setValue(60);
    m_timeSpin->setSuffix(" мин");
    formLayout->addRow("Время партии:", m_timeSpin);

    m_difficultyCombo = new QComboBox(formGroup);
    m_difficultyCombo->addItems({"Лёгкая", "Средняя", "Сложная"});
    formLayout->addRow("Сложность:", m_difficultyCombo);

    m_descriptionEdit = new QTextEdit(formGroup);
    m_descriptionEdit->setPlaceholderText("Описание игры...");
    m_descriptionEdit->setMaximumHeight(100);
    formLayout->addRow("Описание:", m_descriptionEdit);

    rootLayout->addWidget(formGroup);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    m_addButton = new QPushButton("Добавить игру", central);
    m_addButton->setShortcut(QKeySequence("Ctrl+N"));
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddGame);
    btnLayout->addWidget(m_addButton);

    m_clearButton = new QPushButton("Очистить форму", central);
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::onClearForm);
    btnLayout->addWidget(m_clearButton);

    rootLayout->addLayout(btnLayout);
    rootLayout->addStretch();
}

void MainWindow::setupMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("&Файл");

    QAction *newAction = fileMenu->addAction("&Добавить игру");
    newAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(newAction, &QAction::triggered, this, &MainWindow::onAddGame);

    fileMenu->addSeparator();

    QAction *exitAction = fileMenu->addAction("В&ыход");
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExit);

    QMenu *helpMenu = menuBar()->addMenu("&Справка");
    QAction *aboutAction = helpMenu->addAction("&О программе");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "О программе",
                           "Каталог настольных игр\n"
                           "Сквозной проект. Этап 1 — каркас приложения.\n"
                           "Вариант 16.");
    });
}

void MainWindow::setupToolBar()
{
    QToolBar *toolbar = addToolBar("Главная");
    toolbar->setMovable(false);

    QAction *addAct = toolbar->addAction("Добавить");
    connect(addAct, &QAction::triggered, this, &MainWindow::onAddGame);

    QAction *clearAct = toolbar->addAction("Очистить");
    connect(clearAct, &QAction::triggered, this, &MainWindow::onClearForm);

    toolbar->addSeparator();

    QAction *exitAct = toolbar->addAction("Выход");
    connect(exitAct, &QAction::triggered, this, &MainWindow::onExit);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Готово. Введите данные игры и нажмите «Добавить».");
}

void MainWindow::onAddGame()
{
    if (m_titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название игры.");
        return;
    }

    QString msg = QString("Игра «%1» добавлена (%2 игроков, %3 мин, %4).")
                      .arg(m_titleEdit->text().trimmed())
                      .arg(m_playersSpin->value())
                      .arg(m_timeSpin->value())
                      .arg(m_difficultyCombo->currentText());

    statusBar()->showMessage(msg, 5000);
    QMessageBox::information(this, "Успех", msg);
}

void MainWindow::onClearForm()
{
    m_titleEdit->clear();
    m_playersSpin->setValue(2);
    m_timeSpin->setValue(60);
    m_difficultyCombo->setCurrentIndex(0);
    m_descriptionEdit->clear();
    statusBar()->showMessage("Форма очищена.", 3000);
}

void MainWindow::onExit()
{
    QApplication::quit();
}