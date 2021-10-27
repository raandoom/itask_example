#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ithreadpool.h"
#include "itasklistmodel.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow* ui;

    IThreadPool* m_pool = nullptr;

    ITaskListModel* m_running_model = nullptr;
    ITaskListModel* m_finished_model = nullptr;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
