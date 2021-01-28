#include <string.h>
class Triangle
{
public:
    Triangle()
    {
        setShaders();

        glGenVertexArrays(1, &TriangleVAO);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementBuffer);

        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Triangle/Triangle.vertexshader", "objects/Triangle/Triangle.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        LightPositionID = glGetUniformLocation(ProgramID, "lightPosition");
        ViewPositionID = glGetUniformLocation(ProgramID, "viewPosition");
    }
    void setBuffers()
    {
        GLfloat g_vertex_buffer_data[9] = {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0};

        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);

        glBindVertexArray(TriangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
    }

    void bindBuffers()
    {
        glBindVertexArray(TriangleVAO);

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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
    }
    void draw(glm::mat4 MVP, glm::vec3 view_position)
    {
        glUseProgram(ProgramID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);                          // umieszcza MVP jako uniform z location = MatrixID
        glUniform3f(LightPositionID, 0.0f, 3.0f, 0.0f);                                 //pozycja swiatla!!!
        glUniform3f(ViewPositionID, view_position.x, view_position.y, view_position.z); //pozycja swiatla!!!
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0          // element array buffer offset
        );

        bindBuffers();
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    GLuint LightPositionID;
    GLuint ViewPositionID;

    GLuint TriangleVAO;
    GLuint vertexBuffer;
    GLuint elementBuffer;

    std::vector<unsigned short> indices;
};