#pragma once
#pragma pack(2)

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <fstream>
#include "shader.h"
#include <memory>
#include <array>

enum BSPLUMPS
{
	ENTITIES,
	TEXTURES,
	PLANES,
	NODES,
	LEAFS,
	LEAF_FACES,
	LEAF_BRUSHES,
	MODELS,
	BRUSHES,
	BRUSHSIDES,
	VERTEXES,
	MESHVERTS,
	EFFECTS,
	FACES,
	LIGHTMAPS,
	LIGHTVOLS,
	VISDATA,
	MAX_LUMPS
};

struct BSPLump
{
	int32_t offset; /* offset to start of lump */
	int32_t length; /* length of lump, always multiple of 4 */
};

struct BSPHeader
{
	char magic[4]; /* ALWAYS IBSP */
	int32_t version; /* 0x2E for Quake 3 */
	BSPLump lumps[BSPLUMPS::MAX_LUMPS]; /* direntries */
};

struct BSPEntities
{
	char* ents_array;
};

struct BSPTexture
{
	char name[64];
	int32_t flags;
	int32_t contents;
};

struct BSPPlane
{
	glm::vec3 normal;
	float distance;
};

struct BSPNode
{
	int32_t plane;
	glm::ivec2 children;
	glm::ivec3 mins;
	glm::ivec3 maxs;
};

struct BSPLeaf
{
	int32_t cluster;
	int32_t area;
	glm::ivec3 mins;
	glm::ivec3 maxs;
	int32_t leafface;
	int32_t num_leaffaces;
	int32_t leaf_brush;
	int32_t num_leaf_brushes;
};

struct BSPLeafFace
{
	int32_t face;
};

struct BSPLeafBrush
{
	int32_t brush;
};

struct BSPModel
{
	glm::fvec3 mins;
	glm::fvec3 maxs;
	int32_t face;
	int32_t num_faces;
	int32_t brush;
	int32_t num_brushes;
};

struct BSPBrush
{
	int32_t brush_side;
	int32_t num_brush_sides;
	int32_t texture;
};

struct BSPBrushSides
{
	int32_t plane;
	int32_t texture;
};

struct BSPVerts
{
	glm::vec3 position;
	glm::vec2 tex_coord; /* same as float tex_coord[2][2] */
	glm::vec2 lm_coord; /* same as float tex_coord[2][2] */
	glm::vec3 normal;
	char color[4];
};

struct BSPMeshVerts
{
	int32_t offset;
};

struct BSPEffects
{
	char name[64];
	int32_t brush;
	int32_t unk; /* unknown */
};

struct BSPFaces
{
	int32_t texture;
	int32_t effect;
	int32_t type;
	int32_t vertex;
	int32_t num_vertexes;
	int32_t meshvert;
	int32_t num_of_meshverts;
	int32_t lm_index;
	glm::ivec2 lm_start;
	glm::ivec2 lm_size;
	glm::vec3 lm_origin;
	float lm_vecs[2][3];
	glm::fvec3 normal[3];
	glm::ivec2 size;
};

class KikoBSP
{
public:
	bool load_map(std::string);
	void render(glm::vec3);
	int32_t get_max();
	int32_t get_min();
	void get_vert();

	/* outdated function, has NO usage, was used when I was managing memory*/
	void cleanup_map();

	std::vector<BSPVerts> vertices_vector;

	uint32_t num_of_indices;
	std::vector<int32_t> indicides_vector;

	int32_t num_textures;
	int32_t num_planes;
	int32_t num_nodes;
	int32_t num_leafs;
	int32_t num_leaf_faces;
	int32_t num_leaf_brushes;
	int32_t num_models;
	int32_t num_brushes;
	int32_t num_brush_sides;
	int32_t num_vertexes;
	int32_t num_meshverts;
	int32_t num_effects;
	int32_t num_faces;
	int32_t num_lightmaps;
	int32_t num_lightvols;
	int32_t num_mesh_verts;

	BSPTexture* textures;
	BSPPlane* planes;
	BSPNode* nodes;
	BSPLeaf* leafs;
	BSPLeafFace* leaf_faces;
	BSPLeafBrush* leaf_brushes;
	BSPModel* models;
	BSPBrush* brushes;
	BSPBrushSides* brush_sides;
	BSPVerts* vertexes;
	BSPMeshVerts* mesh_verts;
	BSPEffects* effects;
	BSPFaces* faces;

	Shader shader;
	BSPHeader header;

private:
	std::ifstream file;
	uint32_t vbo;
	uint32_t vao;
	uint32_t color_vbo;

	int32_t coord3d;
	int32_t mvp;
	int32_t attrib_color;

	std::vector<float> vertices;
	std::vector<float> colors;
};