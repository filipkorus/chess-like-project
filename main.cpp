#include <iostream>

#include "./Board.h"

int main() {

   /*
    * S - skoczek (kon) knight
    * K - krol          king
    * P - pionek        pawn
    * W - wieza         rook
    * G - goniec        bishop [nadobowiazkowy]
    * H - hetman        queen  [nadobowiazkowy]
    */

   auto board = new Board();

   board->placeFigureAtRandomPosition('W');
   board->placeFigureAtRandomPosition('S');
   board->placeFigureAtRandomPosition('K');
   board->placeFigureAtRandomPosition('P');
   board->placeFigureAtRandomPosition('G');
   board->placeFigureAtRandomPosition('H');

   for (;;) {
      board->clearX();
      board->print();

      std::cout << "\nPodaj literke figury, ktora chcesz ruszyc: ";
      std::string figureStr;
      std::cin >> figureStr;
      char figure = toupper(figureStr[0]);

      switch (figure) {
         case 'W':
            std::cout << "Oto wszystkie dozwolone ruchy dla wiezy:\n";
            break;
         case 'S':
            std::cout << "Oto wszystkie dozwolone ruchy dla konia:\n";
            break;
         case 'K':
            std::cout << "Oto wszystkie dozwolone ruchy dla krola:\n";
            break;
         case 'G':
            std::cout << "Oto wszystkie dozwolone ruchy dla gonca:\n";
            break;
         case 'H':
            std::cout << "Oto wszystkie dozwolone ruchy dla hetmana:\n";
            break;
         case 'P':
            break;
         case 'Q':
            delete board;
            exit(0);
         default:
            std::cout << "Nieprawidlowa figura\n";
            break;
      }

      if (figure == 'P') {
         board->movePawn();
      } else {
         board->printMoves(figure);
         board->print();
         std::cout << "\nJaki ruch chcesz wykonac (podaj docelowa wspolrzedna): ";
         std::string newMove;
         std::cin >> newMove;
         board->moveFigure(figure, newMove);
      }
   }

   return 0;
}
