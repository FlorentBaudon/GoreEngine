glm::vec2 findFirstPoint(glm::vec2 p, glm::vec2 fwd, float angle);

glm::vec2 findHorizontalIntersect(glm::vec2 pos, glm::vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY);

glm::vec2 findVerticalIntersect(glm::vec2 pos, glm::vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY);

void drawRaycast(glm::vec2 pos, glm::vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY);

void scanEnv(glm::vec2 pos, glm::vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY, float fov);