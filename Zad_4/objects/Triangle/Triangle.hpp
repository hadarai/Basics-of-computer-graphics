class Triangle
{
public:
    Triangle()
    {
        setShaders();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Triangle/TriangleVertexShader.vertexshader", "objects/Triangle/TriangleFragmentShader.fragmentshader");
        // MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        // Get a handle for our "MVP" uniform
        // Only during the initialisation
        MatrixID = glGetUniformLocation(ProgramID, "MVP");
        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        printf("ProgramID: %d\n", ProgramID);
    }
    void setBuffers(glm::vec3 point_1, glm::vec3 point_2, glm::vec3 point_3)
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        GLfloat g_vertex_buffer_data[] = {
            point_1.x, point_1.y, point_1.z,
            point_2.x, point_2.y, point_2.z,
            point_3.x, point_3.y, point_3.z};

        // GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

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
    }
    void draw(glm::mat4 MVP)
    {
        glUseProgram(ProgramID);
        glBindVertexArray(VertexArrayID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID
        glDrawArrays(GL_TRIANGLES, 0, 3);                      // 12*3 indices starting at 0 -> 12 triangles
    }

private:
    GLuint ProgramID;
    GLuint MatrixID;
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint elementbuffer;
    // kind of something
    std::vector<unsigned int> indices;
};
