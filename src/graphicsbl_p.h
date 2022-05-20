#pragma once

#include <unordered_map>
#include "entity.h"
#include "camera.h"
#include "light.h"
#include "effects.h"


namespace bl::GraphicsBL {
	
	extern float  dtime;
	extern Camera camera;
	extern Bloom  bloom;

	extern Vec2u  size;
	extern Vec2u  mid;
	extern size_t bufferSize;

	extern std::vector<Vec3f> pixels;
	extern std::vector<float> depth;

	extern std::unordered_map<std::string, Model*>   models;
	extern std::unordered_map<std::string, Texture*> textures;

	extern std::vector<Entity*> entities;
	extern std::vector<Light*>  lights;
	extern std::vector<Effect*> effects;
	extern Vec3f light_ambient;
	extern Vec3f bg_color;

	void drawEntities();
	void getCollisions();

	int coordsToIndex(const Vec2& in);

}
