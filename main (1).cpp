#include <fstream>
#include <iostream>
using namespace std;

// constants
const int RMAX = 10;
const int CMAX = 10;

// enumerations
enum Direction { LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4 };
enum Level { BEGIN = 4, INTERMEDIATE = 7, ADVANCED = 10 };
enum Grid { LEFTBAFFLE, RIGHTBAFFLE, EMPTY, LEFTFOUND, RIGHTFOUND };

// prototypes
void printBoard(Grid[][CMAX], bool);
void printScore(int, int);
void setBoard(Grid[][CMAX], Level);
int trackLaser(Grid[][CMAX], int, int, Direction);
bool makeGuess(Grid[][CMAX], int, int, Direction);

void Welcome();
int Menu(Grid[][CMAX], bool);
void findLevel(int&, Level&);
void getHighScore();
void setHighScore(int);

int main() {

  // welcome
  srand(time(0));
  Welcome();
  getHighScore();

  // find difficulty
  int dif;
  Level gameLevel;
  findLevel(dif, gameLevel);

  // clear the console
  cout << "\033c";

  // variables
  Grid board[RMAX][CMAX];
  bool play = true;
  bool foundOnly = true;
  int choice;
  int numLaserShot = 0;
  int numGuess = 0;
  int entryNum;
  int exitNum;
  int laserR;
  int laserC;
  Direction dir;
  int foundBafflesCounter = 0;
  int allTrueCounter = 0;
  bool foundBaffles[dif];

  // populate foundBaffles with false values
  // these will become true once the player finds them
  for (int i = 0; i < dif; i++)
    foundBaffles[i] = false;

  // fill array
  setBoard(board, gameLevel);

  // print board
  printBoard(board, foundOnly);

  while (play) {
    choice = Menu(board, foundOnly);

    // choose option
    if (choice == 1) {

      // add 1 to numlasershots
      numLaserShot += 1;

      // determine x, y & direction
      cin >> entryNum;

      // validate
      while (entryNum < 0 || entryNum > 39) {
        cout << "Try again >> ";
        cin >> entryNum;
        cout << endl;
      }

      // determine laserR, laserC, direction
      // get values of the array
      if (entryNum >= 0 && entryNum <= 9) {
        laserR = 9 - entryNum;
        laserC = 0;
        dir = RIGHT;
      } else if (entryNum >= 10 && entryNum <= 19) {
        laserR = 0;
        laserC = entryNum - 10;
        dir = DOWN;
      } else if (entryNum >= 20 && entryNum <= 29) {
        laserR = entryNum - 20;
        laserC = 9;
        dir = LEFT;
      } else if (entryNum >= 30 && entryNum <= 39) {
        laserR = 9;
        laserC = 39 - entryNum;
        dir = UP;
      }

      // track laser
      exitNum = trackLaser(board, laserR, laserC, dir);
      cout << "Laser #" << entryNum << " exited at #" << exitNum << endl;
    } else if (choice == 2) {

      // reset allTrueCounter to 0
      allTrueCounter = 0;

      // add 1 to num guesses
      numGuess += 1;

      // variables
      int guessX = 0;
      int guessY = 0;
      int guessR = 0;
      int guessC = 0;
      int directionInt;
      Direction directionGuess;

      // input two coordinates -- cannot be from same side

      cout << "Row (0-9) | Column (10-19) | 1 = Left [\\], 2 = Right [/]"
           << endl;
      cin >> guessX >> guessY >> directionInt;

      // make sure not on same side
      while ((guessX <= 9 && guessX >= 0) && (guessY <= 9 && guessX >= 0) ||
             (guessX <= 19 && guessX >= 10) && (guessY <= 19 && guessX >= 10) ||
             (guessX <= 29 && guessX >= 20) && (guessY <= 29 && guessX >= 20) ||
             (guessX <= 39 && guessX >= 30) && (guessY <= 39 && guessX >= 30)) {
        cout << "Out of bounds | Row (0-9) | Column (10-19) | 1 = Left [\\], 2 "
                "= Right [/]";
        cin >> guessX >> guessY >> directionInt;
        cout << endl;
      }

      // valiate direction
      while (directionInt < 0 || directionInt > 2) {
        cout << "Try again >> ";
        cin >> directionInt;
        cout << endl;
      }

      // convert to array coordinates:

      // convert guessX to laserR
      if (guessX >= 0 && guessX <= 9)
        guessR = 9 - guessX;
      else if (guessX >= 20 && guessX <= 29)
        guessR = guessX - 20;

      // convert guessY to laserC
      if (guessY >= 10 && guessY <= 19)
        guessC = guessY - 10;
      else if (guessY >= 30 && guessY <= 39)
        guessC = 39 - guessY;

      // change from direction int to direction guess
      if (directionInt == 1)
        directionGuess = LEFT;
      else
        directionGuess = RIGHT;

      // figure if valid guess
      foundBaffles[foundBafflesCounter] =
          makeGuess(board, guessR, guessC, directionGuess);

      // if last value was true, add 1 to counter
      if (foundBaffles[foundBafflesCounter] == true)
        foundBafflesCounter++;

      // if all values are true, stop playing
      // win condition
      for (int i = 0; i < dif; i++)
        if (foundBaffles[i] == true)
          allTrueCounter++;

      // exit the game
      if (allTrueCounter == dif) {
        // print out board
        printBoard(board, false);

        // end game message
        cout << "\n\nCongratulations!" << endl;
        cout << "You have beat BAFFLE!\n\n" << endl;

        // print score
        printScore(numLaserShot, numGuess);

        // save score to file
        setHighScore(numLaserShot + (2 * numGuess));

        // end game
        play = false;
      }

    } else if (choice == 3) {
      printScore(numLaserShot, numGuess);
    } else if (choice == 4) {
      printBoard(board, false);
    } else if (choice == 5) {
      // clear the console
      cout << "\033c";

      printBoard(board, true);
    } else if (choice == 6) {
      cout << "\nScore discarded\nThank you for playing!\n" << endl;
      exit(0);
    }
  }

  return 0;
}
void Welcome() {
  cout << "\n\n\n";
  cout << " ____    __    ____  ____  __    ____ " << endl;
  cout << "(  _ \\  /__\\  ( ___)( ___)(  )  ( ___)" << endl;
  cout << " ) _ < /(__)\\  )__)  )__)  )(__  )__) " << endl;
  cout << "(____/(__)(__)(__)  (__)  (____)(____)" << endl;
  cout << "\n\n\n";
}
void findLevel(int& dif, Level& gameLevel) {
  
  // choices
  cout << "Choose a difficulty!" << endl;
  cout << "--------------------" << endl;
  cout << "1. Beginner" << endl;
  cout << "2. Intermediate" << endl;
  cout << "3. Advanced" << endl;

  // selection
  int choice;
  cout << "\nSelection >> ";
  cin >> choice;
  cout << endl;

  // make sure valid
  while (choice < 1 || choice > 3) {
    cout << "Try again >> ";
    cin >> choice;
    cout << endl;
  }

  // set level to enumerated value
  switch (choice) {
  case 1:
    gameLevel = BEGIN;
    dif = 4;
    break;
  case 2:
    gameLevel = INTERMEDIATE;
    dif = 7;
    break;
  case 3:
    gameLevel = ADVANCED;
    dif = 10;
    break;
  }
}
void getHighScore() {
  ifstream instream;
  instream.open("highScore.txt");
  int highScore;
  instream >> highScore;
  instream.close();

  cout << "Current high score: " << highScore << "!" << endl;
  cout << "Can you beat it?\n\n\n" << endl;
}
void setHighScore(int score) {
  // only replace if higher (really lower)
  ifstream instream;
  instream.open("highScore.txt");
  int highScore;
  instream >> highScore;
  instream.close();

  // only update if high score is higher than the new score
  if (highScore > score) {
    ofstream outstream;
    outstream.open("highScore.txt");
    highScore = score;
    outstream << score;
    outstream.close();
    cout << "New high score: " << highScore << endl;
  }
}
void setBoard(Grid board[][CMAX], Level gameLevel) {
  // find dif
  int dif = gameLevel;
  
  // set entire board to EMPTY
  for (int row = 0; row < RMAX; row++)
    for (int col = 0; col < CMAX; col++)
      board[row][col] = EMPTY;

  // create bool array if they are used
  // default is false-- later on can become true
  bool usedCoords[RMAX][CMAX];
  for (int i = 0; i < RMAX; i++)
    for (int j = 0; j < CMAX; j++)
      usedCoords[i][j] = false;

  // based on level, set random baffles
  for (int baffle = 0; baffle < dif; baffle++) {
    // generate random coordinates from 0 - 9
    int randX;
    int randY;

    // generate the coordinates
    do {
      randX = (int)rand() % RMAX;
      randY = (int)rand() % CMAX;
    } while (usedCoords[randX][randY]);

    // the coordinates are used now
    usedCoords[randX][randY] = true;

    // assign random baffle
    // 0 = LEFTBAFFLE
    // 1 = RIGHTBAFFLE
    int random = (int)rand() % 2;
    if (random == 0)
      board[randX][randY] = LEFTBAFFLE;
    else if (random == 1)
      board[randX][randY] = RIGHTBAFFLE;
  }
}
void printBoard(Grid board[][CMAX], bool foundOnly) {

  // print spaces
  cout << "\n\n";

  // print out top row of numbers
  cout << " 10 11 12 13 14 15 16 17 18 19 " << endl;
  for (int row = 0; row < RMAX; row++) {
    for (int col = 0; col < CMAX; col++) {
      // print out left column of numbers first
      if (col == 0)
        cout << 9 - row;

      // not found only
      if (!foundOnly) {
        if (board[row][col] == LEFTBAFFLE || board[row][col] == LEFTFOUND)
          cout << " \\ ";
        else if (board[row][col] == RIGHTBAFFLE ||
                 board[row][col] == RIGHTFOUND)
          cout << " / ";
        else
          cout << " - ";
      } else if (foundOnly) {
        // found only
        if (board[row][col] == LEFTFOUND)
          cout << " \\ ";
        else if (board[row][col] == RIGHTFOUND)
          cout << " / ";
        else
          cout << " - ";
      }

      // print out right column of numbers last
      if (col == 9)
        cout << 20 + row;
    }
    // print spaces
    cout << endl;
  }

  // print out bottom row of numbers
  cout << " 39 38 37 36 35 34 33 32 31 30 " << endl;
  cout << "\n\n";
}
int Menu(Grid board[][CMAX], bool foundOnly) {
  // vars
  int choice;

  // print out menu
  cout << "\n";
  cout << "                        Menu                " << endl;
  cout << "--------------------------------------------------" << endl;
  cout << "1. Laser shot            |  2. Guess" << endl;
  cout << "3. Score                 |  4. Print [All baffles]" << endl;
  cout << "5. Print [Found baffles] |  6. Quit" << endl;

  // input
  cout << "\nSelection >> ";
  cin >> choice;
  cout << endl;

  // validate
  while (choice < 1 || choice > 6) {
    cout << "Try again >> ";
    cin >> choice;
    cout << endl;
  }

  return choice;
}
int trackLaser(Grid board[][CMAX], int laserR, int laserC, Direction dir) {

  // determine base case
  // base case is reached when the laser is travelling out of bounds
  // the numbers being inputted are values of an array
  // change them into number 0-39
  if (laserR < 0) { // going out of bounds 10-19
    return 10 + laserC;
  } else if (laserR > 9) { // going out of bounds 39 - 30
    return 39 - laserC;
  } else if (laserC < 0) { // going out of bounds 0 - 9
    return 9 - laserR;
  } else if (laserC > 9) { // going out of bounds 20 - 29
    return 20 + laserR;
  }

  // determine new direction
  if (board[laserR][laserC] == LEFTBAFFLE ||
      board[laserR][laserC] == LEFTFOUND) {
    // left baffle: \ that is a left baffle
    if (dir == RIGHT)
      return trackLaser(board, laserR + 1, laserC, DOWN);
    else if (dir == DOWN)
      return trackLaser(board, laserR, laserC + 1, RIGHT);
    else if (dir == LEFT)
      return trackLaser(board, laserR - 1, laserC, UP);
    else if (dir == UP)
      return trackLaser(board, laserR, laserC - 1, LEFT);
  } else if (board[laserR][laserC] == RIGHTBAFFLE ||
             board[laserR][laserC] == RIGHTFOUND) {
    // right baffle: / that is a right baffle
    if (dir == RIGHT)
      return trackLaser(board, laserR - 1, laserC, UP);
    else if (dir == DOWN)
      return trackLaser(board, laserR, laserC - 1, LEFT);
    else if (dir == LEFT)
      return trackLaser(board, laserR + 1, laserC, DOWN);
    else if (dir == UP)
      return trackLaser(board, laserR, laserC + 1, RIGHT);
  } else {
    // keep going in same direction
    if (dir == RIGHT)
      return trackLaser(board, laserR, laserC + 1, dir);
    else if (dir == DOWN)
      return trackLaser(board, laserR + 1, laserC, dir);
    else if (dir == LEFT)
      return trackLaser(board, laserR, laserC - 1, dir);
    else if (dir == UP)
      return trackLaser(board, laserR - 1, laserC, dir);
  }
  // this should never happen
  // makes the compiler happy :)
  return -1;
}
bool makeGuess(Grid board[][CMAX], int guessR, int guessC, Direction dir) {

  // return value if guessed value is a baffle
  if (dir == RIGHT && board[guessR][guessC] == RIGHTBAFFLE) {
    // print direction first
    if (dir == LEFT)
      cout << "Left";
    else if (dir == RIGHT)
      cout << "Right";

    board[guessR][guessC] = RIGHTFOUND;
    cout << " baffle found at row " << guessR << ", column " << guessC << "!"
         << endl;
    return true;
  } else if (dir == LEFT && board[guessR][guessC] == LEFTBAFFLE) {
    // print direction first
    if (dir == LEFT)
      cout << "Left";
    else if (dir == RIGHT)
      cout << "Right";

    board[guessR][guessC] = LEFTFOUND;
    cout << " baffle found at row " << guessR << ", column " << guessC << "!"
         << endl;
    return true;
  } else {
    cout << "Incorrect guess!" << endl;
    return false;
  }
}
void printScore(int numLaserShot, int numGuess) {
  cout << "\n";
  cout << "Current score!" << endl;
  cout << "--------------" << endl;
  cout << "Number of shots:   " << numLaserShot << endl;
  cout << "Number of guesses: " << numGuess << endl;
  cout << "Current score:     " << (numLaserShot) + (2 * numGuess) << endl;
  cout << "\n" << endl;
}