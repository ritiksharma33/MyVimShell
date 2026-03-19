#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList> // For command history
#include <QProcess>    // Include QProcess header for ExitStatus and ProcessError

// Forward declarations to speed up compilation
class QTextEdit;
class QLineEdit;
class QLabel;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    // This function is used to intercept key presses for command history
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    // Core slots for handling commands and process I/O
    void handleCommand();
    void readProcessOutput();
    void readProcessError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void stopCurrentProcess();

    // THE FIX: Add a new slot to handle process startup errors
    void processError(QProcess::ProcessError error);

private:
    // Helper function to update the command prompt
    void updatePrompt();

    // UI Widgets
    QTextEdit *outputArea;
    QLineEdit *inputLine;
    QLabel *promptLabel;
    QLabel *statusBarLabel;
    QPushButton *stopButton;

    // Core Logic
    QProcess *process;

    // Command History
    QStringList commandHistory;
    int historyIndex;
};

#endif // MAINWINDOW_H

