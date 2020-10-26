
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

class EnemyLine : public AGLDrawable
{
public:
   EnemyLine() : AGLDrawable(0)
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
         layout(location = 0) in vec2 pos;
         layout(location = 0) uniform float scale;
         layout(location = 1) uniform vec2  line_center;
         layout(location = 2) uniform float rotation_angle_in_degrees;
         out vec4 vtex;
         out vec4 vcolor;
         
         
         void main(void) {
            
            vec2 circle_vector = vec2(0.0, 0.0);
            if(gl_VertexID%2 == 0)
            {
               // rysuje ten z przodu
               
               float rotation_angle_in_radians = radians(rotation_angle_in_degrees);
               circle_vector.x = cos(rotation_angle_in_radians);
               circle_vector.y = sin(rotation_angle_in_radians);
            }
            else if(gl_VertexID%2 == 1)
            {
               // rysuje ten z tylu
               float rotation_angle_in_radians = radians(rotation_angle_in_degrees);
               circle_vector.x = -cos(rotation_angle_in_radians);
               circle_vector.y = -sin(rotation_angle_in_radians);
            }
            
            vec2 p = ((circle_vector * scale) + line_center);
            
            gl_Position = vec4(p, 0.0, 1.0);

            const vec4 colors[] = vec4[2](vec4(1.0, 0.0, 0.0, 1.0),
                                          vec4(0.0, 1.0, 0.0, 1.0));
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
      float angle = 0.234; //kat w radianach, wiec z przedziału 0.0-2.0
      float some_scale = 1.7f;

      GLfloat vert[4][2] = {{0.0, 0.0},
                            {0.0, 0.0}};

      glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(float), vert, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
          0,        // attribute 0, must match the layout in the shader.
          2,        // size
          GL_FLOAT, // type
          GL_FALSE, // normalized?
          0,        // 24,             // stride
          (void *)0 // array buffer offset
      );
   }
   void draw(float tx, float ty, float rotation_angle_in_degrees)
   {
      bindProgram();
      bindBuffers();
      glUniform1f(0, 1.0 / 10); // scale  in vertex shader - tu można coś zmienić by były większe
      glUniform2f(1, tx, ty);   // line_center in vertex shader - a tu nwm co
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
void MyWin::MainLoop()
{
   ViewportOne(0, 0, wd, ht);
   clock_t start;
   double duration;

   PlayerLine player;
   EnemyLine enemies[10][10]; // wrogowie na wspolrzednych 0,0 i 9,9 nie sa rysowani jak cos
   BackgroundSqare background;

   float player_position_x = -0.9f, player_position_y = -0.9f, player_rotation_angle = 0.0;
   float screen_space = 1.8f;
   int amount_of_columns = 10, amount_of_rows = 10;
   GLfloat enemy_position_X, enemy_position_Y;
   float enemy_rotation = rand() % 180 + 1;
   float player_movement = 0.005;

   for (int i = 0; i < amount_of_columns; i++)
   {
      for (int j = 0; j < amount_of_rows; j++)
      {
      }
   }
   do
   {
      start = clock();

      glClear(GL_COLOR_BUFFER_BIT);

      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      background.draw();
      //drawing lowest row
      player.draw(player_position_x, player_position_y, player_rotation_angle);

      // enemies[0][1].draw(0.6, 0.6, enemy_rotation);
      for (int column = 1; column < amount_of_columns; column++)
      {
         enemy_position_X = -0.9f + (0.2) * column;
         enemy_position_Y = -0.9f;

         enemies[0][column].draw(enemy_position_X, enemy_position_Y, enemy_rotation);
      }

      //drawing everything in the middle
      for (int row = 1; row < amount_of_rows - 1; row++)
      {
         for (int column = 0; column < amount_of_columns; column++)
         {
            enemy_position_X = -0.9f + 0.2 * column;
            enemy_position_Y = -0.9f + 0.2 * row;

            enemies[row][column].draw(enemy_position_X, enemy_position_Y, enemy_rotation);
         }
      }
      //drawing last row
      for (int column = 0; column < amount_of_columns - 1; column++)
      {
         enemy_position_X = -0.9 + 0.2 * column;
         enemy_position_Y = 0.9f;

         enemies[amount_of_rows - 1][column].draw(enemy_position_X, enemy_position_Y, enemy_rotation);
      }

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
         player_rotation_angle -= 0.5;
      }
      else if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS)
      {
         player_rotation_angle += 0.5;
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

      duration = (clock() - start) / (double)CLOCKS_PER_SEC;
      usleep(1000 / 144 - duration);
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
