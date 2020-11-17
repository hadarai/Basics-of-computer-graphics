class ObstacleTriangle
{
public:
    ObstacleTriangle()
    {
        setShaders();
        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
        // MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
        // Get a handle for our "MVP" uniform
        // Only during the initialisation
        MatrixID = glGetUniformLocation(ProgramID, "MVP");
        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
    }
    void setBuffers()
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        static const GLfloat g_vertex_buffer_data[] = {
            -2.0f, 3.0f, -2.0f,
            -1.0f, 2.0f, 0.0f,
            1.0f, 2.0f, 1.0f};

        // GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        // fill "indices" as needed
        // indices.push_back(0);
        // indices.push_back(1);
        // indices.push_back(2);
        // Generate a buffer for the indices

        // glGenBuffers(1, &elementbuffer);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }
    void draw(glm::mat4 MVP, glm::vec3 point_1, glm::vec3 point_2, glm::vec3 point_3)
    {

        glUseProgram(ProgramID);

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID

        // glEnable(GL_CULL_FACE);
        // glFrontFace(GL_CCW);

        static const GLfloat g_vertex_buffer_data[] = {
            point_1[0], point_1[1], point_1[2],
            point_2[0], point_2[1], point_2[2],
            point_3[0], point_3[1], point_3[2]}; // glCullFace(GL_BACK);

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

        // 2nd attribute buffer : colors
        // glEnableVertexAttribArray(1);
        // glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        // glVertexAttribPointer(
        //     1,        // attribute. No particular reason for 1, but must match the layout in the shader.
        //     3,        // size
        //     GL_FLOAT, // type
        //     GL_FALSE, // normalized?
        //     0,        // stride
        //     (void *)0 // array buffer offset
        // );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // 12*3 indices starting at 0 -> 12 triangles
        // Index buffer
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // // Draw the triangles !
        // glDrawElements(
        //     GL_TRIANGLES,    // mode
        //     indices.size(),  // count
        //     GL_UNSIGNED_INT, // type
        //     (void *)0        // element array buffer offset
        // );

        glDisableVertexAttribArray(0);
        // glDisableVertexAttribArray(1);
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