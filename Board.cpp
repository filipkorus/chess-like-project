#include "Board.h"

struct Position {
   int x;
   int y;
};

void text(int color) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hConsole, color == RESET_BG ? BLACK_BG : color);
#else
   switch (color) {
      case WHITE_BG:
         std::cout << "\033[1;30;47m";
         break;
      case BLACK_BG:
         std::cout << "\033[1;37;40m";
         break;
      case WHITE_BG_RED_TXT:
         std::cout << "\033[1;31;47m";
         break;
      case BLACK_BG_RED_TXT:
         std::cout << "\033[1;31;40m";
         break;
      case RESET_BG:
         std::cout << "\033[0m";
         break;
   }
#endif
}

/* private functions */

Position Board::getFigurePosition(char figure) {
   for (int y = 0; y < 8; ++y) {
      for (int x = 0; x < 8; ++x) {
         Position pos = {x, y};
         if (board[pos.y][pos.x] == figure) {
            return pos;
         }
      }
   }
}

bool Board::isPositionOnBoard(Position pos) {
   return !(pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0);
}

void Board::placeFigureAtNewPosition(Position position, char figure) {
   if (Board::isPositionTaken(position) && board[position.y][position.x] != 'x') return;
   Position previousPosition = Board::getFigurePosition(figure);
   board[previousPosition.y][previousPosition.x] = ' ';
   board[position.y][position.x] = figure;
}

bool Board::isPositionTaken(Position position) {
   return board[position.y][position.x] != ' ';
}

bool Board::sortComparator(Position const &a, Position const &b) {
   if (a.x == b.x) return a.y < b.y;
   return a.x < b.x;
}

Position Board::convertPosition(std::string place) {
   Position position = {0, 0};
   position.x = int(toupper(place[0])) - int('A');
   position.y = int(place[1]) - int('0') - 1;
   return position;
}

Position *Board::getKnightMoves(int &positionsCounter) {
   Position knightPosition = Board::getFigurePosition('S');
   auto availablePositions = new Position[64];

   for (int x = -2; x <= 2; ++x) {
      for (int y = -2; y <= 2; ++y) {
         if ((abs(x) == 1 && abs(y) == 2) ||
             (abs(x) == 2 && abs(y) == 1)) {
            Position trialPosition = {knightPosition.x + x, knightPosition.y + y};
            if (!Board::isPositionTaken(trialPosition) && Board::isPositionOnBoard(trialPosition)) {
               availablePositions[positionsCounter] = trialPosition;
               positionsCounter++;
            }
         }
      }
   }
   std::sort(availablePositions, availablePositions + positionsCounter, Board::sortComparator);
   return availablePositions;
}

Position *Board::getBishopMoves(int &positionsCounter, char figure = 'G') {
   Position bishopPosition = Board::getFigurePosition(toupper(figure));
   auto availablePositions = new Position[64];
   bool stopCheck1 = false, stopCheck2 = false, stopCheck3 = false, stopCheck4 = false;

   for (int i = 1; i <= 7; ++i) {
      if ((bishopPosition.x + i >= 0 && bishopPosition.x + i <= 7) && !stopCheck1) {
         if (bishopPosition.y + i >= 0 && bishopPosition.y + i <= 7) {
            Position trialPosition = {bishopPosition.x + i, bishopPosition.y + i};
            if (!Board::isPositionTaken(trialPosition)) {
               availablePositions[positionsCounter] = trialPosition;
               positionsCounter++;
            } else {
               stopCheck1 = true;
            }
         }
      }

      if ((bishopPosition.x - i >= 0 && bishopPosition.x - i <= 7) && !stopCheck2) {
         if (bishopPosition.y + i >= 0 && bishopPosition.y + i <= 7) {
            Position trialPosition = {bishopPosition.x - i, bishopPosition.y + i};
            if (!Board::isPositionTaken(trialPosition)) {
               availablePositions[positionsCounter] = trialPosition;
               positionsCounter++;
            } else {
               stopCheck2 = true;
            }
         }
      }

      if ((bishopPosition.x + i >= 0 && bishopPosition.x + i <= 7) && !stopCheck3) {
         if (bishopPosition.y - i >= 0 && bishopPosition.y - i <= 7) {
            Position trialPosition = {bishopPosition.x + i, bishopPosition.y - i};
            if (!Board::isPositionTaken(trialPosition)) {
               availablePositions[positionsCounter] = trialPosition;
               positionsCounter++;
            } else {
               stopCheck3 = true;
            }
         }
      }

      if ((bishopPosition.x - i >= 0 && bishopPosition.x - i <= 7) && !stopCheck4) {
         if (bishopPosition.y - i >= 0 && bishopPosition.y - i <= 7) {
            Position trialPosition = {bishopPosition.x - i, bishopPosition.y - i};
            if (!Board::isPositionTaken(trialPosition)) {
               availablePositions[positionsCounter] = trialPosition;
               positionsCounter++;
            } else {
               stopCheck4 = true;
            }
         }
      }
   }
   std::sort(availablePositions, availablePositions + positionsCounter, Board::sortComparator);
   return availablePositions;
}

Position *Board::getRookMoves(int &positionsCounter, char figure = 'W') {
   Position rookPosition = Board::getFigurePosition(toupper(figure));
   auto availablePositions = new Position[14];

   int coordinateChange = 1;
   while (true) {
      Position trialPosition = {rookPosition.x, rookPosition.y - coordinateChange};
      if (Board::isPositionOnBoard(trialPosition) && !Board::isPositionTaken(trialPosition)) {
         availablePositions[positionsCounter] = trialPosition;
         positionsCounter++;
      } else break;
      coordinateChange++;
   }
   /*
   * pętla ma wykonywać się, dopóki w kierunku 'góry planszy' pozycja jest wolna
   * oraz nie wykracza poza dostępny obszar.
   * Aktualna pozycja wieży jest bazą do tworzenia testowych pozycji, dzięki czemu wiadomo,
   * że jeśli na swojej potencjalnej drodze jakieś pole jest zajęte, ruch na pola następne również
   * jest niemożliwy.
   * Jeśli testowa pozycja spełnia wszystkie warunki, jest dodawana na kolejne miejsce w tablicy.
   *
   * Pozostałe 3 pętle działają w analogiczny sposób, odpowiednio sprawdzając pola:
   * w dół
   * w lewo
   * w prawo
   */

   coordinateChange = 1;
   while (true) {
      Position trialPosition = {rookPosition.x, rookPosition.y + coordinateChange};
      if (Board::isPositionOnBoard(trialPosition) && !Board::isPositionTaken(trialPosition)) {
         availablePositions[positionsCounter] = trialPosition;
         positionsCounter++;
      } else break;
      coordinateChange++;
   }
   coordinateChange = 1;
   while (true) {
      Position trialPosition = {rookPosition.x - coordinateChange, rookPosition.y};
      if (Board::isPositionOnBoard(trialPosition) && !Board::isPositionTaken(trialPosition)) {
         availablePositions[positionsCounter] = trialPosition;
         positionsCounter++;
      } else break;
      coordinateChange++;
   }
   coordinateChange = 1;
   while (true) {
      Position trialPosition = {rookPosition.x + coordinateChange, rookPosition.y};
      if (Board::isPositionOnBoard(trialPosition) && !Board::isPositionTaken(trialPosition)) {
         availablePositions[positionsCounter] = trialPosition;
         positionsCounter++;
      } else break;
      coordinateChange++;
   }

   std::sort(availablePositions, availablePositions + positionsCounter, Board::sortComparator);
   return availablePositions;
}

Position *Board::getKingMoves(int &positionsCounter) {
   Position kingPosition = Board::getFigurePosition('K');
   auto availablePositions = new Position[64];

   if (kingPosition.x + 1 <= 7) {
      Position trialPosition = {kingPosition.x + 1, kingPosition.y};
      if (!Board::isPositionTaken(trialPosition)) {
         availablePositions[positionsCounter] = trialPosition;
         positionsCounter++;
      }
      if (kingPosition.y + 1 <= 7) {
         Position pos = {kingPosition.x + 1, kingPosition.y + 1};
         if (!Board::isPositionTaken(pos)) {
            availablePositions[positionsCounter] = pos;
            positionsCounter++;
         }
      }
      if (kingPosition.y - 1 >= 0) {
         Position pos = {kingPosition.x + 1, kingPosition.y - 1};
         if (!Board::isPositionTaken(pos)) {
            availablePositions[positionsCounter] = pos;
            positionsCounter++;
         }
      }
   }
   if (kingPosition.y + 1 <= 7) {
      Position pos = {kingPosition.x, kingPosition.y + 1};
      if (!Board::isPositionTaken(pos)) {
         availablePositions[positionsCounter] = pos;
         positionsCounter++;
      }
   }
   if (kingPosition.y - 1 >= 0) {
      Position pos = {kingPosition.x, kingPosition.y - 1};
      if (!Board::isPositionTaken(pos)) {
         availablePositions[positionsCounter] = pos;
         positionsCounter++;
      }
   }
   if (kingPosition.x - 1 >= 0) {
      Position trialPosition = {kingPosition.x - 1, kingPosition.y};
      if (!Board::isPositionTaken(trialPosition)) {
         availablePositions[positionsCounter] = trialPosition;
         positionsCounter++;
      }
      if (kingPosition.y + 1 <= 7) {
         Position pos = {kingPosition.x - 1, kingPosition.y + 1};
         if (!Board::isPositionTaken(pos)) {
            availablePositions[positionsCounter] = pos;
            positionsCounter++;
         }
      }
      if (kingPosition.y - 1 >= 0) {
         Position pos = {kingPosition.x - 1, kingPosition.y - 1};
         if (!Board::isPositionTaken(pos)) {
            availablePositions[positionsCounter] = pos;
            positionsCounter++;
         }
      }
   }

   std::sort(availablePositions, availablePositions + positionsCounter, Board::sortComparator);
   return availablePositions;
}

Position *Board::getQueenMoves(int &positionsCounter) {
   auto availablePositions = new Position[64];
   int rookPositionsCounter = 0, bishopPositionsCounter = 0, arrayPosition = 0;

   Position *rookPositions = Board::getRookMoves(rookPositionsCounter,'H'); // wpisanie na 2 nowe tablice wszystkich mozliwych
   Position *bishopPositions = Board::getBishopMoves(bishopPositionsCounter, 'H'); // ruchow po prostej i po przekatnej
   positionsCounter = rookPositionsCounter + bishopPositionsCounter;

   for (int i = 0; i < rookPositionsCounter; i++) { // scalenie 2 poprzednich tablic w jedna
      availablePositions[arrayPosition] = rookPositions[i];
      arrayPosition++;
   }
   for (int i = 0; i < bishopPositionsCounter; i++) {
      availablePositions[arrayPosition] = bishopPositions[i];
      arrayPosition++;
   }

   delete[] rookPositions;
   delete[] bishopPositions;
   std::sort(availablePositions, availablePositions + positionsCounter, Board::sortComparator);

   return availablePositions;
}

/* public functions */

Board::Board() {}

Board::~Board() {}

void Board::print() {
   bool white = true;
   std::cout << "\n   A  B  C  D  E  F  G  H\n";
   for (int y = 0; y < 8; ++y) {
      std::cout << y + 1 << ' ';
      for (int x = 0; x < 8; ++x) {
         if (board[y][x] == 'x') {
            text(white ? WHITE_BG_RED_TXT : BLACK_BG_RED_TXT);
         } else {
            text(white ? WHITE_BG : BLACK_BG);
         }
         std::cout << ' ' << board[y][x] << ' ';
         text(RESET_BG);
         white = !white;
      }
      white = !white;
      std::cout << ' ' << y + 1 << '\n';
   }
   std::cout << "   A  B  C  D  E  F  G  H\n";
}

void Board::clearX() {
   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
         if (board[i][j] == 'x') board[i][j] = ' ';
      }
   }
}

void Board::placeFigureAtRandomPosition(char figure) {
   srand(time(0));
   Position position;
   do {
      position = {rand() % 8, rand() % 8};
   } while (Board::isPositionTaken(position));
   board[position.y][position.x] = figure;
}

void Board::movePawn() {
   Position pawnPosition = Board::getFigurePosition('P');
   Position pawnDestination = {pawnPosition.x, pawnPosition.y == 0 ? 7 : pawnPosition.y - 1};
   if (!Board::isPositionTaken(pawnDestination)) {
      Board::placeFigureAtNewPosition(pawnDestination, 'P');
   } else {
      std::cout << "brak mozliwosci ruchu\n";
   }
}

void Board::printMoves(char figure) {
   int positionsCounter = 0;
   Position *availablePositions;
   switch (figure) {
      case 'K':
         availablePositions = Board::getKingMoves(positionsCounter);
         break;
      case 'W':
         availablePositions = Board::getRookMoves(positionsCounter);
         break;
      case 'S':
         availablePositions = Board::getKnightMoves(positionsCounter);
         break;
      case 'G':
         availablePositions = Board::getBishopMoves(positionsCounter);
         break;
      case 'H':
         availablePositions = Board::getQueenMoves(positionsCounter);
         break;

      default:
         return;
   }
   if (positionsCounter == 0) {
      std::cout << "brak mozliwosci ruchu\n";
   }
   for (int i = 0; i < positionsCounter; ++i) {
      board[availablePositions[i].y][availablePositions[i].x] = 'x';
      std::cout << char(availablePositions[i].x + int('A')) << availablePositions[i].y + 1;
      if (i != positionsCounter - 1) std::cout << ", ";
      else std::cout << '\n';
   }
   delete[] availablePositions;
}

void Board::moveFigure(char figure, std::string newPosition) {
   Position figurePosition = Board::getFigurePosition(figure);
   Position figureDestination = Board::convertPosition(newPosition);

   if (!Board::isPositionOnBoard(figureDestination)) {
      std::cout << "wyjscie poza plansze\n";
      return;
   }
   if (board[figureDestination.y][figureDestination.x] == 'x') {
      Board::placeFigureAtNewPosition(figureDestination, figure);
   } else {
      std::cout << "niedozwolony ruch\n";
   }
}
