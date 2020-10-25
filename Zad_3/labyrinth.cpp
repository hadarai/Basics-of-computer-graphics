
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>

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
         layout(location = 1) uniform vec2  center;
         out vec4 vtex;
         // out vec4 vcolor;
         
         void main(void) {
            vec2 p = (pos * scale + center);
            gl_Position = vec4(p, 0.0, 1.0);
            // To z internetu: gl_Position = trans * vec4(position, 0.0, 1.0);
         }

      )END",
                     R"END(
         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         layout(location = 3) uniform vec3  cross_color;
         // in vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vec4(cross_color,1.0);
            
            // color = vcolor
         } 
      )END");
   }
   void setBuffers()
   {
      bindBuffers();
      float angle = 0.234;  //kat w radianach, wiec z przedziału 0.0-2.0
      GLfloat vert[4][2] = {// Cross lines
                            {(GLfloat)-cos(angle * M_PI), (GLfloat)-sin(angle * M_PI)},
                            {(GLfloat)cos(angle * M_PI), (GLfloat)sin(angle * M_PI)}};

      glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(float), vert, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
          0,        // attribute 0, must match the layout in the shader.
          2,        // size
          GL_FLOAT, // type
          GL_FALSE, // normalized?
          0,        //24,             // stride
          (void *)0 // array buffer offset
      );
   }
   void draw(float tx, float ty)
   {
      bindProgram();
      bindBuffers();
      glUniform1f(0, 1.0 / 16); // scale  in vertex shader
      glUniform2f(1, tx, ty);   // center in vertex shader
      glUniform3f(3, cross_color[0], cross_color[1], cross_color[2]);

      glDrawArrays(GL_LINES, 0, 2);
   }
   // void setColor(float r, float g, float b)
   // {
   //    cross_color[0] = r;
   //    cross_color[1] = g;
   //    cross_color[2] = b;
   // }

private:
   GLfloat cross_color[3] = {1.0, 0.0, 0.0};
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
   if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS)
   {
      ; // do something
   }
   if (key == GLFW_KEY_H && (action == GLFW_PRESS))
   {
      ; // do something
   }
}

// ==========================================================================
void MyWin::MainLoop()
{
   ViewportOne(0, 0, wd, ht);

   PlayerLine player;
   EnemyLine sample_enemy;
   BackgroundSqare background;

   float tx = 0.0, ty = 0.0, player_angle = 45.0;
   float screen_space = 1.8f;
   int amount_of_x_fragmentation = 10;
   GLfloat enemy_position_X, enemy_position_Y;

   do
   {
      glClear(GL_COLOR_BUFFER_BIT);

      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      background.draw();
      player.draw(tx, ty, player_angle);
      for (int i = 0; i < amount_of_x_fragmentation; i++)
      {
         enemy_position_X = -0.9 + (screen_space / amount_of_x_fragmentation) * i;
         enemy_position_Y = -0.9f;
         // printf("%f %f\n", enemy_position_X, enemy_position_Y);
         sample_enemy.draw(enemy_position_X, enemy_position_Y);
      }

      AGLErrors("main-afterdraw");

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();

      if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS)
      {
         tx += 0.01;
      }
      else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
      {
         tx -= 0.01;
      }
      else if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
      {
         player_angle -= 0.01;
      }
      else if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS)
      {
         player_angle += 0.01;
      }

   } while ((glfwGetKey(win(), GLFW_KEY_Q) != GLFW_PRESS &&
             glfwWindowShouldClose(win()) == 0));
}

int main(int argc, char *argv[])
{
   MyWin win;
   win.Init(800, 800, "labyrinth", 0, 33);
   win.MainLoop();
   return 0;
}
