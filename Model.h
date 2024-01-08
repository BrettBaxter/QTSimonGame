/*
 * Assignment 7: Simon Says
 * Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Model header
 *
*/

#ifndef MODEL_H
#define MODEL_H

#include <queue>
#include <QObject>
#include <QTimer>
#include <QDebug>

// -------------- Enums and Constants ----------------------

enum class gameState { gameNotStarted, gameInProgress, gameOver };
enum class turnState { playerTurn, cpuTurn };
enum class difficulty { easy, medium, hard };
enum class inputColor { red, blue };

const int easyStartInstructionCount = 1;
const int mediumStartInstructionCount = 3;
const int hardStartInstructionCount = 5;

const int mSecondsPerInstruction = 2000;

const float mediumDifficultyMultiplier = 0.8;
const float hardDifficultyMultiplier = 0.5;

const int easyInstructionIncrement = 1;
const int mediumInstructionIncrement = 2;
const int hardInstructionIncrement = 3;

// -------------- END Enums and Constants -------------------

class Model : public QObject{
    Q_OBJECT
private:
    // Instance Variables
    gameState state;
    turnState turn;
    difficulty gameDifficulty;
    int instructionCount;
    float difficultyMultiplier;
    int instructionIncrement;
    int instructionIndex;

    // Timer Object
    QTimer timer;

    // Instruction Queue
    std::queue<inputColor> instructions;

public:
    // Constructor
    explicit Model(QObject *parent = nullptr) : state(gameState::gameNotStarted), turn(turnState::cpuTurn), gameDifficulty(difficulty::easy),
        instructionCount(1), difficultyMultiplier(1), instructionIncrement(1), instructionIndex(0){}

    // Public Methods
    inputColor generateInstruction();
    void handleCPUTurn();
    void handlePlayerTurn();
    void gameOver();
    void displayIntervalEnded();
    void delayIntervalEnded();
    void turnEnded();
    void instructionLoop();

public slots:
    void onStartPressed();
    void onDifficultySelected(difficulty diff);
    void onColorInput(inputColor color);

signals:
    void updateView(gameState currentState, turnState currentTurn);
    void displayInstruction(inputColor color);
    void updateProgressBar(float value, bool reset);
};

#endif // MODEL_H
