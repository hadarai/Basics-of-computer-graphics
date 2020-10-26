
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

#include "BackgroundSquare.hpp"
#include "PlayerLine.hpp"
#include "EnemyLine.hpp"
#include "FinishLine.hpp"

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
   // void framebuffer_size_callback(GLFWwindow window, int width, int height);
   void MainLoop(); // deklaracja funkcji, ktora pozniej zostanie uzupelniona
};

// ==========================================================================
// to wlasnie jest cialo funkcji MyWin, ktora wczesniej jest zadeklarowana jako funkcja wirtualna
void MyWin::KeyCB(int key, int scancode, int action, int mods)
{
   AGLWindow::KeyCB(key, scancode, action, mods); // f-key full screen switch
   // if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS)
   // {
   //    ; // do something
   // }
   // if (key == GLFW_KEY_H && (action == GLFW_PRESS))
   // {
   //    ; // do something
   // }
}

// ==========================================================================

// void MyWin::framebuffer_size_callback(GLFWwindow window, int width, int height)
// {
//    glViewport(0, 0, width, height);
// }

void MyWin::MainLoop()
{
   ViewportOne(0, 0, wd, ht);

   GLFWwindow *window;
   // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   clock_t start;
   double duration;

   PlayerLine player;
   EnemyLine enemies[10][10]; // wrogowie na wspolrzednych 0,0 i 9,9 nie sa rysowani jak cos
   FinishLine finish;
   BackgroundSqare background;

   float player_position_x = -0.9f, player_position_y = -0.9f, player_rotation_angle = 45.0;
   float screen_space = 1.8f;
   int amount_of_columns = 10, amount_of_rows = 10;
   GLfloat enemy_position_X, enemy_position_Y;

   float player_movement = 0.005;

   float enemy_rotations[amount_of_columns][amount_of_rows];

   for (int i = 0; i < amount_of_columns; i++)
   {
      for (int j = 0; j < amount_of_rows; j++)
      {
         enemy_rotations[i][j] = rand() % 180 + 1;
      }
   }
   do
   {
      // start = clock();

      glClear(GL_COLOR_BUFFER_BIT);

      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      background.draw();
      //drawing lowest row
      player.draw(player_position_x, player_position_y, player_rotation_angle);

      for (int column = 1; column < amount_of_columns; column++)
      {
         enemy_position_X = -0.9f + (0.2) * column;
         enemy_position_Y = -0.9f;

         enemies[0][column].draw(enemy_position_X, enemy_position_Y, enemy_rotations[0][column]);
      }

      //drawing everything in the middle
      for (int row = 1; row < amount_of_rows - 1; row++)
      {
         for (int column = 0; column < amount_of_columns; column++)
         {
            enemy_position_X = -0.9f + 0.2 * column;
            enemy_position_Y = -0.9f + 0.2 * row;

            enemies[row][column].draw(enemy_position_X, enemy_position_Y, enemy_rotations[row][column]);
         }
      }
      //drawing last row
      for (int column = 0; column < amount_of_columns - 1; column++)
      {
         enemy_position_X = -0.9 + 0.2 * column;
         enemy_position_Y = 0.9f;

         enemies[amount_of_rows - 1][column].draw(enemy_position_X, enemy_position_Y, enemy_rotations[amount_of_rows - 1][column]);
      }
      finish.draw(0.9, 0.9, 45.0);

      AGLErrors("main-afterdraw");

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();

      if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS)
      {
         player_position_x += player_movement * cos(player_rotation_angle * M_PI / 180);
         player_position_y += player_movement * sin(player_rotation_angle * M_PI / 180);
      }
      else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
      {
         player_position_x -= player_movement * cos(player_rotation_angle * M_PI / 180);
         player_position_y -= player_movement * sin(player_rotation_angle * M_PI / 180);
         // player_position_x -= 0.01;
      }
      else if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
      {
         player_rotation_angle += 0.5;
      }
      else if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS)
      {
         player_rotation_angle -= 0.5;
      }

      // collision checking
      // // The main function that returns true if line segment 'p1q1'
      // // and 'p2q2' intersect.
      // bool doIntersect(Point p1, Point q1, Point p2, Point q2)
      // {
      //    // Find the four orientations needed for general and
      //    // special cases
      //    int o1 = orientation(p1, q1, p2);
      //    int o2 = orientation(p1, q1, q2);
      //    int o3 = orientation(p2, q2, p1);
      //    int o4 = orientation(p2, q2, q1);

      //    // General case
      //    if (o1 != o2 && o3 != o4)
      //       return true;

      //    // Special Cases
      //    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
      //    if (o1 == 0 && onSegment(p1, p2, q1))
      //       return true;

      //    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
      //    if (o2 == 0 && onSegment(p1, q2, q1))
      //       return true;

      //    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
      //    if (o3 == 0 && onSegment(p2, p1, q2))
      //       return true;

      //    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
      //    if (o4 == 0 && onSegment(p2, q1, q2))
      //       return true;

      //    return false; // Doesn't fall in any of the above cases
      // }

      // duration = (clock() - start) / (double)CLOCKS_PER_SEC;
      // usleep(1000 / 144 - duration);
      WaitForFixedFPS(1000.0 / 144.0);
   } while ((glfwGetKey(win(), GLFW_KEY_Q) != GLFW_PRESS &&
             glfwWindowShouldClose(win()) == 0));
}

int main(int argc, char *argv[])
{
   int seed = time(NULL);

   if (argc < 2)
   {
      printf("Assuming seed: %d\n", seed);
   }
   else
   {
      seed = atoi(argv[1]);
      printf("Given seed: %d\n", seed);
   }

   srand(seed);
   MyWin win;
   win.Init(800, 800, "labyrinth", 0, 33);
   win.MainLoop();
   return 0;
}
