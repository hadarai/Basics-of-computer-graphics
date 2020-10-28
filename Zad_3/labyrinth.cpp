
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <unistd.h>
#include <time.h>

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int amount_of_columns = 10;
int amount_of_rows = amount_of_columns;

#include "objects/Background.hpp"
#include "objects/PlayerLine.hpp"
#include "objects/EnemyLine.hpp"
#include "objects/FinishLine.hpp"

bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
   float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
   float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
   return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
}

bool collision_check(float player_x, float player_y, float player_angle, float something_x, float something_y, float something_angle)
{
   const float scale = 1.0 / amount_of_columns;

   float player_x1 = (cos(player_angle * M_PI / 180) * scale) + player_x,
         player_y1 = (sin(player_angle * M_PI / 180) * scale) + player_y,
         player_x2 = (-cos(player_angle * M_PI / 180) * scale) + player_x,
         player_y2 = (-sin(player_angle * M_PI / 180) * scale) + player_y;

   float something_x1 = (cos(something_angle * M_PI / 180) * scale) + something_x,
         something_y1 = (sin(something_angle * M_PI / 180) * scale) + something_y,
         something_x2 = (-cos(something_angle * M_PI / 180) * scale) + something_x,
         something_y2 = (-sin(something_angle * M_PI / 180) * scale) + something_y;

   return (lineLine(player_x1, player_y1, player_x2, player_y2,
                    something_x1, something_y1, something_x2, something_y2));
}

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow // MYWin dziedziczy po AGLWindow
{
public:
   MyWin(){}; // to jest konstruktor bez argumentów
   MyWin(int _wd, int _ht, const char *name, int vers, int fullscr = 0)
       : AGLWindow(_wd, _ht, name, vers, fullscr){};
   // to jest konstruktor z argumentami, dziedziczacy po  AGLWindow
   virtual void KeyCB(int key, int scancode, int action, int mods);
   // to jest deklaracja wirtualnej (nadpisywalnej w dziedziczeniu) funkcji klasy.
   void MainLoop(); // deklaracja funkcji, ktora pozniej zostanie uzupelniona
};

// ==========================================================================
// to wlasnie jest cialo funkcji MyWin, ktora wczesniej jest zadeklarowana jako funkcja wirtualna
void MyWin::KeyCB(int key, int scancode, int action, int mods)
{
   AGLWindow::KeyCB(key, scancode, action, mods); // f-key full screen switch
}

// ==========================================================================

void MyWin::MainLoop()
{
   ViewportOne(0, 0, wd, ht);

   clock_t start;

   PlayerLine player;
   EnemyLine enemies[amount_of_columns][amount_of_rows]; // wrogowie na wspolrzednych 0,0 i 9,9 nie sa rysowani jak cos
   FinishLine finish;
   Background background;

   float board_pixel = 2.0 / amount_of_columns;
   float player_position_x = (-1.0 + board_pixel / 2), player_position_y = (-1.0 + board_pixel / 2), player_rotation_angle = 45.0;
   float finish_position_x = (1.0 - board_pixel / 2), finish_position_y = (1.0 - board_pixel / 2), finish_rotation_angle = 45.0;

   float player_movement = 0.005;

   for (int i = 0; i < amount_of_columns; i++)
   {
      for (int j = 0; j < amount_of_rows; j++)
      {
         enemies[i][j].angle_in_degrees = rand() % 180 + 1;
      }
   }
   start = glfwGetTime();
   do
   {

      glClear(GL_COLOR_BUFFER_BIT);

      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      bool is_black = true;
      for (int i = 0; i < amount_of_rows; i++)
      {
         for (int j = 0; j < amount_of_columns; j++)
         {
            background.draw(-0.9 + j * board_pixel, -0.9 + i * board_pixel, is_black);
            is_black = not is_black;
         }
         if (amount_of_columns % 2 == 0)
         {
            is_black = not is_black;
         }
      }
      //drawing lowest row
      player.draw(player_position_x, player_position_y, player_rotation_angle);

      for (int column = 1; column < amount_of_columns; column++)
      {
         enemies[0][column].x = (-1.0 + board_pixel / 2) + (board_pixel * column);
         enemies[0][column].y = (-1.0 + board_pixel / 2);
         enemies[0][column].draw();
      }

      //drawing everything in the middle
      for (int row = 1; row < amount_of_rows - 1; row++)
      {
         for (int column = 0; column < amount_of_columns; column++)
         {
            enemies[row][column].x = (-1.0 + board_pixel / 2) + board_pixel * column;
            enemies[row][column].y = (-1.0 + board_pixel / 2) + board_pixel * row;
            enemies[row][column].draw();
         }
      }
      //drawing last row
      for (int column = 0; column < amount_of_columns - 1; column++)
      {
         enemies[amount_of_rows - 1][column].x = (-1.0 + board_pixel / 2) + board_pixel * column;
         enemies[amount_of_rows - 1][column].y = (1.0 - board_pixel / 2);
         enemies[amount_of_rows - 1][column].draw();
      }
      finish.draw(finish_position_x, finish_position_y, finish_rotation_angle);

      AGLErrors("main-afterdraw");

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();

      // =====================================================   Detect events
      if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS)
      {
         player_position_x += player_movement * cos(player_rotation_angle * M_PI / 180);
         player_position_y += player_movement * sin(player_rotation_angle * M_PI / 180);
      }
      else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
      {
         player_position_x -= player_movement * cos(player_rotation_angle * M_PI / 180);
         player_position_y -= player_movement * sin(player_rotation_angle * M_PI / 180);
      }
      else if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
      {
         player_rotation_angle += 0.5;
      }
      else if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS)
      {
         player_rotation_angle -= 0.5;
      }

      // =====================================================    Detect collisions

      for (int i = 0; i < amount_of_rows; i++)
      {
         for (int j = 0; j < amount_of_columns; j++)
         {
            if (collision_check(player_position_x,
                                player_position_y,
                                player_rotation_angle,
                                enemies[i][j].x,
                                enemies[i][j].y,
                                enemies[i][j].angle_in_degrees))
            {
               printf("GAME OVER\nTime in game: %fs\n", (glfwGetTime() - start));
               exit(EXIT_FAILURE);
            }
         }
      }

      if (collision_check(player_position_x,
                          player_position_y,
                          player_rotation_angle,
                          finish_position_x,
                          finish_position_y,
                          finish_rotation_angle))
      {
         printf("CONGRATULATIONS!\nTime in game: %fs\n", (glfwGetTime() - start));
         exit(EXIT_SUCCESS);
      }

      WaitForFixedFPS(1000.0 / 60.0);
   } while ((glfwGetKey(win(), GLFW_KEY_Q) != GLFW_PRESS &&
             glfwWindowShouldClose(win()) == 0));
}

int main(int argc, char *argv[])
{
   int seed = time(NULL);

   if (argc > 3)
   {
      printf("Wrong arguments.");
      exit(EXIT_FAILURE);
   }
   if (argc < 2)
   {
      printf("Assuming board side length: %d\n", amount_of_columns);
      amount_of_columns = 10;
      amount_of_rows = 10;
   }
   else
   {
      amount_of_columns = atoi(argv[1]);
      if (amount_of_columns < 10)
      {
         printf("Minimum siede length is 10\n");
         exit(EXIT_FAILURE);
      }
      printf("Given side length: %d\n", amount_of_columns);
      amount_of_rows = amount_of_columns;
   }

   if (argc < 3)
   {
      printf("Assuming seed: %d\n", seed);
   }
   else
   {
      seed = atoi(argv[2]);
      printf("Given seed: %d\n", seed);
   }

   srand(seed);
   MyWin win;
   win.Init(1200, 1200, "labyrinth", 0, 33);
   win.MainLoop();
   return 0;
}
