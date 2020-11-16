class PlayerSphere
{
public:
    PlayerSphere(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat r)
    {
        // printf("x0=%f, y0=%f, z0=%f, r=%f\n", x0, y0, z0, r);
        // x0 = x0;
        // y0 = y0;
        // z0 = z0;
        // r = r;
        setShaders();
        setBuffers(x0, y0, z0, r);
    }
    void setShaders()
    {
        printf("ustawianie shaderow\n");
        ProgramID = LoadShaders("objects/PlayerSphere/PlayerSphereVertexShader.vertexshader", "objects/PlayerSphere/PlayerSphereFragmentShader.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void setBuffers(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat r)
    {
        // printf("DUPA1\n");
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        GLfloat g_vertex_buffer_data[660];
        int index = 0;
        printf("x0=%f, y0=%f, z0=%f, r=%f", x0, y0, z0, r);
        for (float theta = 0; theta < M_PI; theta += (M_PI / 10))
        {
            for (float phi = 0; phi < (2 * M_PI); phi += (M_PI / 10))
            {
                printf("Teraz jestem na: %d\n", index);
                g_vertex_buffer_data[index + 0] = x0 + r * sin(theta) * cos(phi);
                g_vertex_buffer_data[index + 1] = y0 + r * sin(theta) * sin(phi);
                g_vertex_buffer_data[index + 2] = z0 + r * cos(theta);
                printf("wygenerowalem: (%f, %f, %f)\n", x0 + r * sin(theta) * cos(phi), g_vertex_buffer_data[index + 1], g_vertex_buffer_data[index + 2]);
                index += 3;
            }
        }

        // for (int i = 0; i < 120; i++) //; i += 2)
        // {
        //     indices.push_back(i);
        //     indices.push_back(i + 1);
        //     indices.push_back(i + 11);
        //     indices.push_back(i + 1);
        //     indices.push_back(i + 11);
        //     indices.push_back(i + 12);
        // }
        for (int i = 0; i < 220; i++) //; i += 2)
        {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + 20);
            indices.push_back(i + 1);
            indices.push_back(i + 20);
            indices.push_back(i + 21);
        }

        // printf("DUPA2\n");
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // printf("DUPA3\n");
    }
    void draw(glm::mat4 MVP)
    {
        // printf("DUPA6\n");
        glUseProgram(ProgramID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID

        // glEnable(GL_CULL_FACE);
        // glFrontFace(GL_CCW);
        // glCullFace(GL_BACK);
        // printf("DUPA5\n");
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

        // printf("DUPA4\n");
        // glDrawArrays(GL_LINES, 0, 120);
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glDrawElements(
            GL_TRIANGLES,    // mode
            indices.size(),  // count
            GL_UNSIGNED_INT, // type
            (void *)0        // element array buffer offset
        );

        // printf("DUPA44\n");
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
    std::vector<unsigned int> indices;
    // GLfloat x0;
    // GLfloat y0;
    // GLfloat z0;
    // GLfloat r;
    // kind of something
};