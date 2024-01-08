/*
 * Assignment 7: Simon Says
 * Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * SimonWindow source
 *
*/

#include "simonwindow.h"
#include "ui_simonwindow.h"
#include "Model.h"

SimonWindow::SimonWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SimonWindow)
{
    ui->setupUi(this);
    // Hide the game over label by default.
    ui->loserLabel->setVisible(false);
    ui->turnLabel->setVisible(false);
    // Set default difficulty checkbox.
    ui->easyButton->setChecked(true);
    model.onDifficultySelected(difficulty::easy);
    // Disable input colors by default.
    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);

    // connections here

    // Connect difficulty buttons
    connect(ui->easyButton, &QPushButton::clicked, this, [this](){emit difficultySelected(difficulty::easy);});
    connect(ui->mediumButton, &QPushButton::clicked, this, [this](){emit difficultySelected(difficulty::medium);});
    connect(ui->hardButton, &QPushButton::clicked, this, [this](){emit difficultySelected(difficulty::hard);});
    connect(this, &SimonWindow::difficultySelected, &model, &Model::onDifficultySelected);

    // Connect the start button to the start pressed signal.
    connect(ui->startButton, &QPushButton::clicked, this, &SimonWindow::startPressed);

    // Connect the start pressed signal to the on start pressed slot in model.
    connect(this, &SimonWindow::startPressed, &model, &Model::onStartPressed);

    // Connect the update view signal to the on update view slot.
    connect(&model, &Model::updateView, this, &SimonWindow::onUpdateView);

    // Connect color input buttons.
    connect(ui->redButton, &QPushButton::clicked, this, [this](){emit colorInput(inputColor::red);});
    connect(ui->blueButton, &QPushButton::clicked, this, [this](){emit colorInput(inputColor::blue);});
    connect(ui->redButton, &QPushButton::clicked, this, [this](){highlightButton(inputColor::red);});
    connect(ui->blueButton, &QPushButton::clicked, this, [this](){highlightButton(inputColor::blue);});

    connect(this, &SimonWindow::colorInput, &model, &Model::onColorInput);

    connect(&model, &Model::displayInstruction, this, &SimonWindow::onDisplayInstruction);

    connect(&model, &Model::updateProgressBar, this, &SimonWindow::onUpdateProgressBar);
}

SimonWindow::~SimonWindow()
{
    delete ui;
}

/**
 * @brief SimonWindow::onDisplayInstruction
 * @param color - the color to display
 * The parameter color determines which button will get
 * highlighted.
 */
void SimonWindow::onDisplayInstruction(inputColor color){
    if(color == inputColor::red){
        ui->redButton->setStyleSheet("background-color: red;"
                                      "border-style: outset;"
                                      "border-width: 5px;"
                                      "border-color: yellow;");
    }
    else if(color == inputColor::blue){
        ui->blueButton->setStyleSheet("background-color: blue;"
                                      "border-style: outset;"
                                      "border-width: 5px;"
                                      "border-color: yellow;");
    }
}

/**
 * @brief SimonWindow::onUpdateView
 * @param state - the state of the model
 * @param turn - the turn of the model
 * Based on the current game state and turn state, update the UI.
 */
void SimonWindow::onUpdateView(gameState state, turnState turn){
    // When game in progress, disable loser label, start button,
    // difficulty selectors, reset button colors.
    if(state == gameState::gameInProgress){
        ui->loserLabel->setVisible(false);
        ui->turnLabel->setVisible(true);
        ui->startButton->setEnabled(false);
        ui->easyButton->setEnabled(false);
        ui->mediumButton->setEnabled(false);
        ui->hardButton->setEnabled(false);
        ui->redButton->setStyleSheet("background-color: red");
        ui->blueButton->setStyleSheet("background-color: blue");
        // If it is the players turn, enable the input buttons.
        if(turn == turnState::playerTurn){
            ui->redButton->setEnabled(true);
            ui->blueButton->setEnabled(true);
            ui->turnLabel->setText("Player Turn");
        }
        // otherwise it is the CPU turn, disable them.
        else{
            ui->redButton->setEnabled(false);
            ui->blueButton->setEnabled(false);
            ui->turnLabel->setText("CPU Turn");
        }
    }
    // When the game has been lost, show the game over label,
    // enable the start button.
    if(state == gameState::gameOver){
        ui->loserLabel->setVisible(true);
        ui->startButton->setEnabled(true);
        ui->turnLabel->setVisible(false);
    }
    // Otherwise the game is not started, everythign stays the same except for hide
    // the game over label.
    else{
        ui->loserLabel->setVisible(false);
    }
}

/**
 * @brief SimonWindow::onUpdateProgressBar
 * @param value - Value to add to the progress bar
 * @param reset - reset bar
 * If the reset bool is true, reset bar to 0.
 * If not, then add the value to the current value of the bar.
 */
void SimonWindow::onUpdateProgressBar(float value, bool reset){
    if(reset){
        ui->progressBar->setValue(0);
    }
    else{
        ui->progressBar->setValue((int)value + ui->progressBar->value());
    }
}

/**
 * @brief SimonWindow::highlightButton
 * @param color - color of input button
 * Highlights the button pressed in green then resets after a certain time.
 */
void SimonWindow::highlightButton(inputColor color){
    if(color == inputColor::red){
        ui->redButton->setStyleSheet("background-color: red;"
                                     "border-style: outset;"
                                     "border-width: 5px;"
                                     "border-color: green;");
        timer.singleShot(buttonHighlightTime, this, [this](){ui->redButton->setStyleSheet("background-color: red");});
    }
    else if(color == inputColor::blue){
        ui->blueButton->setStyleSheet("background-color: blue;"
                                      "border-style: outset;"
                                      "border-width: 5px;"
                                      "border-color: green;");
        timer.singleShot(buttonHighlightTime, this, [this](){ui->blueButton->setStyleSheet("background-color: blue");});
    }
}

