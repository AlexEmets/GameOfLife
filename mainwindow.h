#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getTableFromPattern(std::string patternFileName);
    void execLifeGame();
private slots:
    void on_exportPatternButton_clicked();

    void on_table_cellClicked(int row, int column);

    void on_startGameButton_clicked();

    void on_stopGameButton_clicked();


private:
    Ui::MainWindow *ui;
    bool needToStop;
};

#endif // MAINWINDOW_H
