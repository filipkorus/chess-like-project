#ifndef PI_2021_1615_1_BOARD_H
#define PI_2021_1615_1_BOARD_H

#include <iostream>
#include <random>
#include <algorithm>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <windows.h>
#include <ctime>
#endif

#define WHITE_BG 240
#define BLACK_BG 7
#define WHITE_BG_RED_TXT 252
#define BLACK_BG_RED_TXT 12
#define RESET_BG 0

struct Position;

class Board {
private:
   //         y  x
   char board[8][8] = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

private:
   Position getFigurePosition(char); // jako argument przyjmuje char (typ figury), zwraca jej pozycje jako struct typu Position
   bool isPositionOnBoard(Position); // jako argument przyjmuje pozycje na planszy (structa) [np C4 -> position = { 2, 3 }] i zwraca boola czy pozycja znajduje sie na planszy
   void placeFigureAtNewPosition(Position, char); // jako argument przyjmuje pozycje na planszy (structa) [np C4 -> position = { 2, 3 }] oraz typ figury, zmienia pozycje figury na planszy na zadanie pole i czysci poprzednie
   bool isPositionTaken(Position); // jako argument przyjmuje pozycje na planszy (structa) [np C4 -> position = { 2, 3 }] i zwraca prawda/falsz
   static bool sortComparator(Position const&, Position const&); // komparator do sortowania pozycji
   Position convertPosition(std::string); // konwertuje string na structa typu Position [np B6 -> position = { 1, 5 }] aby ulatwic poruszanie sie po tablicy
   Position* getKnightMoves(int&); // zwraca wskaznik na tablice typu Position zawierajaca dostepne ruchy dla konia
   Position* getBishopMoves(int&, char); // zwraca wskaznik na tablice typu Position zawierajaca dostepne ruchy dla gonca
   Position* getRookMoves(int&, char); // zwraca wskaznik na tablice typu Position zawierajaca dostepne ruchy dla wiezy
   Position* getKingMoves(int&); // zwraca wskaznik na tablice typu Position zawierajaca dostepne ruchy dla krola
   Position* getQueenMoves(int&); // zwraca wskaznik na tablice typu Position zawierajaca dostepne ruchy dla hetmana

public:
   Board(void);
   ~Board(void);
   void print(void); // wyswietla zawartosc planszy
   void clearX(void); // czysci plansze z x
   void placeFigureAtRandomPosition(char); // jako argument przyjmuje char (typ figury), umiejscawia figure na planszy w losowym (niezajetym) miejscu
   void movePawn(void); // po sprawdzeniu dozwolonego ruchu przenosi pionek na odpowiednie pole
   void printMoves(char); // przyjmuje chara (typ figury) i wyswietla dozwolone dla niej ruchy (UWAGA: funkcja nie jest przeznaczona dla figury pionek)
   void moveFigure(char, std::string); // przyjmuje chara (typ figury) oraz wspolrzedne w postaci stringa (np A5) i po sprawdzeniu dozwolonych ruchow przenosi figure na to pole (UWAGA: dla pionka istnieje osobna funkcja movePawn())
};

#endif //PI_2021_1615_1_BOARD_H
