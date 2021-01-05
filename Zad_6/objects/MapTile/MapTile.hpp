class MapTile
{
public:
    MapTile(std::vector<short> *height_map, float offset)
    {
        setShaders();

        glGenVertexArrays(1, &RectangleVAO);
        glGenBuffers(1, &vertexbuffer);
        glGenBuffers(1, &elementbuffer);

        setBuffers(height_map, offset);
    }
    void setShaders()
    {
        ProgramID = LoadShaders("objects/MapTile/maptile.vertexshader", "objects/MapTile/maptile.fragmentshader");
        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void setBuffers(std::vector<short> *height_map, float offset)
    {
        GLfloat *g_vertex_buffer_data = (GLfloat *)malloc(g_vertex_buffer_data_size);
        tile_middle = glm::vec3(offset + 0.5, offset + 0.5, 0.0f);
        int ilosc_wierszy_mapy = SRTM_SIZE;
        int ilosc_kolumn_mapy = SRTM_SIZE;
        int szerokosc_wiersza = SRTM_SIZE * 3;
        int g_vertex_buffer_data_index = 0;
        printf("OTRZYMALEM %ld ELEMENTÓW\n", height_map->size());
        // exit(EXIT_SUCCESS);
        for (int i = 0; i < ilosc_wierszy_mapy; i++) //iteruje sie 1201 razy
        {
            // if (i > 1200)
            //     break;
            // g_vertex_buffer_data_index = 0;
            for (int j = 0; j < ilosc_kolumn_mapy; j++) //iteruje sie 1201 razy
            {
                // printf("%d ", g_vertex_buffer_data_index);
                // if (i > 1200 || j > 3600)
                //     break;
                // int current_index = i * szerokosc_wiersza + j;

                g_vertex_buffer_data[g_vertex_buffer_data_index] = (GLfloat)i / SRTM_SIZE;     //To jest X ofc
                g_vertex_buffer_data[g_vertex_buffer_data_index + 1] = (GLfloat)j / SRTM_SIZE; //To jest Y, bo przeciez to mapa scienna.
                                                                                               // if (g_vertex_buffer_data_index / 3 < height_map->size())

                //To jest Z, no i jest on blizej/dalej oka kamery "skalowanie" [-500, 9000] -> [-1, 1]
                GLfloat Z_value = (GLfloat)(*height_map)[g_vertex_buffer_data_index / 3];
                if (Z_value > 0)
                {
                    g_vertex_buffer_data[g_vertex_buffer_data_index + 2] = Z_value / 9000;
                }
                else
                {
                    // printf("Znalazlem ujemna: %f\n", Z_value);
                    if (Z_value == -32768)
                        g_vertex_buffer_data[g_vertex_buffer_data_index + 2] = 0.0f;
                    else
                        g_vertex_buffer_data[g_vertex_buffer_data_index + 2] = Z_value / 500;
                }
                // printf("%d: Teraz wygenerowalem: (%f, %f, %f)\n",
                //        current_index,
                //        (GLfloat)i / SRTM_SIZE,
                //        (GLfloat)j / SRTM_SIZE,
                //        (GLfloat)(*height_map)[current_index]);
                //    0.0f);

                g_vertex_buffer_data[g_vertex_buffer_data_index + 0] += offset;
                g_vertex_buffer_data[g_vertex_buffer_data_index + 1] += offset;

                g_vertex_buffer_data_index = g_vertex_buffer_data_index + 3;
            }
            // printf("\n");
        }
        // exit(EXIT_SUCCESS);

        // for (unsigned int i = 0; i < SRTM_SIZE * SRTM_SIZE; i = i + 2) //powinno byc i++
        // {
        //     if (i % (SRTM_SIZE - 1) == 0)
        //     {
        //         printf("pomijam");
        //         continue;
        //     }

        //     indices.push_back(i);
        //     indices.push_back(i + 1);
        //     indices.push_back(i + SRTM_SIZE);

        //     indices.push_back(i + 1);
        //     indices.push_back(i + SRTM_SIZE);
        //     indices.push_back(i + SRTM_SIZE + 1);
        //     // printf("Triangle (%d, %d, %d) generated\n", i, i + 1, i + 2);
        // }

        for (unsigned int i = 0; i < SRTM_SIZE - 1; i++)
        {
            for (unsigned int j = 0; j < SRTM_SIZE - 1; j++)
            {
                int index = i * SRTM_SIZE + j;
                indices_lod0.push_back(index);
                indices_lod0.push_back(index + 1);
                indices_lod0.push_back(index + SRTM_SIZE);

                indices_lod0.push_back(index + 1);
                indices_lod0.push_back(index + SRTM_SIZE);
                indices_lod0.push_back(index + SRTM_SIZE + 1);
            }
        }
        for (unsigned int i = 0; i < SRTM_SIZE - 1; i = i + 2)
        {
            for (unsigned int j = 0; j < SRTM_SIZE - 1; j = j + 2)
            {
                int index = i * SRTM_SIZE + j;
                indices_lod1.push_back(index);
                indices_lod1.push_back(index + 2);
                indices_lod1.push_back(index + SRTM_SIZE * 2);

                indices_lod1.push_back(index + 2);
                indices_lod1.push_back(index + SRTM_SIZE * 2);
                indices_lod1.push_back(index + SRTM_SIZE * 2 + 2);
            }
        }

        for (unsigned int i = 0; i < SRTM_SIZE - 1; i = i + 4)
        {
            for (unsigned int j = 0; j < SRTM_SIZE - 1; j = j + 4)
            {
                int index = i * SRTM_SIZE + j;
                indices_lod2.push_back(index);
                indices_lod2.push_back(index + 4);
                indices_lod2.push_back(index + SRTM_SIZE * 4);

                indices_lod2.push_back(index + 4);
                indices_lod2.push_back(index + SRTM_SIZE * 4);
                indices_lod2.push_back(index + SRTM_SIZE * 4 + 4);
            }
        }

        glBindVertexArray(RectangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data_size, g_vertex_buffer_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_lod0.size() * sizeof(unsigned int), &indices_lod0[0], GL_STATIC_DRAW);
        Errors("DUPA26");

        free(g_vertex_buffer_data);
        glEnableVertexAttribArray(0);
    }
    void bindBuffers(int which_lod)
    {
        glBindVertexArray(RectangleVAO);

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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        switch (which_lod)
        {
        case 0:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_lod0.size() * sizeof(unsigned int), &indices_lod0[0], GL_STATIC_DRAW);
            break;
        case 1:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_lod1.size() * sizeof(unsigned int), &indices_lod1[0], GL_STATIC_DRAW);
            break;
        case 2:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_lod2.size() * sizeof(unsigned int), &indices_lod2[0], GL_STATIC_DRAW);
            break;
        default:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_lod0.size() * sizeof(unsigned int), &indices_lod0[0], GL_STATIC_DRAW);
            break;
        }
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_lod->size() * sizeof(unsigned int), indices_lod, GL_STATIC_DRAW);
    }
    void draw(glm::mat4 MVP, glm::vec3 player_position)
    {
        glUseProgram(ProgramID);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform:
        // Errors("pupa1\n");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID
        float distance = glm::distance(player_position, tile_middle);
        if (distance < 1.0f)
        {
            bindBuffers(0);
            glDrawElements(
                GL_TRIANGLES,        // mode
                indices_lod0.size(), // count
                GL_UNSIGNED_INT,     // type
                (void *)0            // element array buffer offset
            );
            // printf("a bardzo\n");
        }

        else if (distance > 1.0f && distance < 5.0f)
        {
            bindBuffers(1);
            glDrawElements(
                GL_TRIANGLES,        // mode
                indices_lod1.size(), // count
                GL_UNSIGNED_INT,     // type
                (void *)0            // element array buffer offset
            );
            // printf("a średnio\n");
        }

        else
        {
            bindBuffers(2);
            glDrawElements(
                GL_TRIANGLES,        // mode
                indices_lod2.size(), // count
                GL_UNSIGNED_INT,     // type
                (void *)0            // element array buffer offset
            );
            // printf("a mało\n");
        }

        // glDisableVertexAttribArray(0);
    }

private:
    GLuint RectangleVAO;

    GLuint ProgramID;
    GLuint MatrixID;

    GLuint vertexbuffer;

    glm::vec3 tile_middle;

    long long g_vertex_buffer_data_size = SRTM_SIZE * SRTM_SIZE * 3 * sizeof(GLfloat);
    GLuint elementbuffer;
    std::vector<unsigned int> indices_lod0;
    std::vector<unsigned int> indices_lod1;
    std::vector<unsigned int> indices_lod2;
};