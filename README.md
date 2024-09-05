# WordChecker

## Overview

**WordChecker** is a C-based game inspired by the classic game **Mastermind**, where players attempt to guess a secret word by comparing it to a reference word. The project implements fundamental data structures and algorithms (DSA) to efficiently handle word sets and process comparisons.

Players receive feedback on whether characters in their guessed word are:
- Exactly correct (right character in the right position).
- Present but misplaced.
- Not present in the reference word.

The project applies **efficient algorithms** and uses data structures such as **tries** to handle large sets of words and ensure fast lookups, dynamic updates, and real-time feedback processing.

## Features

- **Word comparison**: Like Mastermind, compares words and gives feedback based on position and presence of characters.
- **Game sessions**: Handles multiple game sessions where players can compare guesses with a reference word.
- **Dynamic word set management**: Supports adding new words to the set of valid guesses during the game.
- **Learned constraints**: Filters valid guesses based on feedback from previous attempts.
- **Error handling**: Provides meaningful output for invalid words (`not_exists` for guesses outside the valid set).

