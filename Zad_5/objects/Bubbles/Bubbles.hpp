#include <string.h>
#include <tgmath.h>

class Bubbles
{
public:
    Bubbles()
    {
        setShaders();

        glGenVertexArrays(1, &bubbleVAO);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &colorBuffer);
        glGenBuffers(1, &instanceBuffer);
        glGenBuffers(1, &elementBuffer);

        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Bubbles/bubbles.vertexshader", "objects/Bubbles/bubbles.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        LightPositionID = glGetUniformLocation(ProgramID, "lightPosition");
        ViewPositionID = glGetUniformLocation(ProgramID, "viewPosition");
    }
    void setBuffers()
    {
        int trans_index = 0;
        for (float z = -1; z < 1 - 0.25; z += 0.25)
        {
            for (float x = -2; x < 2 - 0.25; x += 0.25)
            {
                if (rand() < 0.5 * RAND_MAX)
                {
                    glm::vec3 translation;
                    translation.x = (float)x + 0.15; // + offset;
                    translation.y = 0.1;
                    translation.z = (float)z + 0.15;
                    // printf("%d: trans o (%f, %f. %f)\n", trans_index, translation.x, translation.y, translation.z);
                    translations.push_back(translation);
                    trans_index++;
                    // translations[trans_index++] = translation;
                    colors.push_back(
                        glm::vec3(
                            (float)rand() / RAND_MAX,
                            (float)rand() / RAND_MAX,
                            (float)rand() / RAND_MAX));
                    // printf("%f ", (float)rand() / RAND_MAX);
                }
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
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(translations), translations, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::vec3), &translations[0], GL_STATIC_DRAW);

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

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1,
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
    std::vector<glm::vec3> move_bubbles_higher()
    {
        for (auto &trans : translations)
        {
            trans.y += 0.0001 + (rand() / (RAND_MAX / (0.015 - 0.001)));
            trans.y = fmod(trans.y, 2.0f);
        }
        // int trans_index = 0;
        // for (float z = -1; z < 1 - 0.25; z += 0.25)
        // {
        //     for (float x = -2; x < 2 - 0.25; x += 0.25)
        //     {
        //         translations[trans_index].y += 0.01; // + offset;
        //         translations[trans_index].y = fmod(translations[trans_index].y, 2.0f);
        //         trans_index++;
        //     }
        // }
        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(translations), translations, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::vec3), &translations[0], GL_STATIC_DRAW);
        return translations;
    }
    void draw(glm::mat4 MVP, glm::vec3 player_position)
    {
        glUseProgram(ProgramID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);                                // umieszcza MVP jako uniform z location = MatrixID
        glUniform3f(LightPositionID, 0.0f, 3.0f, 0.0f);                                       //pozycja swiatla!!!
        glUniform3f(ViewPositionID, player_position.x, player_position.y, player_position.z); //pozycja swiatla!!!
        Errors("step1");
        bindBuffers();

        Errors("step2");
        glDrawElementsInstanced(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0,         // element array buffer offset
            translations.size());

        Errors("step3");

        // glDisableVertexAttribArray(0);
        // glDisableVertexAttribArray(1);
        // glDisableVertexAttribArray(2);
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    // GLuint OffsetID;
    GLuint LightPositionID;
    GLuint ViewPositionID;

    unsigned int bubbleVAO;

    unsigned int vertexBuffer;
    unsigned int colorBuffer;
    unsigned int instanceBuffer;
    unsigned int elementBuffer;

    std::vector<unsigned short> indices;

    // glm::vec3 translations[105];
    std::vector<glm::vec3> translations;
    std::vector<glm::vec3> colors;

    GLfloat r = 0.1;
};