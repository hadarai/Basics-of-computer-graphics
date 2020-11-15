// ==========================================================================
// Labyrinth 3D game
//
// Ver.1  14.11.2020 (c) Maksymilian Czudziak
// ==========================================================================
// labyrinth3D.cpp
//===========================================================================
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
int amount_of_layers = amount_of_rows;

#include "objects/Background.hpp"

#include "objects/EnemyLine.hpp"
#include "objects/FinishLine.hpp"

class PlayerBall : public AGLDrawable
{
public:
   PlayerBall() : AGLDrawable(0)
   {
      setShaders();
      setBuffers();
   }
   void setShaders()
   {
      compileShaders(R"END(
         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) uniform float scale;
         layout(location = 1) uniform vec2  line_center;
         layout(location = 2) uniform float rotation_angle_in_degrees;
         out vec4 vtex;
         out vec4 vcolor;
         
         void main(void) {
            
            vec2 circle_vector = vec2(0.0, 0.0);
            if(gl_VertexID % 2 == 0)
            {
               // rysuje ten z przodu
               float rotation_angle_in_radians = radians(rotation_angle_in_degrees);
               circle_vector.x = cos(rotation_angle_in_radians);
               circle_vector.y = sin(rotation_angle_in_radians);
            }
            else if(gl_VertexID % 2 == 1)
            {
               // rysuje ten z tylu
               float rotation_angle_in_radians = radians(rotation_angle_in_degrees);
               circle_vector.x = -cos(rotation_angle_in_radians);
               circle_vector.y = -sin(rotation_angle_in_radians);
            }
            vec2 p = ((circle_vector * scale) + line_center);
            
            gl_Position = vec4(p, 0.0, 1.0);

            const vec4 colors[] = vec4[2](vec4(1.0, 0.0, 1.0, 1.0),
                                          vec4(0.0, 1.0, 1.0, 1.0));
            vcolor = colors[gl_VertexID];
         }

      )END",
                     R"END(
         #version 330 
         
         in vec4 vcolor;
         out vec4 color;

         void main(void) {
            
            color = vcolor;
         } 
      )END");
   }
   void setBuffers()
   {
      bindBuffers();
   }
   void draw(float x, float y, float rotation_angle_in_degrees)
   {
      bindProgram();
      bindBuffers();
      glUniform1f(0, 1.0 / amount_of_columns); // scale  in vertex shader - tu można coś zmienić by były większe
      glUniform2f(1, x, y);                    // line_center in vertex shader - a tu nwm co
      glUniform1f(2, rotation_angle_in_degrees);

      glDrawArrays(GL_LINES, 0, 2);
   }

private:
   // kind of something
};

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

   PlayerBall player;
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
            background.draw(-1.0 + j * board_pixel, -1.0 + i * board_pixel, is_black, board_pixel);
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
      printf("Assuming seed: %d\n", seed);
   }
   else
   {
      seed = atoi(argv[1]);
      printf("Given seed: %d\n", seed);
   }

   if (argc < 3)
   {
      printf("Assuming board side length: %d\n", amount_of_columns);
      amount_of_columns = 10;
      amount_of_rows = 10;
      amount_of_layers = 10;
   }
   else
   {
      amount_of_columns = atoi(argv[2]);
      printf("Given side length: %d\n", amount_of_columns);
      amount_of_rows = amount_of_columns;
   }

   srand(seed);
   MyWin win;
   win.Init(1200, 1200, "Labyrinth 3D", 0, 33);
   win.MainLoop();
   return 0;
}
