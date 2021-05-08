#pragma once
class Raycaster
{
	/**** Members ****/
	public :
		int cellSize;
		int* map;
		int mapSizeX;
		int mapSizeY;
		int xResolution;
		int yResolution;

	/**** Functions ****/
	public :
		Raycaster(int cellSize, int* map, int mapSizeX, int mapSizeY, float fov, int xResolution, int yResolution);
		void scanEnv(const glm::vec2 pos, const float angle, const float fov);
		void testRaycasts(glm::vec2 pos, float angle, float fov, float precision =1);
	private :
		int checkCellValue(glm::vec2 p);
		glm::vec2 findHorizontalIntersect(glm::vec2 pos, float angle);
		glm::vec2 findVerticalIntersect(glm::vec2 pos, float angle);
		glm::vec2 drawRaycast(glm::vec2 pos, float angle, bool bDebug = false);
};

