#pragma once
class Player
{
	/**** Members ****/
	public :
		//Position of the player on map
		glm::vec2 position;
		// direction where the player looking
		glm::vec2 forward;
		// right direction of the player
		glm::vec2 right;
		// angle in RADIAN 
		float angle;

		float speed;

	private : 
		glm::vec2 worldForward;
		glm::vec2 worldRight;
		glm::mat4 rotationMatrix;


	/**** Functions ****/
	public:
		Player(glm::vec2 position, glm::vec2 worldForward, glm::vec2 worldRight, float angle);

		void turn(float angle);
		void move(glm::vec2 direction);
	
	private :
		void refreshPlayerDatas();

};

