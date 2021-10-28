#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "factorial.h"
#include "itasklistdelegate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("IThreadPool");
    resize(1024,480);

    m_pool = new IThreadPool(10);

    ui->runningListView->setItemDelegate(new ITaskListDelegate(ui->runningListView));
    ui->finishedListView->setItemDelegate(new ITaskListDelegate(ui->finishedListView));

    m_running_model = new ITaskListModel(ui->runningListView);
    ui->runningListView->setModel(m_running_model);

    m_finished_model = new ITaskListModel(ui->finishedListView);
    ui->finishedListView->setModel(m_finished_model);

    connect(ui->factorialAdd,&QPushButton::clicked,this,[this]() {
        const QSharedPointer<ITask>& task =
                m_pool->createTask<FactorialTask>(ui->factorialLimit->value());
        m_running_model->addTask(task);
    });
    connect(ui->factorialDoubleAdd,&QPushButton::clicked,this,[this]() {
        const QSharedPointer<ITask>& task =
                m_pool->createTask<DoubleFactorialTask>(ui->factorialDoubleLimit->value());
        m_running_model->addTask(task);
    });

    connect(m_pool,&IThreadPool::taskEnded,this,[this](const QSharedPointer<ITask>& task) {
        m_running_model->removeTask(task);
        m_finished_model->addTask(task);
    });
    connect(m_pool,&IThreadPool::taskProgress,this,[this]() {
        ui->runningListView->viewport()->update();
    });
}

MainWindow::~MainWindow()
{
    delete m_pool;
    delete ui;
}

