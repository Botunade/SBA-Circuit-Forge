#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <string>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_fault_received(const std::vector<std::string>& targets);

private:
    QGraphicsScene *scene;
    QGraphicsView *view;

    void populate_mock_canvas();
};

#endif // MAINWINDOW_H