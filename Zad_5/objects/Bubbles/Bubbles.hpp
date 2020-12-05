#include <string.h>
class Bubbles
{
public:
    Bubbles()
    {
        setShaders();

        glGenVertexArrays(1, &bubbleVAO);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &instanceBuffer);
        glGenBuffers(1, &elementBuffer);

        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Bubbles/bubbles.vertexshader", "objects/Bubbles/bubbles.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void setBuffers()
    {
        int trans_index = 0;
        for (int z = -1; z < 1; z += 1)
        {
            for (int x = -2; x < 2; x += 1)
            {
                glm::vec3 translation;
                translation.x = (float)x + 0.5; // + offset;
                translation.y = -0.9;           // + offset;
                translation.z = (float)z + 0.5;
                printf("%d: trans o (%f, %f. %f)\n", trans_index, translation.x, translation.y, translation.z);
                translations[trans_index++] = translation;
            }
        }

        GLfloat g_vertex_buffer_data[660];
        GLfloat x0 = 0.0, y0 = 0.0, z0 = 0.0;
        int vertex_index = 0;
        for (float theta = 0; theta < M_PI; theta += (M_PI / 10))
        {
            for (float phi = 0; phi < (2 * M_PI); phi += (M_PI / 10))
            {
                g_vertex_buffer_data[vertex_index + 0] = x0 + r * sin(theta) * cos(phi);
                g_vertex_buffer_data[vertex_index + 1] = y0 + r * sin(theta) * sin(phi);
                g_vertex_buffer_data[vertex_index + 2] = z0 + r * cos(theta);
                vertex_index += 3;
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

        glBindVertexArray(bubbleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(translations), translations, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    }
    void bindBuffers()
    {
        Errors("stepex1");
        glBindVertexArray(bubbleVAO);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0,
                              3 /*tu sie mowi, ze sie wklada 3 floaty*/,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void *)0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        glVertexAttribPointer(2,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void *)0);
        glVertexAttribDivisor(2, 1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        Errors("stepex2");
    }
    void draw(glm::mat4 MVP)
    {
        glUseProgram(ProgramID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID
        Errors("step1");
        bindBuffers();

        Errors("step2");
        glDrawElementsInstanced(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0,         // element array buffer offset
            8);

        Errors("step3");

        // glDisableVertexAttribArray(0);
        // glDisableVertexAttribArray(1);
        // glDisableVertexAttribArray(2);
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    GLuint OffsetID;

    unsigned int bubbleVAO;

    unsigned int vertexBuffer;
    unsigned int instanceBuffer;
    unsigned int elementBuffer;

    std::vector<unsigned short> indices;

    glm::vec3 translations[8];

    GLfloat r = 0.1;
};