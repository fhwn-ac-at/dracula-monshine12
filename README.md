# Nico Reinprecht - 210776 | 6th Assignment: Snakes and Ladders Simulator

## Introduction

Originating in India as *Moksha Patam*, **Snakes and Ladders** is a classic board game that relies entirely on chance through die rolls. The board features numbered squares, with players moving their pieces according to the result of a die roll.

* Some squares contain **snakes** that send players backward.
* Other squares have **ladders** that propel players forward.

The classic version of *Chutes and Ladders* published by Milton Bradley in 1943 consists of 100 squares arranged in a **10x10 grid**, includes **9 ladders and 10 chutes**, and is played with a **6-sided die** (or a spinner). Play begins off the board, and players move based on die rolls.

* Landing at the **bottom of a ladder** moves the player to the **top**.
* Landing on the **head of a snake** sends the player to its **tail**.

---

## Snakes and Ladders Simulator

You are to implement a simulator for Snakes and Ladders using **Markov Chain Monte Carlo (MCMC)** methods.

### Features

#### Game Setup

* The board should consist of an `N × M` grid of squares.
  *Default size: 10 × 10*

* The board must support an **arbitrary number** of snakes and ladders.

  * No snake or ladder should start or end on the same square as itself or any other snake or ladder.
  * No snake or ladder should start at the final square.

#### Game Rules

* The game is played with an `n`-sided die.
  *Default: 6-sided*

  * *(Optional)* Allow for **non-uniform die distributions**.

* The game is won when:

  * The last square is **reached exactly**, **or**

  * A player rolls a die that **exceeds the last square** (if allowed by rules).

  * If exact win is required, any roll that **overshoots** the final square results in **no movement**.

#### Game Simulation

* The program should simulate the game by **repeatedly rolling the die** and updating the position per game rules.
* Run the simulation for a **specified number of iterations** and collect results.

  * *(Optional)* Support **parallel simulations**.

#### Game Statistics

* The program should calculate and display:

  * Average number of rolls to win.
  * Shortest number and sequence of rolls leading to a win.
  * Number and relative frequency of each snake and ladder traversal.

---

## Specific Requirements

* Implement the board as a **graph**, where each square is a **node**.
* Support **non-interactive usage** through:

  * Command-line arguments, or
  * Configuration files
* Each simulation should **abort** after a defined number of steps (to avoid infinite loops).
* Prepare **multiple board and game configurations** with interesting properties and **compare results**.

---

## Common Requirements

* The program must be implemented in **C**.

  * Code should be well-structured into **header (.h)** and **source (.c)** files.
  * Public functions must be **well-documented**.
  * Private functions should include **internal comments** as needed.

* The code must compile with `clang` using the following flags:

  ```bash
  -Wall -Wextra -Werror
  ```

* The application **must not crash or leak memory**.

---

## Submission Guidelines

* Submit via the **provided GitHub Classroom** assignment link.
* **Late submissions** will incur a **point penalty**.
* Only include **plain-text files** in your submission:

  * **Do not** include binaries, build artifacts, or archive files.