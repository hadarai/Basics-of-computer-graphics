#include <string.h>
class Particle
{
public:
    Particle()
    {
        setShaders();

        glGenVertexArrays(1, &trianVAO);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &colorBuffer);
        glGenBuffers(1, &instanceBuffer);
        glGenBuffers(1, &elementBuffer);

        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Particle/particle.vertexshader", "objects/Particle/particle.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void setBuffers()
    {
        int index = 0;
        for (int y = -5; y < 5; y += 1)
        {
            for (int x = -5; x < 5; x += 1)
            {
                glm::vec3 translation;
                translation.x = (float)x; // + offset;
                translation.y = (float)y; // + offset;
                translation.z = 0;
                printf("%d: trans o (%f, %f. %f)\n", index, translation.x, translation.y, translation.z);
                translations[index++] = translation;
            }
        }

        float trianVertices[] = {
            // positions
            0.0f,
            0.0f,
            0.0f,

            1.0f,
            0.0f,
            0.0f,

            0.0f,
            1.0f,
            0.0f};

        float trianColors[] = {
            1.0f,
            1.0f,
            0.0f,

            0.0f,
            1.0f,
            0.0f,

            0.0f,
            0.0f,
            1.0f,
        };
        indices = {1, 2, 3};

        glBindVertexArray(trianVAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trianVertices), trianVertices,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trianColors), trianColors, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(translations), translations, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        // glVertexAttribPointer(0,
        //                       3 /*tu sie mowi, ze sie wklada 3 floaty*/,
        //                       GL_FLOAT,
        //                       GL_FALSE,
        //                       0,
        //                       (void *)0);

        // glEnableVertexAttribArray(1);
        // glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        // glVertexAttribPointer(1,
        //                       3,
        //                       GL_FLOAT,
        //                       GL_FALSE,
        //                       0,
        //                       (void *)0);

        // glEnableVertexAttribArray(2);
        // glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        // glVertexAttribPointer(2,
        //                       2,
        //                       GL_FLOAT,
        //                       GL_FALSE,
        //                       0,
        //                       (void *)0);
        // glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
    }
    void bindBuffers()
    {
        Errors("stepex1");
        glBindVertexArray(trianVAO);

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
                              3,
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
            100);

        Errors("step3");

        // glDisableVertexAttribArray(0);
        // glDisableVertexAttribArray(1);
        // glDisableVertexAttribArray(2);
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    GLuint OffsetID;

    unsigned int trianVAO;

    unsigned int vertexBuffer;
    unsigned int colorBuffer;
    unsigned int instanceBuffer;
    unsigned int elementBuffer;

    std::vector<unsigned short> indices;

    glm::vec3 translations[100];

    float offset = 1.0f;
};