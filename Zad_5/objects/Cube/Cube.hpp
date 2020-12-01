class Cube
{
public:
    Cube()
    {
        setShaders();
        setBuffers();
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/Cube/CubeVertexShader.vertexshader", "objects/Cube/CubeFragmentShader.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void setBuffers()
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        GLfloat g_vertex_buffer_data[] = {
            -2.5f, -2.5f, -2.5f,
            -2.5f, -2.5f, 2.5f,
            2.5f, -2.5f, -2.5f,
            2.5f, -2.5f, 2.5f,

            -2.5f, 2.5f, -2.5f,
            -2.5f, 2.5f, 2.5f,
            2.5f, 2.5f, -2.5f,
            2.5f, 2.5f, 2.5f};

        indices = {
            0, 1, 2,
            1, 2, 3,

            4, 6, 5,
            6, 5, 7,

            0, 1, 4,
            1, 4, 5,

            2, 3, 6,
            3, 6, 7,

            0, 2, 4,
            2, 4, 6,

            1, 3, 5,
            3, 5, 7};

        GLfloat g_color_buffer_data[] = {
            0.583f, 0.771f, 0.014f,
            0.609f, 0.115f, 0.436f,
            0.327f, 0.483f, 0.844f,
            0.822f, 0.569f, 0.201f,
            0.435f, 0.602f, 0.223f,
            0.310f, 0.747f, 0.185f,
            0.597f, 0.770f, 0.761f,
            0.559f, 0.436f, 0.730f,
            0.359f, 0.583f, 0.152f,
            0.483f, 0.596f, 0.789f,
            0.559f, 0.861f, 0.639f,
            0.195f, 0.548f, 0.859f,
            0.014f, 0.184f, 0.576f,
            0.771f, 0.328f, 0.970f,
            0.406f, 0.615f, 0.116f,
            0.676f, 0.977f, 0.133f,
            0.971f, 0.572f, 0.833f,
            0.140f, 0.616f, 0.489f,
            0.997f, 0.513f, 0.064f,
            0.945f, 0.719f, 0.592f,
            0.543f, 0.021f, 0.978f,
            0.279f, 0.317f, 0.505f,
            0.167f, 0.620f, 0.077f,
            0.347f, 0.857f, 0.137f,
            0.055f, 0.953f, 0.042f,
            0.714f, 0.505f, 0.345f,
            0.783f, 0.290f, 0.734f,
            0.722f, 0.645f, 0.174f,
            0.302f, 0.455f, 0.848f,
            0.225f, 0.587f, 0.040f,
            0.517f, 0.713f, 0.338f,
            0.053f, 0.959f, 0.120f,
            0.393f, 0.621f, 0.362f,
            0.673f, 0.211f, 0.457f,
            0.820f, 0.883f, 0.371f,
            0.982f, 0.099f, 0.879f};

        // GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        // GLuint colorbuffer;
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

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

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,        // attribute. No particular reason for 1, but must match the layout in the shader.
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
    GLuint colorbuffer;

    GLuint elementbuffer;
    std::vector<unsigned int> indices;
    // kind of something
};