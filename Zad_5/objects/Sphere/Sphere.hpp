#include <string.h>
class Sphere
{
public:
    Sphere()
    {
        setShaders();

        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementBuffer);

        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Sphere/Sphere.vertexshader", "objects/Sphere/Sphere.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP");    //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        OffsetID = glGetUniformLocation(ProgramID, "offset"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        LightPositionID = glGetUniformLocation(ProgramID, "lightPosition");
        ViewPositionID = glGetUniformLocation(ProgramID, "viewPosition");
    }
    void setBuffers()
    {
        GLfloat g_vertex_buffer_data[660];
        int index = 0;
        for (float theta = 0; theta < M_PI; theta += (M_PI / 10))
        {
            for (float phi = 0; phi < (2 * M_PI); phi += (M_PI / 10))
            {
                g_vertex_buffer_data[index + 0] = 0.0 + r * sin(theta) * cos(phi);
                g_vertex_buffer_data[index + 1] = 0.0 + r * sin(theta) * sin(phi);
                g_vertex_buffer_data[index + 2] = 0.0 + r * cos(theta);
                index += 3;
            }
        }
        for (int i = 0; i < 220; i++) //; i += 2)
        {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + 20);
            indices.push_back(i + 1);
            indices.push_back(i + 20);
            indices.push_back(i + 21);
        }

        glBindVertexArray(sphereVAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
    }

    void bindBuffers()
    {
        glBindVertexArray(sphereVAO);

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
    void draw(glm::mat4 MVP, glm::vec3 position, glm::vec3 view_position)
    {
        glUseProgram(ProgramID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID
        glUniform3f(OffsetID, position.x, position.y, position.z);
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
    GLuint OffsetID;
    GLuint LightPositionID;
    GLuint ViewPositionID;

    GLuint sphereVAO;
    GLuint vertexBuffer;
    GLuint colorBuffer;
    GLuint elementBuffer;

    std::vector<unsigned short> indices;

    GLfloat r = 1.0;
    // kind of something
};