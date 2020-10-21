// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage
//===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

const int N = 180;
const int r = 2;
// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
class MyTri : public AGLDrawable
{
public:
   MyTri() : AGLDrawable(0)
   {
      setShaders();
   }
   void setShaders()
   {
      compileShaders(R"END(

         #version 330 
         out vec4 vcolor;
         out vec2 pos;
         void main(void) {
            const vec2 vertices[3] = // to sa wspolrzedne wierzcholkow trojkata
               vec2[3](vec2( 0.9, -0.9), 
                       vec2(-0.9, -0.9),
                       vec2( 0.9,  0.9));

            const vec4 colors[] = // a to jakies wspolrzene kolorow?
               vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
                       vec4(0.0, 1.0, 0.0, 1.0),
                       vec4(0.0, 0.0, 1.0, 1.0));

            vcolor      = colors[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
            pos = vertices[gl_VertexID];
         }

      )END",
                     R"END(

         #version 330 
         in  vec4 vcolor;
         in  vec2 pos;
         out vec4 color;

         void main(void) {
            if((pos.x * pos.x + pos.y * pos.y) < (0.1 * 0.1))
            {
               color = vec4(1.0, 1.0, 1.0, 1.0);
            }
            else
            {
               color = vcolor;
            }
         } 

      )END");
   }
   void draw()
   {
      bindProgram();
      glDrawArrays(GL_TRIANGLES, 0, 3);
   }
};

class MyCircle : public AGLDrawable
{

public:
   MyCircle() : AGLDrawable(0)
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

         void main(void) {
            vec2 p = (pos * scale + center);
            gl_Position = vec4(p, 0.0, 1.0);x
         }
      )END",
                     R"END(
         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         layout(location = 3) uniform vec3  circle_color;
         out vec4 color;

         void main(void) {
            color = vec4(circle_color,1.0);
         } 
      )END");
   }

   void setBuffers()
   {
      bindBuffers();
      GLfloat x[N];
      GLfloat y[N];

      GLfloat circle_verts[N][2]; // circle verts;

      for (int i = 0; i < N; i++)
      {
         circle_verts[i][0] = r * cos((2 * M_PI * i / N) + 0);  //x[n] = r * cos(2 * pi * n / N)
         circle_verts[i][1] = -r * sin((2 * M_PI * i / N) + 0); //y[n] = r * sin(2 * pi * n / N);
      }

      glBufferData(GL_ARRAY_BUFFER, N * 2 * sizeof(GLfloat), circle_verts, GL_STATIC_DRAW);
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

   void draw(float tx, float ty)
   {
      bindProgram();
      bindBuffers();
      glUniform1f(0, 1.0 / 16); // scale  in vertex shader ZMIANA? rozmiar kolka
      glUniform2f(1, tx, ty);   // center in vertex shader ZMIANA?
      glUniform3f(3, circle_color[0], circle_color[1], circle_color[2]);

      glDrawArrays(GL_LINE_LOOP, 0, N);
   }
   void setColor(float r, float g, float b)
   {
      circle_color[0] = r;
      circle_color[1] = g;
      circle_color[2] = b;
   }

private:
   GLfloat circle_color[3] = {0.0, 0.0, 1.0};
};

// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment programs
// ==========================================================================
class MyCross : public AGLDrawable
{
public:
   MyCross() : AGLDrawable(0)
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

         void main(void) {
            vec2 p = (pos * scale + center);
            gl_Position = vec4(p, 0.0, 1.0);
         }

      )END",
                     R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         layout(location = 3) uniform vec3  cross_color;
         out vec4 color;

         void main(void) {
            color = vec4(cross_color,1.0);
         } 

      )END");
   }
   void setBuffers()
   {
      bindBuffers();
      GLfloat vert[4][2] = {// Cross lines
                            {-1, 0},
                            {1, 0},
                            {0, -1},
                            {0, 1}};

      glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vert, GL_STATIC_DRAW);
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

      glDrawArrays(GL_LINES, 0, 4);
   }
   void setColor(float r, float g, float b)
   {
      cross_color[0] = r;
      cross_color[1] = g;
      cross_color[2] = b;
   }

private:
   GLfloat cross_color[3] = {0.0, 1.0, 0.0};
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

   MyCross cross;
   MyTri trian;
   MyCircle circle;

   float tx = 0.0, ty = 0.0;
   float cx = 0.0, cy = 0.5;
   do
   {
      glClear(GL_COLOR_BUFFER_BIT);

      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      trian.draw();
      cross.draw(tx, ty);
      circle.draw(cx, cy);
      AGLErrors("main-afterdraw");

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();

      if (glfwGetKey(win(), GLFW_KEY_K) == GLFW_PRESS)
      {
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else if (glfwGetKey(win(), GLFW_KEY_I) == GLFW_PRESS)
      {
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
      else if (glfwGetKey(win(), GLFW_KEY_L) == GLFW_PRESS)
      {
         tx += 0.01;
      }
      else if (glfwGetKey(win(), GLFW_KEY_J) == GLFW_PRESS)
      {
         tx -= 0.01;
      }

      else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
      {
         cy -= 0.01;
      }
      else if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS)
      {
         cy += 0.01;
      }
      else if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS)
      {
         cx += 0.01;
      }
      else if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
      {
         cx -= 0.01;
      }

      if ((abs(ty - cy) * abs(ty - cy)) + (abs(tx - cx) * abs(tx - cx)) <= 0.175 * 0.175)
      {
         printf("Game over.\n");
         break;
      }

   } while ((glfwGetKey(win(), GLFW_KEY_Q) != GLFW_PRESS &&
             glfwWindowShouldClose(win()) == 0));
}

int main(int argc, char *argv[])
{
   MyWin win;
   win.Init(800, 800, "AGL3 example", 0, 33);
   win.MainLoop();
   return 0;
}
