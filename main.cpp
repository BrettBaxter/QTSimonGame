/*
 * Assignment 7: Simon Says
 * Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Github: BrettBaxter1
 *
 * Creative Element: For my creative element I opted to add a difficulty
 * selector. I added 3 radio buttons that you can use to select easy, medium,
 * or hard difficulty when the game is not in progress.
 * Each difficulty has variables to determine:
 *  - Starting Instruction Count
 *  - Instruction Count Increment
 *  - Time per Instruction Multiplier
 * The harder the difficulty the more instructions you start with, instructions
 * display quicker, and more instructions are added per round.
 *
 * Main
 *
*/

#include "simonwindow.h"
#include "Model.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model model;
    SimonWindow w(model);
    w.show();
    return a.exec();
}
