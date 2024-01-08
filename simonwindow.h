/*
 * Assignment 7: Simon Says
 * Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * SimonWindow header
 *
*/

#ifndef SIMONWINDOW_H
#define SIMONWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include "Model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SimonWindow; }
QT_END_NAMESPACE

const int buttonHighlightTime = 150;

class SimonWindow : public QMainWindow
{
    Q_OBJECT

public:
    SimonWindow(Model& model, QWidget *parent = nullptr);
    ~SimonWindow();
    void difficultySelection(difficulty diff);
    void highlightButton(inputColor color);

private:
    Ui::SimonWindow *ui;
    QTimer timer;

public slots:
    void onUpdateView(gameState currentState, turnState currentTurn);
    void onDisplayInstruction(inputColor color);
    void onUpdateProgressBar(float value, bool reset);

signals:
    void startPressed();
    void difficultySelected(difficulty diff);
    void colorInput(inputColor color);
};
#endif // SIMONWINDOW_H
