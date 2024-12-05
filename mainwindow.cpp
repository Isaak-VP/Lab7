#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , taskId(1) // set one for first id
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::on_closeButton_clicked);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableView->sortByColumn(3, Qt::AscendingOrder); // Індекс для рядка дати, для відображення в правльному порядку
    ui->tableView->verticalHeader()->setDefaultSectionSize(35);

    // сейв файлів коли закривається прога
    connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::saveToCsv);

    connect(ui->DeleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);




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
    ui->tableView->setSortingEnabled(true);


    if (name.isEmpty() || description.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Будь ласка, заповніть усі поля!");
        return;
    }

    // Додавання нового запису
    QList<QStandardItem *> row;
    row << new QStandardItem(QString::number(taskId++));                                // ID
    row << new QStandardItem(name);                                                     // Назва
    row << new QStandardItem(description);                                              // Опис
    //перписна дата
    QStandardItem *deadlineItem = new QStandardItem(deadline.toString("yyyy-MM-dd"));
    deadlineItem->setData(deadline, Qt::UserRole);                                      // Дата під сортування
    row << deadlineItem;

    row << new QStandardItem("Не виконано");                                            // Статус



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

void MainWindow::onDeleteButtonClicked() {
    // Отримуємо вибрані рядки
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Виберіть задачу для видалення!");
        return;
    }

    // Видаляємо кожен обраний рядок
    for (const QModelIndex &index : selected) {
        model->removeRow(index.row());
    }
}





    //Читаєм з файлу
void MainWindow::saveToCsv() {
    QString fileName = "tasks.csv";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Помилка", "Не вдалося зберегти файл.");
        return;
    }

    QTextStream out(&file);

    // Запис заголовків стовпців
    QStringList headers;
    for (int i = 0; i < model->columnCount(); ++i) {
        headers << model->headerData(i, Qt::Horizontal).toString();
    }
    out << headers.join(";") << "\n";

    // Запис даних з моделі
    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList rowData;
        for (int column = 0; column < model->columnCount(); ++column) {
            QModelIndex index = model->index(row, column);
            rowData << QVariant(model->data(index)).toString();
        }
        out << rowData.join(";") << "\n";
    }

    file.close();
    QMessageBox::information(this, "Успішно", "Дані збережено у файл tasks.csv.");
}





//commit Start3

void MainWindow::loadFromCsv()
{
    QString fileName = "tasks.csv";

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Помилка", "Не вдалося відкрити файл.");
        return;
    }

    QTextStream in(&file);

    // Пропускаємо заголовок
    in.readLine();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(';'); // Розділяємо дані за роздільником ';'

        if (fields.size() == model->columnCount()) {
            QList<QStandardItem*> row;
            for (int i = 0; i < fields.size(); ++i) {
                row << new QStandardItem(fields[i]);
            }
            model->appendRow(row);
        }
    }

    file.close();
}




void MainWindow::on_closeButton_clicked()
{
    saveToCsv();
    close();
}


void MainWindow::on_pushButton_clicked()
{
    loadFromCsv();
}

