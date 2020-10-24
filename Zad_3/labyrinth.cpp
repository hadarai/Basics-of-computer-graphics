
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

class BackgroundSqare : public AGLDrawable
{
public:
   BackgroundSqare() : AGLDrawable(0)
   {
      setShaders();
   }
   void setShaders()
   {
      compileShaders(R"END(

         #version 330 
         out vec4 vcolor;
         void main(void) {
            // const vec2 vertices[3] = // to sa wspolrzedne wierzcholkow trojkata
            //    vec2[3](vec2( 0.9, -0.9), 
            //            vec2(-0.9, -0.9),
            //            vec2( 0.9,  0.9));

            const vec2 vertices[4] = // to sa wspolrzedne wierzcholkow trojkata
               vec2[4](vec2(-0.9, -0.9),
                       vec2( 0.9, -0.9),                       
                       vec2(-0.9,  0.9),
                       vec2( 0.9,  0.9));

            // const vec4 colors[] = // a to jakies wspolrzene kolorow?
            //    vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
            //            vec4(0.0, 1.0, 0.0, 1.0),
            //            vec4(0.0, 0.0, 1.0, 1.0));

            const vec4 colors[4] = // a to jakies wspolrzene kolorow?
               vec4[4](vec4(0.0, 0.0, 0.0, 1.0),
                       vec4(1.0, 0.0, 0.0, 1.0),
                       vec4(0.0, 0.0, 1.0, 1.0),
                       vec4(0.0, 1.0, 0.0, 1.0));

            vcolor      = colors[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END",
                     R"END(

         #version 330 
         in  vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vcolor;
         } 

      )END");
   }
   void draw()
   {
      bindProgram();
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_TRIANGLES, 1, 3);
   }
};

class PlayerLine : public AGLDrawable
{
public:
   PlayerLine() : AGLDrawable(0)
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
                            {1, 0}};

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

   PlayerLine player;
   BackgroundSqare background;

   float tx = 0.0, ty = 0.0;
   do
   {
      glClear(GL_COLOR_BUFFER_BIT);

      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      background.draw();
      player.draw(tx, ty);

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
