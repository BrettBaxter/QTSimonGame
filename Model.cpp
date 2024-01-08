/*
 * Assignment 7: Simon Says
 * Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Model source
 *
*/

#include "Model.h"

/**
 * @brief Model::onStartPressed
 * Called when the start button is pressed in the UI.
 * Sets the state to in progress.
 *
 * Determines the proper variable values based on chosen difficulty.
 * Automatically starts a CPU turn.
 */
void Model::onStartPressed(){
    state = gameState::gameInProgress;
    instructionIndex = 0;
    // reset queue
    instructions = std::queue<inputColor>();
    // determine difficulty changes
    if(gameDifficulty == difficulty::easy){
        instructionCount = easyStartInstructionCount;
        difficultyMultiplier = 1;
        instructionIncrement = easyInstructionIncrement;
    }
    else if(gameDifficulty == difficulty::medium){
        instructionCount = mediumStartInstructionCount;
        difficultyMultiplier = mediumDifficultyMultiplier;
        instructionIncrement = mediumInstructionIncrement;
    }
    else if(gameDifficulty == difficulty::hard){
        instructionCount = hardStartInstructionCount;
        difficultyMultiplier = hardDifficultyMultiplier;
        instructionIncrement = hardInstructionIncrement;
    }
    // Update the UI, this should disable the start button and difficulty select.
    emit updateView(state, turn);
    // execute a CPU turn.
    handleCPUTurn();
}

/**
 * @brief Model::onDifficultySelected
 * @param diff - The new difficulty
 * Changes the difficulty if the game is not in progress.
 * Called when one of the difficulty buttons is selected.
 */
void Model::onDifficultySelected(difficulty diff){
    if(state != gameState::gameInProgress){
        gameDifficulty = diff;
    }
}

/**
 * @brief Model::onColorInput
 * @param color - The color input
 * If it is the player's turn, process the color in the parameter.
 * If the color is not at the front of the queue, the player input
 * wrong, game over.
 * If the color is correct, pop from queue.
 * If the instruction queue is empty, its the CPU's turn.
 */
void Model::onColorInput(inputColor color){
    if(turn == turnState::playerTurn){
        if(color == instructions.front()){
            instructions.pop();
            emit updateProgressBar((1/float(instructionCount-1)) * 100, false);
            if(instructions.size() == 0){
                timer.singleShot(1000, this, &Model::turnEnded);
                timer.singleShot(1000, this, [this](){emit updateProgressBar(0, true);});
            }
        }
        else{
            gameOver();
        }
    }
}

// -------------- CPU TURN METHODS ------------------

/**
 * @brief Model::handleCPUTurn
 * Starts a overall turn timer (time per instruction * instruction count)
 * Sets the turn state to CPU.
 * Loops through total instruction count and starts a timer each time.
 * Increments the instruction count for next turn.
 *
 * When the turn timer ends, the turn ended function is called and the
 * turn should switch to player.
 */
void Model::handleCPUTurn(){
    if(instructions.size() == 0){
        // handle turn changeover
        turn = turnState::cpuTurn;
        instructionIndex = 0;
        emit updateView(state, turn);
        // start overall turn timer.
        timer.singleShot(mSecondsPerInstruction * instructionCount, this, &Model::turnEnded);
        instructionLoop();
    }
}

/**
 * @brief Model::instructionLoop
 * Loop through total instruction count.
 */
void Model::instructionLoop(){
    if(turn == turnState::cpuTurn){
        if(instructionIndex >= instructionCount){
            turnEnded();
            return;
        }
        else{
            timer.singleShot(mSecondsPerInstruction/2 * difficultyMultiplier, this, &Model::displayIntervalEnded);
        }
    }
}

/**
 * @brief Model::displayIntervalEnded
 * Called each time the delay timer ends in the handleCPUTurn function.
 * This will be called for each instruction.
 *
 * Generate a random color (red or blue), then emit the signal to the
 * view to display the chosen color.
 *
 * Start another timer that will reset the view after the delay.
 */
void Model::displayIntervalEnded(){
    // generate a color
    inputColor generatedColor = generateInstruction();
    // display the color, start display timer
    emit displayInstruction(generatedColor);
    // start delay timer
    timer.singleShot(mSecondsPerInstruction/2 * difficultyMultiplier, this, &Model::delayIntervalEnded);
}

/**
 * @brief Model::delayIntervalEnded
 * Sends signal to reset highlighted color button.
 */
void Model::delayIntervalEnded(){
    emit updateView(state, turn);
    instructionIndex++;
    instructionLoop();
}

/**
 * @brief Model::generateInstruction
 * @return A random color
 * Chooses a random number between 0 and 1 then casts it to
 * the inputColor enum.
 * Pushes random color to the instruction queue.
 */
inputColor Model::generateInstruction(){
    int choiceNum = rand()%2;
    inputColor choice = static_cast<inputColor>(choiceNum);
    instructions.push(choice);
    return choice;
}

// -------------- END CPU TURN METHODS -----------------

// -------------- PLAYER TURN METHODS ------------------

/**
 * @brief Model::handlePlayerTurn
 *
 */
void Model::handlePlayerTurn(){
    turn = turnState::playerTurn;
    emit updateView(state, turn);
}

// -------------- END PLAYER TURN METHODS ---------------

// -------------- Other Methods -------------------------

/**
 * @brief Model::turnEnded
 * If cpu turn execute a player turn.
 * If player turn execute a CPU turn.
 */
void Model::turnEnded(){
    if(turn == turnState::cpuTurn){
        instructionCount += instructionIncrement;
        handlePlayerTurn();
    }
    else if (turn == turnState::playerTurn){
        handleCPUTurn();
    }
}

/**
 * @brief Model::gameOver
 * Sets state to gameOver, emits signal to update view.
 */
void Model::gameOver(){
    state = gameState::gameOver;
    emit updateView(state, turn);
}

