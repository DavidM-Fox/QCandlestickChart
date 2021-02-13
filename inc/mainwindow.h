#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QApplication>
#include <QDebug>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void set_chart(std::string output_file);
};

#endif // MAINWINDOW_H