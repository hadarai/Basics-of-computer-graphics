#include <string.h>

class LoadedObject
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
    };

public:
    LoadedObject(char *model_filepath, char *texture_filepath)
    {
        setShaders();

        glGenVertexArrays(1, &objectVAO);
        glGenBuffers(1, &vertexBuffer);
        glGenTextures(1, &textureBuffer);

        setBuffers(model_filepath, texture_filepath);
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/LoadedObject/LoadedObject.vertexshader", "objects/LoadedObject/LoadedObject.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        // OffsetID = glGetUniformLocation(ProgramID, "offset"); //dowiaduje sie gdzie jest w tym shaderze cos jak Offset
        // TextureID = glGetUniformLocation(TextureID, "tex");
        LightPositionID = glGetUniformLocation(ProgramID, "lightPosition");
        ViewPositionID = glGetUniformLocation(ProgramID, "viewPosition");
    }

    void setBuffers(char *model_filepath, char *texture_filepath)
    {
        if (!loadOBJ(model_filepath, vertices, uvs, normals))
        {
            fprintf(stderr, "Error reading from file.\n");
            exit(EXIT_FAILURE);
        }

        std::vector<Vertex> data_packed;
        for (int i = 0; i < vertices.size(); i++)
        {
            Vertex temp;
            temp.position = vertices[i];
            temp.uv = uvs[i];
            temp.normal = normals[i];
            data_packed.push_back(temp);
        }
        // Napełnianie g_vertex_buffer_data i indices

        glBindVertexArray(objectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, data_packed.size() * sizeof(Vertex), &data_packed[0], GL_STATIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        unsigned char *data = stbi_load(texture_filepath, &texture_width, &texture_height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("Failed to load texture\n");
        }
        stbi_image_free(data);

        // glEnableVertexAttribArray(0);
    }

    void bindBuffers()
    {
        glBindVertexArray(objectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,              // attribute. No particular reason for 0, but must match the layout in the shader.
            3,              // size
            GL_FLOAT,       // type
            GL_FALSE,       // normalized?
            sizeof(Vertex), // stride
            (void *)0       // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,                           // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                           // size
            GL_FLOAT,                    // type
            GL_FALSE,                    // normalized?
            sizeof(Vertex),              // stride
            (void *)offsetof(Vertex, uv) // array buffer offset
        );

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,                               // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                               // size
            GL_FLOAT,                        // type
            GL_FALSE,                        // normalized?
            sizeof(Vertex),                  // stride
            (void *)offsetof(Vertex, normal) // array buffer offset
        );

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
    }
    void draw(glm::mat4 MVP, glm::vec3 view_position, glm::vec3 light_position)
    {
        glUseProgram(ProgramID); // decyduje ktorego shadera uzywa-*/
        bindBuffers();
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID

        glUniform3f(LightPositionID, light_position.x, light_position.y, light_position.z); //pozycja swiatla!!!
        glUniform3f(ViewPositionID, view_position.x, view_position.y, view_position.z);     //pozycja obserwatora (do oświetlenia)
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(glm::vec3));
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    // GLuint OffsetID;
    GLuint LightPositionID;
    GLuint ViewPositionID;
    GLuint TextureID;

    GLuint objectVAO;
    GLuint vertexBuffer;
    GLuint colorBuffer;
    GLuint textureBuffer;
    // GLuint elementBuffer;
    int texture_width, texture_height, nrChannels;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
};