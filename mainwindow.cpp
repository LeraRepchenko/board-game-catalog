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
#include <QTime>
#include <QTextStream>
#include <QStringConverter>
#include <QSizePolicy>
#include <QListWidget>

inline void logMsg(const QString &msg)
{
    QTextStream out(stdout);
    out.setEncoding(QStringConverter::Utf8);
    out << msg << "\n";
    out.flush();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupMenu();
    setupToolBar();
    setupStatusBar();
    setupAutosave();
}

MainWindow::~MainWindow()
{
    logMsg("[DESTROY] MainWindow");
}

void MainWindow::setupUi()
{
    setWindowTitle("Каталог настольных игр (Model-View)");
    resize(800, 650);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(16, 16, 16, 16);

    QGroupBox *formGroup = new QGroupBox("Добавить игру", central);
    QFormLayout *formLayout = new QFormLayout(formGroup);
    formLayout->setContentsMargins(8, 8, 8, 8);

    m_titleEdit = new QLineEdit(formGroup);
    m_titleEdit->setPlaceholderText("Название игры");
    m_titleEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    formLayout->addRow("Название:", m_titleEdit);

    m_playersSpin = new QSpinBox(formGroup);
    m_playersSpin->setRange(1, 20);
    m_playersSpin->setValue(2);
    auto *playersRow = new QHBoxLayout();
    playersRow->addWidget(m_playersSpin);
    playersRow->addStretch();
    formLayout->addRow("Игроков:", playersRow);

    m_timeSpin = new QSpinBox(formGroup);
    m_timeSpin->setRange(5, 600);
    m_timeSpin->setValue(60);
    m_timeSpin->setSuffix(" мин");
    auto *timeRow = new QHBoxLayout();
    timeRow->addWidget(m_timeSpin);
    timeRow->addStretch();
    formLayout->addRow("Время партии:", timeRow);

    m_difficultyCombo = new QComboBox(formGroup);
    m_difficultyCombo->addItems({"Лёгкая", "Средняя", "Сложная"});
    formLayout->addRow("Сложность:", m_difficultyCombo);

    m_descriptionEdit = new QTextEdit(formGroup);
    m_descriptionEdit->setPlaceholderText("Описание игры...");
    m_descriptionEdit->setMaximumHeight(80);
    formLayout->addRow("Описание:", m_descriptionEdit);

    rootLayout->addWidget(formGroup);

    QGroupBox *listGroup = new QGroupBox("Список игр", central);
    QVBoxLayout *listLayout = new QVBoxLayout(listGroup);
    m_gameList = new QListWidget(listGroup);
    listLayout->addWidget(m_gameList);
    rootLayout->addWidget(listGroup, 1);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_addButton = new QPushButton("Добавить", central);
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddGame);
    btnLayout->addWidget(m_addButton);

    QPushButton *deleteBtn = new QPushButton("Удалить", central);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDelete);
    btnLayout->addWidget(deleteBtn);

    QPushButton *saveBtn = new QPushButton("Сохранить", central);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSave);
    btnLayout->addWidget(saveBtn);

    QPushButton *settingsBtn = new QPushButton("Настройки", central);
    connect(settingsBtn, &QPushButton::clicked, this, &MainWindow::onSettings);
    btnLayout->addWidget(settingsBtn);

    m_clearButton = new QPushButton("Очистить", central);
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::onClearForm);
    btnLayout->addWidget(m_clearButton);

    rootLayout->addLayout(btnLayout);
}

void MainWindow::setupMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("&Файл");
    QAction *newAction = fileMenu->addAction("&Добавить игру");
    newAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(newAction, &QAction::triggered, this, &MainWindow::onAddGame);

    fileMenu->addSeparator();
    QAction *saveAction = fileMenu->addAction("&Сохранить");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSave);

    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("В&ыход");
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExit);

    QMenu *helpMenu = menuBar()->addMenu("&Справка");
    QAction *aboutAction = helpMenu->addAction("&О программе");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "О программе", "Каталог настольных игр\nЭтап 2. Model-View.");
    });
}

void MainWindow::setupToolBar()
{
    QToolBar *toolbar = addToolBar("Главная");
    toolbar->setMovable(false);
    QAction *addAct = toolbar->addAction("Добавить");
    connect(addAct, &QAction::triggered, this, &MainWindow::onAddGame);
    QAction *saveAct = toolbar->addAction("Сохранить");
    connect(saveAct, &QAction::triggered, this, &MainWindow::onSave);
    QAction *settingsAct = toolbar->addAction("Настройки");
    connect(settingsAct, &QAction::triggered, this, &MainWindow::onSettings);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Готово.");
}

void MainWindow::setupAutosave()
{
    m_autosaveTimer = new QTimer(this);
    m_autosaveTimer->setInterval(60 * 1000);
    connect(m_autosaveTimer, &QTimer::timeout, this, &MainWindow::onAutosave);
    m_autosaveTimer->start();
    logMsg("[TIMER] Автосохранение запущено (интервал 60 сек)");
}

void MainWindow::onAddGame()
{
    if (m_titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название игры.");
        return;
    }

    Game newGame;
    newGame.title = m_titleEdit->text().trimmed();
    newGame.players = m_playersSpin->value();
    newGame.timeMinutes = m_timeSpin->value();
    newGame.difficulty = m_difficultyCombo->currentText();
    newGame.description = m_descriptionEdit->toPlainText();

    m_games.append(newGame);
    updateGameList();
    onClearForm();

    logMsg("[LOG] Игра добавлена: " + newGame.title);
    statusBar()->showMessage("Игра успешно добавлена", 3000);
}

void MainWindow::onDelete()
{
    int currentRow = m_gameList->currentRow();

    if (currentRow < 0 || currentRow >= m_games.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите игру из списка для удаления.");
        return;
    }

    auto reply = QMessageBox::question(this, "Удаление",
                                       "Вы уверены, что хотите удалить игру \"" + m_games[currentRow].title + "\"?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        logMsg("[LOG] Игра удалена: " + m_games[currentRow].title);
        m_games.removeAt(currentRow);
        updateGameList();
        statusBar()->showMessage("Игра удалена", 3000);
    }
}

void MainWindow::updateGameList()
{
    m_gameList->clear();
    for (const Game &g : m_games) {
        QString text = QString("%1 | %2 игроков | %3 мин | %4")
                           .arg(g.title)
                           .arg(g.players)
                           .arg(g.timeMinutes)
                           .arg(g.difficulty);
        m_gameList->addItem(text);
    }
}

void MainWindow::onClearForm()
{
    m_titleEdit->clear();
    m_playersSpin->setValue(2);
    m_timeSpin->setValue(60);
    m_difficultyCombo->setCurrentIndex(0);
    m_descriptionEdit->clear();
    logMsg("[LOG] Форма очищена");
}

void MainWindow::onAutosave()
{
    QString time = QTime::currentTime().toString("HH:mm:ss");
    logMsg(QString("[AUTOSAVE] Данные сохранены в %1").arg(time));
    statusBar()->showMessage(QString("Автосохранение в %1").arg(time), 3000);
}

void MainWindow::onSave()
{
    logMsg("[LOG] Ручное сохранение. Всего игр: " + QString::number(m_games.size()));
    statusBar()->showMessage("Сохранено (в памяти)", 3000);
}

void MainWindow::onSettings()
{
    logMsg("[LOG] Открытие настроек");
    QMessageBox::information(this, "Настройки", "Настройки будут добавлены позже.");
}

void MainWindow::onExit()
{
    QApplication::quit();
}