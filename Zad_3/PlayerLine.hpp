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
         // out vec4 vcolor;
         layout(location = 4) uniform mat4 trans;
         
         void main(void) {
            // const vec4 colors[] = vec4[2](vec4(1.0, 0.0, 0.0, 1.0),
            //                               vec4(0.0, 1.0, 0.0, 1.0));
            
            vec2 p = (pos * scale + center);
            gl_Position = trans * vec4(p, 0.0, 1.0);
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
    void draw(float tx, float ty, float angle)
    {
        bindProgram();
        bindBuffers();
        glUniform1f(0, 1.0 / 16); // scale  in vertex shader
        glUniform2f(1, tx, ty);   // center in vertex shader
        glUniform3f(3, cross_color[0], cross_color[1], cross_color[2]);
        // glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f))));

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(
            trans,                        //model
            angle + glm::radians(0.0f),   //angle in radians (angle in degrees deprecated)
            glm::vec3(0.0f, 0.0f, 1.0f)); //axis of rotation (vector of rotation?)

        // // glm::vec4 result = trans * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        // // printf("%f, %f, %f\n", result.x, result.y, result.z);

        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(trans));

        glDrawArrays(GL_LINES, 0, 2);
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