#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>

// для файлів
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <Qstring>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//new slots for buttons

private slots:
    void onAddButtonClicked();
    void onMarkDoneButtonClicked();
     void saveToCsv();
    void loadFromCsv();

     void on_pushButton_clicked();
   void  on_closeButton_clicked();
   void onDeleteButtonClicked();








 private:


    Ui::MainWindow *ui;

     // Simple counter, model for tabelview
    QStandardItemModel *model;
    int taskId;

};
#endif // MAINWINDOW_H
