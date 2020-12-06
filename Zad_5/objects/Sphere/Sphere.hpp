#include <string.h>
class Sphere
{
public:
    Sphere()
    {
        setShaders();
        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Sphere/Sphere.vertexshader", "objects/Sphere/Sphere.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP");    //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        OffsetID = glGetUniformLocation(ProgramID, "offset"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void setBuffers()
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        GLfloat x0 = 0.0, y0 = 0.0, z0 = 0.0;
        GLfloat g_vertex_buffer_data[660];
        int index = 0;
        for (float theta = 0; theta < M_PI; theta += (M_PI / 10))
        {
            for (float phi = 0; phi < (2 * M_PI); phi += (M_PI / 10))
            {
                g_vertex_buffer_data[index + 0] = x0 + r * sin(theta) * cos(phi);
                g_vertex_buffer_data[index + 1] = y0 + r * sin(theta) * sin(phi);
                g_vertex_buffer_data[index + 2] = z0 + r * cos(theta);
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

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
    }
    void draw(glm::mat4 MVP, glm::vec3 position)
    {
        glUseProgram(ProgramID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        GLfloat scale = 0.5;
        // glUniform1f(ScaleID, scale);
        glUniform3f(OffsetID, position.x, position.y, position.z);

        // Draw the triangles !
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0          // element array buffer offset
        );

        glDisableVertexAttribArray(0);
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    GLuint OffsetID;
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint elementbuffer;

    std::vector<unsigned short> indices;

    GLfloat r = 0.05;
    // kind of something
};