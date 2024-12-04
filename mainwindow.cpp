#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , taskId(1) // set one for first id
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);


    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"ID", "Назва", "Опис", "Дедлайн", "Статус"});
    ui->tableView->setModel(model);

    // connect signals to buttons
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(ui->markDoneButton, &QPushButton::clicked, this, &MainWindow::onMarkDoneButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// plase for new methods!!!

    // Оголошені і реалізовані !!! (Зважані та щасливі)

void MainWindow::onAddButtonClicked() {
    // Формат
    QString name = ui->lineEditName->text();
    QString description = ui->textEditDescription->toPlainText();
    QDate deadline = ui->dateEditDeadline->date();

    if (name.isEmpty() || description.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Будь ласка, заповніть усі поля!");
        return;
    }

    // Додавання нового запису
    QList<QStandardItem *> row;
    row << new QStandardItem(QString::number(taskId++));        // ID
    row << new QStandardItem(name);                             // Назва
    row << new QStandardItem(description);                      // Опис
    row << new QStandardItem(deadline.toString("yyyy-MM-dd"));  // Дедлайн
    row << new QStandardItem("Не виконано");                    // Статус

    model->appendRow(row);

    // Клінінг після додавання нового таска
    ui->lineEditName->clear();
    ui->textEditDescription->clear();
    ui->dateEditDeadline->setDate(QDate::currentDate());
}



void MainWindow::onMarkDoneButtonClicked() {
    // Гетаєм рядок
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Виберіть задачу для зміни статусу!");
        return;
    }

    // реЧек для таблички
    for (const QModelIndex &index : selected) {
        int row = index.row();
        model->setItem(row, 4, new QStandardItem("Виконано")); // Стовпець 4 - Статус
    }
}



//commit Start
