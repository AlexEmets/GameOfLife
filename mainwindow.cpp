#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include<iostream>
#include<fstream>
#include<vector>
#include <utility>
#include <chrono>
#include <thread>
#include <unistd.h>


const int rowCount = 20;
const int columnCount = 30;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    needToStop(false)
{
    ui->setupUi(this);

   ui->table->setRowCount(rowCount);
   ui->table->setColumnCount(columnCount);

   for(int i = 0; i < rowCount;++i) {
       ui->table->setRowHeight(i, 30);
   }
   for(int i = 0; i < columnCount;++i) {
       ui->table->setColumnWidth(i, 30);
   }


   for(int i = 0; i < rowCount; ++i) {
       for(int j = 0; j < columnCount; ++j) {
           ui->table->setItem(i, j, new QTableWidgetItem);
           ui->table->item(i, j)->setBackground(Qt::white);
       }
   }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::getTableFromPattern(std::string patternFileName) {
    std::ifstream patternStream(patternFileName);

    for(int i = 0; i < rowCount; ++i) {
        for(int j = 0; j < columnCount; ++j) {
            ui->table->setItem(i, j, new QTableWidgetItem);
        }
    }

    for(int i = 0; i < rowCount; ++i) {
        for(int j = 0; j < columnCount; ++j) {
            char character;  // . or O
            patternStream >> character;
            if(character == 'O') {
                ui->table->item(i, j)->setBackground(Qt::red);
            }
            else if (character=='.') {
                ui->table->item(i, j)->setBackground(Qt::white);
            }
        }
    }
}


std::vector<std::pair<int, int> > getListOfNeighborCells(int i, int j) {
    std::vector<std::pair<int, int> > neighbourCells;
    neighbourCells.push_back({i,j-1});
    neighbourCells.push_back({i,j+1});
    neighbourCells.push_back({i-1,j-1});
    neighbourCells.push_back({i-1,j+1});
    neighbourCells.push_back({i+1,j-1});
    neighbourCells.push_back({i+1,j+1});
    neighbourCells.push_back({i-1,j});
    neighbourCells.push_back({i+1,j});

    neighbourCells.erase(std::remove_if(neighbourCells.begin(), neighbourCells.end(),
                [](std::pair<int,int> element) {
                    return element.first < 0 || element.first >= rowCount
                        || element.second < 0 || element.second >= 30;
                }
    ), neighbourCells.end());

    return neighbourCells;
}

void MainWindow::on_stopGameButton_clicked()
{
    needToStop=true;
}

void MainWindow::execLifeGame() {

    needToStop = false;

    bool isCellAlive[rowCount][columnCount];
    for(int i = 0; i < rowCount; ++i) {
        for(int j = 0; j < columnCount; ++j) {
            if(ui->table->item(i, j)->background().color() == Qt::white) isCellAlive[i][j] = false;
            else isCellAlive[i][j] = true;
        }
    }



    for(int i = 0; i < 100; i++) {

        bool isTableChanging = false;

        for(int i = 0; i < rowCount; ++i) {
            for(int j = 0; j < columnCount; ++j) { 
                ui->table->repaint();
                if(needToStop) return;

                int countOfAliveCells = 0;
                std::vector<std::pair<int,int>> neighbours = getListOfNeighborCells(i,j);

                auto cellColor = ui->table->item(i, j)->background().color();

                for(auto it: neighbours)
                    if(ui->table->item(it.first, it.second)->background().color() == Qt::red) countOfAliveCells++;

                if(cellColor == Qt::white) {
                    if(countOfAliveCells == 3) {isCellAlive[i][j]=true; isTableChanging=true;}
                }
                else if(cellColor == Qt::red) { // if the cell is alive

                    //Any live cell with fewer than two live neighbours dies, as if by underpopulation
                    if(countOfAliveCells < 2) {
                        isCellAlive[i][j]=false;

                        isTableChanging=true;
                    }

                    //Any live cell with two or three live neighbours lives on to the next generation.
                    if(countOfAliveCells == 2 || countOfAliveCells == 3) {isCellAlive[i][j]=true;}

                    //Any live cell with more than three live neighbours dies, as if by overpopulation.
                    if(countOfAliveCells > 3) {isCellAlive[i][j]=false;isTableChanging=true;}

                }

            }

        }
        if(isTableChanging == false) break;

        for(int i = 0; i < rowCount; ++i) {
            for(int j = 0; j < columnCount; ++j) {
                if(isCellAlive[i][j] == false) ui->table->item(i, j)->setBackground(Qt::white);
                else ui->table->item(i, j)->setBackground(Qt::red);
            }
        }
    }
}

void MainWindow::on_exportPatternButton_clicked()
{

}

void MainWindow::on_table_cellClicked(int row, int column)
{
    ui->table->setItem(row, column, new QTableWidgetItem);
    ui->table->item(row, column)->setBackground(Qt::red);
}


void MainWindow::on_startGameButton_clicked()
{
    execLifeGame();
}
