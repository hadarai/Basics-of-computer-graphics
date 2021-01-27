#include <string.h>

class LoadedObject
{
public:
    LoadedObject(char *model_filepath)
    {
        // readFile(model_file);

        setShaders();

        glGenVertexArrays(1, &objectVAO);
        glGenBuffers(1, &vertexBuffer);
        // glGenBuffers(1, &elementBuffer);

        setBuffers(model_filepath);
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/LoadedObject/LoadedObject.vertexshader", "objects/LoadedObject/LoadedObject.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        // OffsetID = glGetUniformLocation(ProgramID, "offset"); //dowiaduje sie gdzie jest w tym shaderze cos jak Offset
        LightPositionID = glGetUniformLocation(ProgramID, "lightPosition");
        ViewPositionID = glGetUniformLocation(ProgramID, "viewPosition");
    }
    void setBuffers(char *model_filepath)
    {
        if (!loadOBJ(model_filepath, vertices, uvs, normals))
        {
            fprintf(stderr, "Error reading from file.\n");
            exit(EXIT_FAILURE);
        }

        // GLfloat g_vertex_buffer_data[10];
        // Napełnianie g_vertex_buffer_data i indices

        glBindVertexArray(objectVAO);
        // for (auto v : vertices)
        // {
        //     printf("(%f, %f, %f)\n", v.x, v.y, v.z);
        // }
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
    }

    void bindBuffers()
    {
        glBindVertexArray(objectVAO);

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

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
    }
    void draw(glm::mat4 MVP, glm::vec3 view_position, glm::vec3 light_position)
    {
        glUseProgram(ProgramID);
        bindBuffers();
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID
        // glUniform3f(OffsetID, position.x, position.y, position.z);
        glUniform3f(LightPositionID, light_position.x, light_position.y, light_position.z); //pozycja swiatla!!!
        glUniform3f(ViewPositionID, view_position.x, view_position.y, view_position.z);     //pozycja obserwatora (do oświetlenia)
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(glm::vec3));
        // glDrawElements(
        //     GL_TRIANGLES,      // mode
        //     indices.size(),    // count
        //     GL_UNSIGNED_SHORT, // type
        //     (void *)0          // element array buffer offset
        // );
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    // GLuint OffsetID;
    GLuint LightPositionID;
    GLuint ViewPositionID;

    GLuint objectVAO;
    GLuint vertexBuffer;
    GLuint colorBuffer;
    // GLuint elementBuffer;

    // std::vector<unsigned short> indices;
    // Read our .obj file
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    // kind of something
};