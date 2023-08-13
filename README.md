# Connect 4 Game

Connect 4 is a two-player connection board game, in which the players choose a color and then take turns dropping colored discs from the top into a vertically suspended grid. The pieces fall straight down, occupying the lowest available space within the column. The objective of the game is to connect four of one's own discs of the same color next to each other vertically, horizontally, or diagonally before the opponent.

## Features

- Play against another human player.
- Save and load games for later continuation.
- Choose your own player names and play as either "X" or "O".
- Game board visualization with a simple text-based interface.

## Getting Started

1. Clone the repository to your local machine.
2. Compile the source code using a C compiler (e.g., GCC).
3. Run the compiled executable to start the game.

## How to Play

1. Choose an option from the main menu:
   - Start a new game
   - Load a saved game
   - Exit the game
2. If starting a new game, enter the names for both players.
3. During the game, players take turns selecting a column to drop their piece into.
4. Connect four pieces of your color in a row, column, or diagonal to win the game.
5. The game can be saved and loaded later from the load menu.

## Game Rules

- Players take turns dropping their pieces into the grid.
- A player wins by connecting four of their pieces in a row, column, or diagonal.
- The game is a draw if the entire grid is filled without a winner.

## File Structure

- `main.c`: Contains the main game logic and functions.
- `Save_Result.txt`: Stores saved game data.
- Other header and source files: Contain various functions used in the game.

## Acknowledgments

This Connect 4 game was developed by Aleksandar Milosevic as a project. The game utilizes a simple text-based interface to provide an enjoyable gaming experience.

Feel free to contribute to the project by adding new features or improving existing ones. Enjoy playing Connect 4!

