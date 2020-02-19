#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>	
#include "map.h"
#include <fstream>
#include <memory>
#include "game_manager.h"

bool KikoBSP::load_map(std::string file_name)
{
	this->file.open(file_name.c_str(), std::ios::in | std::ios::binary);

	if (this->file.is_open())
	{
		this->file.read(reinterpret_cast<char*>(&this->header), sizeof(this->header));

		BSPEntities* ents = new BSPEntities;
		ents->ents_array = new char[this->header.lumps[BSPLUMPS::ENTITIES].length];

		this->num_textures = this->header.lumps[BSPLUMPS::TEXTURES].length / sizeof(BSPTexture);
		this->num_planes = this->header.lumps[BSPLUMPS::PLANES].length / sizeof(BSPPlane);
		this->num_textures = this->header.lumps[BSPLUMPS::TEXTURES].length / sizeof(BSPTexture);
		this->num_nodes = this->header.lumps[BSPLUMPS::NODES].length / sizeof(BSPNode);
		this->num_leafs = this->header.lumps[BSPLUMPS::LEAFS].length / sizeof(BSPLeaf);
		this->num_leaf_faces = this->header.lumps[BSPLUMPS::LEAF_FACES].length / sizeof(BSPLeafFace);
		this->num_leaf_brushes = this->header.lumps[BSPLUMPS::LEAF_BRUSHES].length / sizeof(BSPLeafBrush);
		this->num_models = this->header.lumps[BSPLUMPS::MODELS].length / sizeof(BSPModel);
		this->num_brushes = this->header.lumps[BSPLUMPS::BRUSHES].length / sizeof(BSPBrush);
		this->num_brush_sides = this->header.lumps[BSPLUMPS::BRUSHSIDES].length / sizeof(BSPBrushSides);
		this->num_vertexes = this->header.lumps[BSPLUMPS::VERTEXES].length / sizeof(BSPVerts);
		this->num_meshverts = this->header.lumps[BSPLUMPS::MESHVERTS].length / sizeof(BSPMeshVerts);
		this->num_effects = this->header.lumps[BSPLUMPS::EFFECTS].length / sizeof(BSPEffects);
		this->num_faces = this->header.lumps[BSPLUMPS::FACES].length / sizeof(BSPFaces);

		this->textures = new BSPTexture[this->num_textures];
		this->planes = new BSPPlane[this->num_planes];
		this->nodes = new BSPNode[this->num_nodes];
		this->leafs = new BSPLeaf[this->num_leafs];
		this->leaf_faces = new BSPLeafFace[this->num_leaf_faces];
		this->leaf_brushes = new BSPLeafBrush[this->num_leaf_brushes];
		this->models = new BSPModel[this->num_models];
		this->brushes = new BSPBrush[this->num_brushes];
		this->brush_sides = new BSPBrushSides[this->num_brush_sides];
		this->vertexes = new BSPVerts[this->num_vertexes];
		this->mesh_verts = new BSPMeshVerts[this->num_mesh_verts];
		this->effects = new BSPEffects[this->num_effects];
		this->faces = new BSPFaces[this->num_faces];

		this->file.seekg(this->header.lumps[BSPLUMPS::ENTITIES].offset);
		this->file.read(reinterpret_cast<char*>(ents->ents_array), this->header.lumps[BSPLUMPS::ENTITIES].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::TEXTURES].offset);
		this->file.read(reinterpret_cast<char*>(this->textures), this->header.lumps[BSPLUMPS::TEXTURES].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::PLANES].offset);
		this->file.read(reinterpret_cast<char*>(this->planes), this->header.lumps[BSPLUMPS::PLANES].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::NODES].offset);
		this->file.read(reinterpret_cast<char*>(this->nodes), this->header.lumps[BSPLUMPS::NODES].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::LEAFS].offset);
		this->file.read(reinterpret_cast<char*>(this->leafs), this->header.lumps[BSPLUMPS::LEAFS].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::LEAF_FACES].offset);
		this->file.read(reinterpret_cast<char*>(this->leaf_faces), this->header.lumps[BSPLUMPS::LEAF_FACES].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::LEAF_BRUSHES].offset);
		this->file.read(reinterpret_cast<char*>(this->leaf_brushes), this->header.lumps[BSPLUMPS::LEAF_BRUSHES].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::MODELS].offset);
		this->file.read(reinterpret_cast<char*>(this->models), this->header.lumps[BSPLUMPS::MODELS].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::BRUSHES].offset);
		this->file.read(reinterpret_cast<char*>(this->brushes), this->header.lumps[BSPLUMPS::BRUSHES].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::BRUSHSIDES].offset);
		this->file.read(reinterpret_cast<char*>(this->brush_sides), this->header.lumps[BSPLUMPS::BRUSHSIDES].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::VERTEXES].offset);
		this->file.read(reinterpret_cast<char*>(this->vertexes), this->header.lumps[BSPLUMPS::VERTEXES].length);
		
		/*this->file.seekg(this->header.lumps[BSPLUMPS::MESHVERTS].offset);
		this->file.read(reinterpret_cast<char*>(mesh_verts), this->header.lumps[BSPLUMPS::MESHVERTS].length);
		
		this->file.seekg(this->header.lumps[BSPLUMPS::EFFECTS].offset);
		this->file.read(reinterpret_cast<char*>(effects), this->header.lumps[BSPLUMPS::EFFECTS].length);

		this->file.seekg(this->header.lumps[BSPLUMPS::FACES].offset);
		this->file.read(reinterpret_cast<char*>(faces), this->header.lumps[BSPLUMPS::FACES].length);
		*/
		
		std::printf("BSP VERSION: '%s'\n", this->header.magic);
		
		if (std::strncmp(this->header.magic, "IBSP", 4) == 0)
		{
			std::printf("SUCCESS: VALID BSP FORMAT!\n");
		}
		else
		{
			std::printf("ERROR: INVALID BSP FORMAT!\n");

			return false;
		}

		std::printf("this->num_of_verts == %i\n", this->num_vertexes);
		std::vector<float> rainbow_vector;

		for (int32_t x = 0; x <= this->num_vertexes; x++)
		{
			this->vertices.push_back(this->vertexes[x].position.x);
			this->vertices.push_back(this->vertexes[x].position.y); 
			this->vertices.push_back(this->vertexes[x].position.z);

			rainbow_vector.push_back(this->vertexes[x].position.x / 0xFF);
			rainbow_vector.push_back(this->vertexes[x].position.y / 0xFF);
			rainbow_vector.push_back(this->vertexes[x].position.z / 0xFF);

			/* rainbow_vectors.push_back(vertexes[x].position.x / 0x01); */
		}

		std::printf("this->vertices.size() == %i\n", this->vertices.size());

		this->shader.load_shader("shaders/bsp.vs", "shaders/bsp.fs");

		glGenVertexArrays(1, &this->vao);

		glBindVertexArray(this->vao);

		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices.front(), GL_STATIC_DRAW);

		glGenBuffers(1, &this->color_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->color_vbo);
		glBufferData(GL_ARRAY_BUFFER, rainbow_vector.size() * sizeof(float), &rainbow_vector.front(), GL_STATIC_DRAW);

		this->coord3d = glGetAttribLocation(this->shader.program, "coord3d");
		this->mvp = glGetUniformLocation(this->shader.program, "mvp");
		this->attrib_color = glGetAttribLocation(this->shader.program, "v_color");
	
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glVertexAttribPointer(this->coord3d, // attribute
			3,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the currentBlock of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			nullptr               // offset of first element
		);

		glBindBuffer(GL_ARRAY_BUFFER, this->color_vbo);
		glVertexAttribPointer(this->attrib_color,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			nullptr
		);
		
		glBindVertexArray(0);

		glVertexAttrib3fv(this->attrib_color, rainbow_vector.data());

		std::printf("size of vector = %i\n", this->vertices.size());

		delete ents;
		return true;
	}
	else
	{
		std::printf("ERROR: COULDN'T OPEN FILE!\n");

		return false;
	}

	return false;
}


void KikoBSP::render(glm::vec3 position)
{
	glBindVertexArray(this->vao);
	glEnableVertexAttribArray(this->coord3d);
	glEnableVertexAttribArray(this->attrib_color);

	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z));
	glm::mat4 mvp = game_manager->projection * game_manager->view * model;
	glUniformMatrix4fv(this->mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	
	glLineWidth(3.0);
	glDrawArrays(GL_LINES, 0, this->vertices.size());
		
	glDisableVertexAttribArray(this->coord3d);
	glDisableVertexAttribArray(this->attrib_color);
	glBindVertexArray(0);
}

void KikoBSP::cleanup_map()
{
	delete[] this->textures;
	delete[] this->planes;
	delete[] this->leafs;
	delete[] this->nodes;
	delete[] this->leaf_faces;
	delete[] this->models;
	delete[] this->brushes;
	delete[] this->brush_sides;
	delete[] this->vertexes;
	delete[] this->mesh_verts;
	delete[] this->effects;
	delete[] this->faces;
}
