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
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //ui->tableWidget->resize(1500,1200);

   ui->table->setRowCount(20);
   ui->table->setColumnCount(30);



   int columnCount = ui->table->columnCount();
   int rowCount = ui->table->rowCount();

   for(int i = 0; i < ui->table->rowCount();++i) {
       ui->table->setRowHeight(i, 30);
   }
   for(int i = 0; i < ui->table->columnCount();++i) {
       ui->table->setColumnWidth(i, 30);
   }





   for(int i = 0; i < ui->table->rowCount(); ++i) {
       for(int j = 0; j < ui->table->columnCount(); ++j) {
           ui->table->setItem(i, j, new QTableWidgetItem);
       }
   }



  // ui->tableWidget->setItem(8, 0, new QTableWidgetItem);
  // ui->tableWidget->item(8, 0)->setBackground(Qt::red);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::getTableFromPattern(std::string patternFileName) {
    std::ifstream patternStream(patternFileName);
    size_t rowCount, columnCount;

    patternStream >> rowCount >> columnCount;


    for(int i = 0; i < ui->table->rowCount(); ++i) {
        for(int j = 0; j < ui->table->columnCount(); ++j) {
            ui->table->setItem(i, j, new QTableWidgetItem);
        }
    }



    std::cout << rowCount << " " << columnCount << '\n';
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

    //for(auto it: neighbourCells) std::cout << it.first << " " << it.second << '\n';
    neighbourCells.erase(std::remove_if(neighbourCells.begin(), neighbourCells.end(),
                [](std::pair<int,int> element) {
                    return element.first < 0 || element.first >= 20
                        || element.second < 0 || element.second >= 30;
                }
    ), neighbourCells.end());
//    for(int i = 0; i < neighbourCells.size(); ++i) {
//        if(neighbourCells[i].first < 0 || neighbourCells[i].first >= 1000 || neighbourCells[i].second < 0 || neighbourCells[i].second >= 1000) {

//            std::cout << neighbourCells[i].first << " " << neighbourCells[i].second << '\n';
//            neighbourCells.erase(neighbourCells.begin()+i);
//        }
//    }
   // std::cout << '\n';
   // std::cout << neighbourCells.size() << '\n';

    return neighbourCells;
}

void MainWindow::execLifeGame() {

    std::cout << ui->table->rowCount() << " " << ui->table->columnCount() << '\n';

    for(int i = 0; i < 10; i++) {
        usleep(5000);
        ui->table->item(5,5)->setBackground(Qt::red);
        for(int i = 0; i < ui->table->rowCount(); ++i) {
            for(int j = 0; j < ui->table->columnCount(); ++j) {
                int countOfAliveCells = 0;
                std::vector<std::pair<int,int>> neighbours = getListOfNeighborCells(i,j);
                std::cerr << "CELL coordinates: " << i+1 << " " << j+1 << '\n';
                for(auto it: neighbours) std::cerr << it.first << " " << it.second << "\n";

                std::cerr << '\n';
                for(auto it: neighbours) {
                   // std::cout << it.first << " " << it.second << '\n';
                    if(ui->table->item(it.first, it.second)->background().color() == Qt::red) countOfAliveCells++;
                }
                std::cerr << countOfAliveCells << '\n';
                //dead cell with exactly three live neighbours becomes a live cell
                if(ui->table->item(i, j)->background().color() == Qt::white) {
                    if(countOfAliveCells == 3) ui->table->item(i, j)->setBackground(Qt::red);
                }


                else if(ui->table->item(i, j)->background().color() == Qt::red) { // if the cell is alive

                    //Any live cell with fewer than two live neighbours dies, as if by underpopulation
                    if(countOfAliveCells < 2) {ui->table->item(i, j)->setBackground(Qt::white);}

                    //Any live cell with two or three live neighbours lives on to the next generation.
                    if(countOfAliveCells == 2 || countOfAliveCells == 3) {/*nothing to do*/}

                    //Any live cell with more than three live neighbours dies, as if by overpopulation.
                    if(countOfAliveCells > 3) {ui->table->item(i, j)->setBackground(Qt::white);}

                }
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

void MainWindow::on_pushButton_clicked()
{
    execLifeGame();
}
