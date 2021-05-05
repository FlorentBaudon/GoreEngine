glm::vec2 findHorizontalIntersect(glm::vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY);

glm::vec2 findVerticalIntersect(glm::vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY);

void drawRaycast(glm::vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY);

void scanEnv(glm::vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY, float fov);