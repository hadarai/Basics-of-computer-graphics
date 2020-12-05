class Cuboid
{
public:
    Cuboid()
    {
        setShaders();
        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Cuboid/cuboid.vertexshader", "objects/Cuboid/cuboid.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void setBuffers()
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        GLfloat g_vertex_buffer_data[] = {
            -2.0f, -1.0f, -1.0f,
            -2.0f, -1.0f, 1.0f,
            2.0f, -1.0f, -1.0f,
            2.0f, -1.0f, 1.0f,

            -2.0f, 1.0f, -1.0f,
            -2.0f, 1.0f, 1.0f,
            2.0f, 1.0f, -1.0f,
            2.0f, 1.0f, 1.0f};

        indices = {
            0, 1, 2,
            1, 2, 3,

            // 4, 6, 5,
            // 6, 5, 7,

            0, 1, 4,
            1, 4, 5,

            2, 3, 6,
            3, 6, 7,

            0, 2, 4,
            2, 4, 6,

            1, 3, 5,
            3, 5, 7};

        // GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }
    void draw(glm::mat4 MVP)
    {

        glUseProgram(ProgramID);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform:
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID

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

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glDrawElements(
            GL_TRIANGLES,    // mode
            indices.size(),  // count
            GL_UNSIGNED_INT, // type
            (void *)0        // element array buffer offset
        );

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    // GLuint colorbuffer;

    GLuint elementbuffer;
    std::vector<unsigned int> indices;
    // kind of something
};