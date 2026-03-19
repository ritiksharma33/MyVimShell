#include "MainWindow.h"
#include <QApplication>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QScrollBar>
#include <QProcess>
#include <QPushButton>
#include <QShortcut>
#include <QKeySequence>
#include <QFont>
#include <QDir>
#include <QEvent>
#include <QKeyEvent>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), historyIndex(-1) {
    setWindowTitle("MyVimShell");
    setMinimumSize(800, 600);

    // --- Main Layout ---
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    outputArea = new QTextEdit(this);
    outputArea->setReadOnly(true);
    outputArea->setFont(QFont("Monaco", 12));
    outputArea->append("Welcome to MyVimShell! A C++/Qt Project. Try 'ls -l'.");

    // --- Input Area Layout (Prompt + Line Edit) ---
    QWidget *inputWidget = new QWidget(this);
    QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(0, 0, 0, 0);

    promptLabel = new QLabel(this);
    promptLabel->setFont(QFont("Monaco", 12));

    inputLine = new QLineEdit(this);
    inputLine->setFont(QFont("Monaco", 12));
    inputLine->setFrame(false);
    inputLine->installEventFilter(this);

    inputLayout->addWidget(promptLabel);
    inputLayout->addWidget(inputLine);

    // --- Stop Button and Status Bar ---
    stopButton = new QPushButton("Stop Command (Ctrl+C)", this);
    stopButton->setEnabled(false);

    statusBarLabel = new QLabel("-- INSERT --");
    statusBar()->addPermanentWidget(statusBarLabel);

    // --- Assemble Layouts ---
    mainLayout->addWidget(outputArea);
    mainLayout->addWidget(inputWidget);
    mainLayout->addWidget(stopButton);
    setCentralWidget(centralWidget);

    // --- Process and Connections ---
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readProcessOutput);
    connect(process, &QProcess::readyReadStandardError, this, &MainWindow::readProcessError);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::processFinished);
    
    // This connection is the fix for the freezing issue.
    // It catches errors if a process (like a bad command) fails to start.
    connect(process, &QProcess::errorOccurred, this, &MainWindow::processError);

    connect(inputLine, &QLineEdit::returnPressed, this, &MainWindow::handleCommand);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopCurrentProcess);

    // --- Global Shortcut for stopping ---
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+C"), this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::stopCurrentProcess);

    updatePrompt();
    inputLine->setFocus();
}

void MainWindow::updatePrompt() {
    QString currentPath = QDir::currentPath();
    QString dirName = QFileInfo(currentPath).fileName();
    if (dirName.isEmpty()) dirName = "/"; // Handle root directory case
    promptLabel->setText(QString("[%1] $ ").arg(dirName));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == inputLine && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up) {
            if (!commandHistory.isEmpty()) {
                historyIndex--;
                if (historyIndex < 0) historyIndex = commandHistory.size() - 1;
                inputLine->setText(commandHistory[historyIndex]);
            }
            return true; // Event was handled
        } else if (keyEvent->key() == Qt::Key_Down) {
            if (!commandHistory.isEmpty()) {
                historyIndex++;
                if (historyIndex >= commandHistory.size()) historyIndex = 0;
                inputLine->setText(commandHistory[historyIndex]);
            }
            return true; // Event was handled
        }
    }
    // Pass the event to the base class for default processing
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::handleCommand() {
    QString commandText = inputLine->text().trimmed();
    inputLine->clear();

    if (commandText.isEmpty()) {
        return;
    }

    outputArea->append(promptLabel->text() + commandText);
    
    if (commandHistory.isEmpty() || commandHistory.last() != commandText) {
        commandHistory.append(commandText);
    }
    historyIndex = commandHistory.size();

    QStringList parts = commandText.split(" ", Qt::SkipEmptyParts);
    const QString command = parts.first();

    // --- Handle Built-in Commands ---
    bool isBuiltIn = false;
    if (command == "exit" || command == ":q") {
        QApplication::quit();
        return;
    } else if (command == "clear") {
        outputArea->clear();
        isBuiltIn = true;
    } else if (command == "cd") {
        if (parts.size() > 1) {
            QString dir = parts.mid(1).join(" ");
            if (!QDir::setCurrent(dir)) {
                outputArea->append(QString("<font color='red'>cd: no such file or directory: %1</font>").arg(dir));
            }
        } else {
            QDir::setCurrent(QDir::homePath());
        }
        isBuiltIn = true;
    }

    if (isBuiltIn) {
        // Manually call processFinished to reset the UI state for built-in commands.
        processFinished(0, QProcess::NormalExit);
        return;
    }

    // --- Handle External Commands ---
    inputLine->setDisabled(true);
    stopButton->setEnabled(true);
    statusBarLabel->setText("-- RUNNING --");
    process->start(command, parts.mid(1));
}

// This function catches errors (like command not found) and prevents the shell from freezing.
void MainWindow::processError(QProcess::ProcessError error) {
    if (error == QProcess::FailedToStart) {
        outputArea->append(QString("<font color='red'>Error: Command not found or failed to start.</font>"));
        // We must manually reset the UI state if the process never started.
        processFinished(0, QProcess::NormalExit);
    }
}

void MainWindow::stopCurrentProcess() {
    if (process->state() == QProcess::Running) {
        process->terminate();
        outputArea->append("<font color='orange'>-- Process terminated by user --</font>");
    }
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    // This is the single function responsible for resetting the UI to an idle state.
    statusBarLabel->setText("-- INSERT --");
    inputLine->setEnabled(true);
    stopButton->setEnabled(false);
    inputLine->setFocus();
    updatePrompt();
    outputArea->verticalScrollBar()->setValue(outputArea->verticalScrollBar()->maximum());
}

void MainWindow::readProcessOutput() {
    QByteArray data = process->readAllStandardOutput();
    if (!data.isEmpty()) {
        outputArea->append(QString::fromLocal8Bit(data).trimmed());
    }
}

void MainWindow::readProcessError() {
    QByteArray data = process->readAllStandardError();
    if (!data.isEmpty()) {
        outputArea->append(QString("<font color='red'>%1</font>").arg(QString::fromLocal8Bit(data).trimmed()));
    }
}