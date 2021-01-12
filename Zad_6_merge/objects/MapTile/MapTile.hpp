class MapTile
{
public:
    MapTile(std::vector<short> *height_map, glm::vec2 tile_position, bool flat)
    {
        setShaders(flat);
        setBuffers(height_map, tile_position);
    }
    void setShaders(bool flat)
    {
        // ProgramID = LoadShaders("objects/MapTileFlat/maptileflat.vertexshader", "objects/MapTileFlat/maptileflat.fragmentshader");
        if (flat)
            ProgramID = LoadShaders("objects/MapTile/map_tile_flat.vertexshader", "objects/MapTile/map_tile.fragmentshader");
        else
            ProgramID = LoadShaders("objects/MapTile/map_tile_sphere.vertexshader", "objects/MapTile/map_tile.fragmentshader");

        MatrixID = glGetUniformLocation(ProgramID, "MVP"); //dowiaduje sie gdzie jest w tym shaderze cos jak MVP
    }
    void genBuffers(void)
    {
        glGenVertexArrays(1, &RectangleVAO);
        glGenBuffers(1, &vertexbuffer);
        glGenBuffers(1, &elementbuffer);
    }
    void setBuffers(std::vector<short> *height_map, glm::vec2 tile_left_corner)
    {
        genBuffers();
        GLfloat *g_vertex_buffer_data = (GLfloat *)malloc(g_vertex_buffer_data_size);

        // tile_middle = glm::vec3(tile_position.x + 0.5f, tile_position.y + 0.5f, 0.0f);
        int data_rows_amount = SRTM_SIZE;
        int data_columns_amount = SRTM_SIZE;
        // int row_actual_width = SRTM_SIZE * 3;
        int g_vertex_buffer_data_index = 0;

        for (int j = 0; j < data_columns_amount; j++) // iteruje sie 1201 razy
        {
            for (int i = 0; i < data_rows_amount; i++) // iteruje sie 1201 razy, po latitude
            {
                g_vertex_buffer_data[g_vertex_buffer_data_index + 0] = //To jest X = longitude
                    tile_left_corner.x;
                g_vertex_buffer_data[g_vertex_buffer_data_index + 1] = //To jest Y = latitude
                    tile_left_corner.y;

                g_vertex_buffer_data[g_vertex_buffer_data_index + 0] += //do longitude dodaje
                    ((GLfloat)(i + 1) / SRTM_SIZE);                     //na przedziale [0,1]
                g_vertex_buffer_data[g_vertex_buffer_data_index + 1] += //do latitude dodaje
                    1.0f - ((GLfloat)(j + 1) / SRTM_SIZE);              //na przedziale [1,0]

                // g_vertex_buffer_data[g_vertex_buffer_data_index + 1] *= cos(latitude_from); //do latitude dodaje

                // ============================= height is ok
                // To jest Z, no i jest on blizej/dalej oka kamery "skalowanie" [-500, 9000] -> [-1, 1]
                GLfloat Z_value = (GLfloat)(*height_map)[g_vertex_buffer_data_index / 3];
                if (Z_value > 0)
                {
                    g_vertex_buffer_data[g_vertex_buffer_data_index + 2] = Z_value / 9000;
                }
                else
                {
                    if (Z_value == -32768)
                        g_vertex_buffer_data[g_vertex_buffer_data_index + 2] = 0.0f;
                    else
                        g_vertex_buffer_data[g_vertex_buffer_data_index + 2] = Z_value / 500;
                }
                // ============================= end of ok height
                // printf("%d: Teraz wygenerowalem: (%f, %f, %f)\n",
                //        g_vertex_buffer_data_index,
                //        g_vertex_buffer_data[g_vertex_buffer_data_index + 0],
                //        g_vertex_buffer_data[g_vertex_buffer_data_index + 1],
                //        g_vertex_buffer_data[g_vertex_buffer_data_index + 2]);
                // exit(EXIT_SUCCESS);
                g_vertex_buffer_data_index = g_vertex_buffer_data_index + 3;
            }
            // printf("\n");
        }
        //fill the LoD arrray
        unsigned short curr_lod;
        for (unsigned short lod_level = 0; lod_level < 5; lod_level++)
        {
            // unsigned short curr_lod = pow(2, lod_level);
            switch (lod_level)
            {
            case 0:
                curr_lod = 1;
                break;
            case 1:
                curr_lod = 2;
                break;
            case 2:
                curr_lod = 4;
                break;
            case 3:
                curr_lod = 10;
                break;
            case 4:
                curr_lod = 20;
                break;

            default:
                break;
            }

            for (unsigned int i = 0; i < SRTM_SIZE - 1; i = i + curr_lod)
            {
                for (unsigned int j = 0; j < SRTM_SIZE - 1; j = j + curr_lod)
                {
                    int index = i * SRTM_SIZE + j;
                    indices_lod[lod_level].push_back(index);
                    indices_lod[lod_level].push_back(index + curr_lod);
                    indices_lod[lod_level].push_back(index + SRTM_SIZE * curr_lod);

                    indices_lod[lod_level].push_back(index + curr_lod);
                    indices_lod[lod_level].push_back(index + SRTM_SIZE * curr_lod);
                    indices_lod[lod_level].push_back(index + SRTM_SIZE * curr_lod + curr_lod);
                }
            }
        }

        glBindVertexArray(RectangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data_size, g_vertex_buffer_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices_lod[0]).size() * sizeof(unsigned int), &(indices_lod[0])[0], GL_STATIC_DRAW);

        free(g_vertex_buffer_data);
        glEnableVertexAttribArray(0);
    }
    void bindBuffers(unsigned short which_lod)
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices_lod[which_lod]).size() * sizeof(unsigned int), &(indices_lod[which_lod])[0], GL_STATIC_DRAW);
    }
    unsigned int draw(glm::mat4 MVP, unsigned short which_lod)
    {
        glUseProgram(ProgramID);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform:
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // umieszcza MVP jako uniform z location = MatrixID
        bindBuffers(which_lod);
        glDrawElements(
            GL_TRIANGLES,                    // mode
            (indices_lod[which_lod]).size(), // count
            GL_UNSIGNED_INT,                 // type
            (void *)0                        // element array buffer offset
        );

        return (indices_lod[which_lod]).size() / 3;
        // glDisableVertexAttribArray(0);
    }

private:
    GLuint RectangleVAO;

    GLuint ProgramID;
    GLuint MatrixID;

    GLuint vertexbuffer;

    long long g_vertex_buffer_data_size = SRTM_SIZE * SRTM_SIZE * 3 * sizeof(GLfloat);
    GLuint elementbuffer;

    unsigned short lod;
    std::vector<unsigned int>
        indices_lod[5];
};