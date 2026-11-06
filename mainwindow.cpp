#include "mainwindow.h"
#include "gamedelegate.h"

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
#include <QListView>

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

    // Главный слой
    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(16, 16, 16, 16);

    // --- 1. ФОРМА (с адаптивностью ЛР4) ---
    QGroupBox *formGroup = new QGroupBox("Добавить игру", central);
    QFormLayout *formLayout = new QFormLayout(formGroup);
    formLayout->setContentsMargins(8, 8, 8, 8);
    formLayout->setSpacing(10);

    // Название: тянется вширь
    m_titleEdit = new QLineEdit(formGroup);
    m_titleEdit->setPlaceholderText("Название игры");
    m_titleEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    formLayout->addRow("Название:", m_titleEdit);

    // Игроков: фиксированное, обернуто в пружину
    m_playersSpin = new QSpinBox(formGroup);
    m_playersSpin->setRange(1, 20);
    m_playersSpin->setValue(2);
    m_playersSpin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    auto *playersRow = new QHBoxLayout();
    playersRow->addWidget(m_playersSpin);
    playersRow->addStretch();
    formLayout->addRow("Игроков:", playersRow);

    // Время: фиксированное, обернуто в пружину
    m_timeSpin = new QSpinBox(formGroup);
    m_timeSpin->setRange(5, 600);
    m_timeSpin->setValue(60);
    m_timeSpin->setSuffix(" мин");
    m_timeSpin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    auto *timeRow = new QHBoxLayout();
    timeRow->addWidget(m_timeSpin);
    timeRow->addStretch();
    formLayout->addRow("Время партии:", timeRow);

    // Сложность: тянется вширь
    m_difficultyCombo = new QComboBox(formGroup);
    m_difficultyCombo->addItems({"Лёгкая", "Средняя", "Сложная"});
    m_difficultyCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    formLayout->addRow("Сложность:", m_difficultyCombo);

    // Описание: тянется вширь и ввысь (как в ЛР4)
    m_descriptionEdit = new QTextEdit(formGroup);
    m_descriptionEdit->setPlaceholderText("Описание игры...");
    m_descriptionEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    formLayout->addRow("Описание:", m_descriptionEdit);

    rootLayout->addWidget(formGroup);

    // --- 2. СПИСОК (Model-View из ЛР5) ---
    QGroupBox *listGroup = new QGroupBox("Список игр", central);
    QVBoxLayout *listLayout = new QVBoxLayout(listGroup);

    m_gameList = new QListView(listGroup);
    m_gameModel = new QStringListModel(this);
    m_gameList->setModel(m_gameModel);
    m_gameList->setItemDelegate(new GameDelegate(m_gameList));

    // Адаптивность для списка (ЛР4): тянется по обеим осям
    m_gameList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    listLayout->addWidget(m_gameList);

    // Список забирает всё свободное место по вертикали (stretch=1)
    rootLayout->addWidget(listGroup, 1);

    // --- 3. КНОПКИ (с пружиной вправо из ЛР4) ---
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch(); // Пружина: прижимает кнопки вправо

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

    m_clearButton = new QPushButton("Очистить всё", central); // Переименовали для ясности
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
    QAction *exitAction = fileMenu->addAction("В&ыход");
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExit);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolbar = addToolBar("Главная");
    toolbar->setMovable(false);
    toolbar->addAction("Добавить", this, &MainWindow::onAddGame);
    toolbar->addAction("Сохранить", this, &MainWindow::onSave);
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

    m_games.append(newGame);
    updateGameList();

    // Очищаем форму после добавления (без очистки списка!)
    m_titleEdit->clear();
    m_playersSpin->setValue(2);
    m_timeSpin->setValue(60);
    m_difficultyCombo->setCurrentIndex(0);
    m_descriptionEdit->clear();

    logMsg("[CRUD] Игра добавлена: " + newGame.title);
}

void MainWindow::onDelete()
{
    QModelIndex current = m_gameList->currentIndex();

    if (!current.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите игру для удаления.");
        return;
    }

    int row = current.row();
    auto reply = QMessageBox::question(this, "Удаление",
                                       "Удалить игру \"" + m_games[row].title + "\"?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        logMsg("[CRUD] Игра удалена: " + m_games[row].title);
        m_games.removeAt(row);
        updateGameList();
    }
}

void MainWindow::onClearForm()
{
    if (m_games.isEmpty()) {
        QMessageBox::information(this, "Пусто", "Список уже пуст.");
        return;
    }

    auto reply = QMessageBox::question(this, "Очистка",
                                       "Вы уверены, что хотите очистить ВЕСЬ список игр?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_games.clear();
        updateGameList();
        logMsg("[CRUD] Список игр очищен полностью");
    }
}

void MainWindow::updateGameList()
{
    QStringList list;
    for (const Game &g : m_games) {
        QString item = QString("%1  |  %2 игроков  |  %3 мин  |  %4")
                           .arg(g.title)
                           .arg(g.players)
                           .arg(g.timeMinutes)
                           .arg(g.difficulty);
        list << item;
    }
    // Передаем данные в модель, View обновится сам
    m_gameModel->setStringList(list);
}

void MainWindow::onAutosave()
{
    QString time = QTime::currentTime().toString("HH:mm:ss");
    logMsg(QString("[AUTOSAVE] Данные сохранены в %1").arg(time));
    statusBar()->showMessage(QString("Автосохранение в %1").arg(time), 3000);
}

void MainWindow::onSave()
{
    logMsg("[LOG] Сохранение. Всего игр: " + QString::number(m_games.size()));
    statusBar()->showMessage("Сохранено (в памяти)", 3000);
}

void MainWindow::onSettings()
{
    QMessageBox::information(this, "Настройки", "Настройки будут позже.");
}

void MainWindow::onExit()
{
    QApplication::quit();
}