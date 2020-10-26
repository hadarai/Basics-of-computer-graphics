
class EnemyLine : public AGLDrawable
{
public:
    float x = 0.0;
    float y = 0.0;
    float angle = 0.0;

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
        //  layout(location = 0) in vec2 pos;
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
        // float angle = 0.234; //kat w radianach, wiec z przedziału 0.0-2.0
        // float some_scale = 1.7f;

        // GLfloat vert[4][2] = {{0.0, 0.0},
        //                       {0.0, 0.0}};

        // glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(float), vert, GL_STATIC_DRAW);
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(
        //     0,        // attribute 0, must match the layout in the shader.
        //     2,        // size
        //     GL_FLOAT, // type
        //     GL_FALSE, // normalized?
        //     0,        // 24,             // stride
        //     (void *)0 // array buffer offset
        // );
    }
    void draw(float tx, float ty, float rotation_angle_in_degrees)
    {
        bindProgram();
        bindBuffers();
        glUniform1f(0, 1.0 / 10); // scale  in vertex shader - tu można coś zmienić by były większe
        glUniform2f(1, tx, ty);   // line_center in vertex shader - a tu nwm co
        glUniform1f(2, rotation_angle_in_degrees);
        x = tx;
        y = ty;
        angle = rotation_angle_in_degrees;
        glDrawArrays(GL_LINES, 0, 2);
    }

private:
    // kind of something
};