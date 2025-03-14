#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPixmap pixmap_;
    QLabel *imageLabel_;

private slots:
    void loadImage();
    void saveImage();
};
