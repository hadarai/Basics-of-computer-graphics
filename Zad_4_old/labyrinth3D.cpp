// ==========================================================================
// Labyrinth 3D game
//
// Ver.1  14.11.2020 (c) Maksymilian Czudziak
// ==========================================================================
// labyrinth3D.cpp
//===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include <time.h>

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// int amount_of_columns = 10;
// int amount_of_rows = amount_of_columns;
// int amount_of_layers = amount_of_rows;

// #include "objects/Background.hpp"

// #include "objects/EnemyLine.hpp"
// #include "objects/FinishLine.hpp"

class Obstacle : public AGLDrawable
{
public:
   Obstacle() : AGLDrawable(0)
   {
      setShaders();
      setBuffers();
   }
   void setShaders()
   {
      compileShaders(R"END(
         #version 330 
         layout(location = 1) in vec3 vertexColor;

         out vec3 fragmentColor;
         void main(void) {
            fragmentColor = vertexColor;
         }

      )END",
                     R"END(
         #version 330 
         
         in vec3 fragmentColor;

         out vec3 color;

         void main(){
            // Output color = color specified in the vertex shader,
            // interpolated between all 3 surrounding vertices
            color = fragmentColor;
         }
      )END");
   }
   void setBuffers()
   {
      bindBuffers();

      static const GLfloat g_vertex_buffer_data[] = {
          -1.0f, -1.0f, -1.0f, // triangle 1 : begin
          -1.0f, -1.0f, 1.0f,
          -1.0f, 1.0f, 1.0f, // triangle 1 : end
          1.0f, 1.0f, -1.0f, // triangle 2 : begin
          -1.0f, -1.0f, -1.0f,
          -1.0f, 1.0f, -1.0f, // triangle 2 : end
          1.0f, -1.0f, 1.0f,
          -1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, 1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          -1.0f, -1.0f, -1.0f,
          -1.0f, -1.0f, -1.0f,
          -1.0f, 1.0f, 1.0f,
          -1.0f, 1.0f, -1.0f,
          1.0f, -1.0f, 1.0f,
          -1.0f, -1.0f, 1.0f,
          -1.0f, -1.0f, -1.0f,
          -1.0f, 1.0f, 1.0f,
          -1.0f, -1.0f, 1.0f,
          1.0f, -1.0f, 1.0f,
          1.0f, 1.0f, 1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, 1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, 1.0f, 1.0f,
          1.0f, -1.0f, 1.0f,
          1.0f, 1.0f, 1.0f,
          1.0f, 1.0f, -1.0f,
          -1.0f, 1.0f, -1.0f,
          1.0f, 1.0f, 1.0f,
          -1.0f, 1.0f, -1.0f,
          -1.0f, 1.0f, 1.0f,
          1.0f, 1.0f, 1.0f,
          -1.0f, 1.0f, 1.0f,
          1.0f, -1.0f, 1.0f};
      static const GLfloat g_color_buffer_data[] = {
          0.583f, 0.771f, 0.014f,
          0.609f, 0.115f, 0.436f,
          0.327f, 0.483f, 0.844f,
          0.822f, 0.569f, 0.201f,
          0.435f, 0.602f, 0.223f,
          0.310f, 0.747f, 0.185f,
          0.597f, 0.770f, 0.761f,
          0.559f, 0.436f, 0.730f,
          0.359f, 0.583f, 0.152f,
          0.483f, 0.596f, 0.789f,
          0.559f, 0.861f, 0.639f,
          0.195f, 0.548f, 0.859f,
          0.014f, 0.184f, 0.576f,
          0.771f, 0.328f, 0.970f,
          0.406f, 0.615f, 0.116f,
          0.676f, 0.977f, 0.133f,
          0.971f, 0.572f, 0.833f,
          0.140f, 0.616f, 0.489f,
          0.997f, 0.513f, 0.064f,
          0.945f, 0.719f, 0.592f,
          0.543f, 0.021f, 0.978f,
          0.279f, 0.317f, 0.505f,
          0.167f, 0.620f, 0.077f,
          0.347f, 0.857f, 0.137f,
          0.055f, 0.953f, 0.042f,
          0.714f, 0.505f, 0.345f,
          0.783f, 0.290f, 0.734f,
          0.722f, 0.645f, 0.174f,
          0.302f, 0.455f, 0.848f,
          0.225f, 0.587f, 0.040f,
          0.517f, 0.713f, 0.338f,
          0.053f, 0.959f, 0.120f,
          0.393f, 0.621f, 0.362f,
          0.673f, 0.211f, 0.457f,
          0.820f, 0.883f, 0.371f,
          0.982f, 0.099f, 0.879f};

      GLuint vertexbuffer;
      // Generate 1 buffer, put the resulting identifier in vertexbuffer
      glGenBuffers(1, &vertexbuffer);
      // The following commands will talk about our 'vertexbuffer' buffer
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      // Give our vertices to OpenGL.
      glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glVertexAttribPointer(
          0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
          3,        // size
          GL_FLOAT, // type
          GL_FALSE, // normalized?
          0,        // stride
          (void *)0 // array buffer offset
      );

      GLuint colorbuffer;
      glGenBuffers(1, &colorbuffer);
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
      // 2nd attribute buffer : colors
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
      glVertexAttribPointer(
          1,        // attribute. No particular reason for 1, but must match the layout in the shader.
          3,        // size
          GL_FLOAT, // type
          GL_FALSE, // normalized?
          0,        // stride
          (void *)0 // array buffer offset
      );
   }
   void draw(float x, float y)
   {
      bindBuffers();
      bindProgram();
      // 1st attribute buffer : vertexes

      // Draw the triangle !
      glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
      }

private:
   // kind of something?
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

   Obstacle obstacle_cube;
   // EnemyLine enemies[amount_of_columns][amount_of_rows]; // wrogowie na wspolrzednych 0,0 i 9,9 nie sa rysowani jak cos
   // FinishLine finish;
   // Background background;

   // float board_pixel = 2.0 / amount_of_columns;
   // float player_position_x = (-1.0 + board_pixel / 2), player_position_y = (-1.0 + board_pixel / 2), player_rotation_angle = 45.0;
   // float finish_position_x = (1.0 - board_pixel / 2), finish_position_y = (1.0 - board_pixel / 2), finish_rotation_angle = 45.0;

   // float player_movement = 0.005;

   // for (int i = 0; i < amount_of_columns; i++)
   // {
   //    for (int j = 0; j < amount_of_rows; j++)
   //    {
   //       enemies[i][j].angle_in_degrees = rand() % 180 + 1;
   //    }
   // }
   start = glfwGetTime();
   do
   {

      glClear(GL_COLOR_BUFFER_BIT);

      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      // bool is_black = true;
      // for (int i = 0; i < amount_of_rows; i++)
      // {
      //    for (int j = 0; j < amount_of_columns; j++)
      //    {
      //       background.draw(-1.0 + j * board_pixel, -1.0 + i * board_pixel, is_black, board_pixel);
      //       is_black = not is_black;
      //    }
      //    if (amount_of_columns % 2 == 0)
      //    {
      //       is_black = not is_black;
      //    }
      // }
      //drawing lowest row
      obstacle_cube.draw(0, 0);

      // for (int column = 1; column < amount_of_columns; column++)
      // {
      //    enemies[0][column].x = (-1.0 + board_pixel / 2) + (board_pixel * column);
      //    enemies[0][column].y = (-1.0 + board_pixel / 2);
      //    enemies[0][column].draw();
      // }

      // //drawing everything in the middle
      // for (int row = 1; row < amount_of_rows - 1; row++)
      // {
      //    for (int column = 0; column < amount_of_columns; column++)
      //    {
      //       enemies[row][column].x = (-1.0 + board_pixel / 2) + board_pixel * column;
      //       enemies[row][column].y = (-1.0 + board_pixel / 2) + board_pixel * row;
      //       enemies[row][column].draw();
      //    }
      // }
      // //drawing last row
      // for (int column = 0; column < amount_of_columns - 1; column++)
      // {
      //    enemies[amount_of_rows - 1][column].x = (-1.0 + board_pixel / 2) + board_pixel * column;
      //    enemies[amount_of_rows - 1][column].y = (1.0 - board_pixel / 2);
      //    enemies[amount_of_rows - 1][column].draw();
      // }
      // finish.draw(finish_position_x, finish_position_y, finish_rotation_angle);

      AGLErrors("main-afterdraw");

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();

      // =====================================================   Detect events
      if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS)
      {
         // player_position_x += player_movement * cos(player_rotation_angle * M_PI / 180);
         // player_position_y += player_movement * sin(player_rotation_angle * M_PI / 180);
      }
      else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
      {
         // player_position_x -= player_movement * cos(player_rotation_angle * M_PI / 180);
         // player_position_y -= player_movement * sin(player_rotation_angle * M_PI / 180);
      }
      else if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
      {
         // player_rotation_angle += 0.5;
      }
      else if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS)
      {
         // player_rotation_angle -= 0.5;
      }

      WaitForFixedFPS(1000.0 / 60.0);
   } while ((glfwGetKey(win(), GLFW_KEY_Q) != GLFW_PRESS &&
             glfwWindowShouldClose(win()) == 0));
}

int main(int argc, char *argv[])
{
   int seed = time(NULL);

   // if (argc > 3)
   // {
   //    printf("Wrong arguments.");
   //    exit(EXIT_FAILURE);
   // }

   // if (argc < 2)
   // {
   //    printf("Assuming seed: %d\n", seed);
   // }
   // else
   // {
   //    seed = atoi(argv[1]);
   //    printf("Given seed: %d\n", seed);
   // }

   // if (argc < 3)
   // {
   //    printf("Assuming board side length: %d\n", amount_of_columns);
   //    amount_of_columns = 10;
   //    amount_of_rows = 10;
   //    amount_of_layers = 10;
   // }
   // else
   // {
   //    amount_of_columns = atoi(argv[2]);
   //    printf("Given side length: %d\n", amount_of_columns);
   //    amount_of_rows = amount_of_columns;
   // }

   srand(seed);
   MyWin win;
   win.Init(1200, 1200, "Labyrinth 3D", 0, 33);
   win.MainLoop();
   return 0;
}
