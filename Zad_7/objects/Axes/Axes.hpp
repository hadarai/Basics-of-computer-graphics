#include <string.h>
class Axes
{
public:
    Axes()
    {
        setShaders();
        glGenVertexArrays(1, &axesVAO);
        glGenBuffers(1, &vertexBuffer);
        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Axes/Axes.vertexshader", "objects/Axes/Axes.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void setBuffers()
    {

        static const GLfloat g_vertex_buffer_data[] = {
            -1000.0f,
            0.0f,
            0.0f,

            1000.0f,
            0.0f,
            0.0f,

            0.0f,
            -1000.0f,
            0.0f,

            0.0f,
            1000.0f,
            0.0f,

            0.0f,
            0.0f,
            -1000.0f,

            0.0f,
            0.0f,
            1000.0f,
        };

        glBindVertexArray(axesVAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
    }

    void bindBuffers()
    {
        glBindVertexArray(axesVAO);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
    }
    void draw(glm::mat4 MVP)
    {
        glUseProgram(ProgramID);
        bindBuffers();
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID
        glDrawArrays(GL_LINES, 0, 6);
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;

    GLuint axesVAO;
    GLuint vertexBuffer;
};