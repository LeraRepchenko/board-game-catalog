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

// Функция для логирования (как в ЛР3)
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
    setWindowTitle("Каталог настольных игр (Адаптивный интерфейс)");
    resize(700, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // Главный вертикальный слой
    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    // Группа формы
    QGroupBox *formGroup = new QGroupBox("Добавить игру", central);
    QFormLayout *formLayout = new QFormLayout(formGroup);
    formLayout->setContentsMargins(8, 8, 8, 8);
    formLayout->setSpacing(10);

    // Поле "Название"
    m_titleEdit = new QLineEdit(formGroup);
    m_titleEdit->setPlaceholderText("Название игры");
    m_titleEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    formLayout->addRow("Название:", m_titleEdit);

    // Поле "Игроков"
    m_playersSpin = new QSpinBox(formGroup);
    m_playersSpin->setRange(1, 20);
    m_playersSpin->setValue(2);
    m_playersSpin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    auto *playersRow = new QHBoxLayout();
    playersRow->addWidget(m_playersSpin);
    playersRow->addStretch();
    formLayout->addRow("Игроков:", playersRow);

    // Поле "Время партии"
    m_timeSpin = new QSpinBox(formGroup);
    m_timeSpin->setRange(5, 600);
    m_timeSpin->setValue(60);
    m_timeSpin->setSuffix(" мин");
    m_timeSpin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto *timeRow = new QHBoxLayout();
    timeRow->addWidget(m_timeSpin);
    timeRow->addStretch();
    formLayout->addRow("Время партии:", timeRow);

    // Поле "Сложность"
    m_difficultyCombo = new QComboBox(formGroup);
    m_difficultyCombo->addItems({"Лёгкая", "Средняя", "Сложная"});
    m_difficultyCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    formLayout->addRow("Сложность:", m_difficultyCombo);

    // Поле "Описание"
    m_descriptionEdit = new QTextEdit(formGroup);
    m_descriptionEdit->setPlaceholderText("Описание игры...");
    m_descriptionEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    formLayout->addRow("Описание:", m_descriptionEdit);

    // Добавляем группу формы в главный слой
    rootLayout->addWidget(formGroup);

    // Панель кнопок действий
    QHBoxLayout *btnLayout = new QHBoxLayout();


    btnLayout->addStretch();

    m_addButton = new QPushButton("Добавить", central);
    connect(m_addButton, &QPushButton::clicked, this, [this]() {
        logMsg("[LOG] Нажата кнопка «Добавить»");
        onAddGame();
    });
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
    connect(m_clearButton, &QPushButton::clicked, this, [this]() {
        logMsg("[LOG] Нажата кнопка «Очистить»");
        onClearForm();
    });
    btnLayout->addWidget(m_clearButton);


    rootLayout->addLayout(btnLayout);
}

void MainWindow::setupMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("&Файл");

    QAction *newAction = fileMenu->addAction("&Добавить игру");
    newAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(newAction, &QAction::triggered, this, [this]() {
        logMsg("[LOG] Меню: Файл → Добавить игру");
        onAddGame();
    });

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
        QMessageBox::about(this, "О программе",
                           "Каталог настольных игр\n"
                           "Сквозной проект. Этап 2.\n"
                           "Вариант 16.");
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
    statusBar()->showMessage("Готово. Введите данные игры и нажмите «Добавить».");
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

    QString msg = QString("Игра «%1» добавлена (%2 игроков, %3 мин, %4).")
                      .arg(m_titleEdit->text().trimmed())
                      .arg(m_playersSpin->value())
                      .arg(m_timeSpin->value())
                      .arg(m_difficultyCombo->currentText());

    logMsg("[LOG] " + msg);
    statusBar()->showMessage(msg, 5000);
}

void MainWindow::onClearForm()
{
    m_titleEdit->clear();
    m_playersSpin->setValue(2);
    m_timeSpin->setValue(60);
    m_difficultyCombo->setCurrentIndex(0);
    m_descriptionEdit->clear();

    logMsg("[LOG] Форма очищена");
    statusBar()->showMessage("Форма очищена.", 3000);
}

void MainWindow::onAutosave()
{
    QString time = QTime::currentTime().toString("HH:mm:ss");
    logMsg(QString("[AUTOSAVE] Данные сохранены в %1").arg(time));
    statusBar()->showMessage(QString("Автосохранение выполнено в %1").arg(time), 3000);
}

void MainWindow::onSave()
{
    logMsg("[LOG] Сохранение данных вручную");
    statusBar()->showMessage("Данные успешно сохранены.", 3000);
}

void MainWindow::onDelete()
{
    logMsg("[LOG] Удаление записи");
    QMessageBox::information(this, "Удаление", "Функция удаления будет реализована на этапе 2.");
}

void MainWindow::onSettings()
{
    logMsg("[LOG] Открытие настроек");
    QMessageBox::information(this, "Настройки", "Окно настроек будет добавлено позже.");
}

void MainWindow::onExit()
{
    logMsg("[LOG] Приложение завершено пользователем");
    QApplication::quit();
}