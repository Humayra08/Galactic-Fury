#include "iGraphics.h"
#include <vector>
using namespace std;

// Global variables
int homePage = 1, menuPage = 2;
int startButtonClick = 0;
int levelOnePage = 0;
int storyPage = 0;  // New variable for story page
int aboutPage = 0;  // New variable for about page

char levelOneCharacter[6][100] = { "Character\\img01.bmp", "Character\\img02.bmp", "Character\\img03.bmp", "Character\\img04.bmp", "Character\\img05.bmp", "Character\\img06.bmp", };
char jumpCharacter[6][100] = { "Character\\01.bmp", "Character\\02.bmp", "Character\\03.bmp", "Character\\04.bmp", "Character\\05.bmp", "Character\\06.bmp" };

int characterPosX = 10, characterPosY = 40; // Initial X position and Y position of the character
int characterFrameIndex = 0; // Initial frame index
int frameCounter = 0; // Counter to control frame rate
bool isJumping = false; // Flag to check if character is jumping
int jumpHeight = 0; // Height of the jump
int jumpDirection = 1; // 1 for going up, -1 for coming down
int currentHealthIndex = 10; // Start with full health (index 10)
bool isGameOver = false;

// Add the path to your sound file
const char* clickSound = "Sounds\\Click.wav";

char healthBarSprites[11][100] = {"Objects\\01.bmp", "Objects\\02.bmp", "Objects\\03.bmp","Objects\\04.bmp", "Objects\\05.bmp", "Objects\\06.bmp","Objects\\07.bmp", "Objects\\08.bmp", "Objects\\09.bmp","Objects\\10.bmp", "Objects\\11.bmp"};

// Function for playing the click sound
void playClickSound() {
	PlaySound(clickSound, NULL, SND_ASYNC);
}

// Enemy properties
struct Enemy {
	int x, y;
	int frameIndex;//current frame of the enemy
	bool isActive;
	int moveCounter; // Counter to control movement speed
	int frameCounter; // Counter to control animation speed
	char spriteSheet[17][100]; // Sprite sheet for the enemy
	int spriteCount; // Number of sprites in the sprite sheet
};

vector<Enemy> enemies;

char enemy1Sprites[11][100] = { "Enemy1\\img01.bmp", "Enemy1\\img02.bmp", "Enemy1\\img03.bmp", "Enemy1\\img04.bmp", "Enemy1\\img05.bmp", "Enemy1\\img06.bmp", "Enemy1\\img07.bmp", "Enemy1\\img08.bmp", "Enemy1\\img09.bmp", "Enemy1\\img10.bmp", "Enemy1\\img11.bmp" };
char enemy2Sprites[11][100] = { "Enemy2\\img01.bmp", "Enemy2\\img02.bmp", "Enemy2\\img03.bmp", "Enemy2\\img04.bmp", "Enemy2\\img05.bmp", "Enemy2\\img06.bmp", "Enemy2\\img07.bmp", "Enemy2\\img08.bmp", "Enemy2\\img09.bmp", "Enemy2\\img10.bmp", "Enemy2\\img11.bmp" };
char enemy3Sprites[15][100] = { "Enemy3\\img01.bmp", "Enemy3\\img02.bmp", "Enemy3\\img03.bmp", "Enemy3\\img04.bmp", "Enemy3\\img05.bmp", "Enemy3\\img06.bmp", "Enemy3\\img07.bmp", "Enemy3\\img08.bmp", "Enemy3\\img09.bmp", "Enemy3\\img10.bmp", "Enemy3\\img11.bmp", "Enemy3\\img12.bmp", "Enemy3\\img13.bmp", "Enemy3\\img14.bmp", "Enemy3\\img15.bmp" };
char enemy4Sprites[17][100] = { "Enemy4\\img01.bmp", "Enemy4\\img02.bmp", "Enemy4\\img03.bmp", "Enemy4\\img04.bmp", "Enemy4\\img05.bmp", "Enemy4\\img06.bmp", "Enemy4\\img07.bmp", "Enemy4\\img08.bmp", "Enemy4\\img09.bmp", "Enemy4\\img10.bmp", "Enemy4\\img11.bmp", "Enemy4\\img12.bmp", "Enemy4\\img13.bmp", "Enemy4\\img14.bmp", "Enemy4\\img15.bmp", "Enemy4\\img16.bmp", "Enemy4\\img17.bmp" };

// Function for showing the Starting page of the game (Galactic Fury Poster)
void drawHomepage() {
	iSetColor(128, 128, 128);
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "Background\\img01.bmp", 0);
}

// Function for showing the Menu page
void drawMenuPage() {
	iSetColor(255, 255, 255);
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "Menu\\img01.bmp", 0);
}

// Function for showing the story page
void drawStoryPage() {
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "Background\\story.bmp", 0);  // Story background image
	iShowBMP2(20, 600, "Menu\\backk.bmp", 0);   // Back button
}

// Function for showing the about page
void drawAboutPage() {
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "Background\\abt.bmp", 0);  // About background image
	iShowBMP2(20, 600, "Menu\\backk.bmp", 0);
}

void drawGameOverPage() {
	// Fill the screen with a background color or image
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "Menu\\over.bmp", 0); // Game over background
}

// Function for handling the start button click
void startButtonClickHandler() {
	homePage = 0;
	menuPage = 0;
	levelOnePage = 1;
}

// Function for handling the story button click
void storyButtonClickHandler() {
	homePage = 0;
	menuPage = 0;
	storyPage = 1;
}

// Function for handling the about button click
void aboutButtonClickHandler() {
	homePage = 0;
	menuPage = 0;
	aboutPage = 1;
}

// Function for handling the back button click
void backButtonClickHandler() {
	homePage = 0;
	menuPage = 2;
	aboutPage = 0;
	storyPage = 0;
	levelOnePage = 0;
}

// Function for handling the exit button click
void exitButtonClickHandler() {
	exit(0);
}

// Bullet properties
struct Bullet {
	int x, y;
	bool isActive;
};

vector<Bullet> bullets;

// Function for initializing the bullet
Bullet initializeBullet(int x, int y) {
	Bullet bullet;
	bullet.x = x;
	bullet.y = y;
	bullet.isActive = true;
	return bullet;
}

// Function to fire a bullet
void fireBullet() {
	int gunOffsetX = 130; // gun's X position on the character sprite
	int gunOffsetY = 125; // gun's Y position on the character sprite
	Bullet newBullet = initializeBullet(characterPosX + gunOffsetX, characterPosY + gunOffsetY); // Fire from the gun's position
	bullets.push_back(newBullet);

	// Play gunshot sound
	PlaySound("Sounds\\gunshot.wav", NULL, SND_ASYNC);
}

// Function for initializing the enemies
void initializeEnemies() {
	for (int i = 0; i < 2; i++) {
		Enemy newEnemy;
		newEnemy.x = 1280 + i * 700; // Stagger the initial positions
		newEnemy.y = 95;
		newEnemy.frameIndex = 0;
		newEnemy.isActive = true;
		newEnemy.moveCounter = 0;
		newEnemy.frameCounter = 0;
		memcpy(newEnemy.spriteSheet, enemy1Sprites, sizeof(enemy1Sprites));
		newEnemy.spriteCount = 11;
		enemies.push_back(newEnemy);
	}
	for (int i = 0; i < 5; i++) {
		Enemy newEnemy;
		newEnemy.x = 1500 + i * 500; // Stagger the initial positions
		newEnemy.y = 95;
		newEnemy.frameIndex = 0;
		newEnemy.isActive = true;
		newEnemy.moveCounter = 0;
		newEnemy.frameCounter = 0;
		memcpy(newEnemy.spriteSheet, enemy2Sprites, sizeof(enemy2Sprites));
		newEnemy.spriteCount = 11;
		enemies.push_back(newEnemy);
	}
	for (int i = 0; i < 5; i++) {
		Enemy newEnemy;
		newEnemy.x = 1700 + i * 900; // Stagger the initial positions
		newEnemy.y = 100;
		newEnemy.frameIndex = 0;
		newEnemy.isActive = true;
		newEnemy.moveCounter = 0;
		newEnemy.frameCounter = 0;
		memcpy(newEnemy.spriteSheet, enemy3Sprites, sizeof(enemy3Sprites));
		newEnemy.spriteCount = 15;
		enemies.push_back(newEnemy);
	}
	for (int i = 0; i < 5; i++) {
		Enemy newEnemy;
		newEnemy.x = 1900 + i * 500; // Stagger the initial positions
		newEnemy.y = 200;
		newEnemy.frameIndex = 0;
		newEnemy.isActive = true;
		newEnemy.moveCounter = 0;
		newEnemy.frameCounter = 0;
		memcpy(newEnemy.spriteSheet, enemy4Sprites, sizeof(enemy4Sprites));
		newEnemy.spriteCount = 17;
		enemies.push_back(newEnemy);
	}
}

// Function to update the enemy positions and animations
void updateEnemies() {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i].isActive) {
			enemies[i].moveCounter++;
			enemies[i].frameCounter++;

			if (enemies[i].moveCounter >= 3) { // Controls the movement speed
				enemies[i].x -= 8; // Move the enemy to the left
				enemies[i].moveCounter = 0;
				if (enemies[i].x <= characterPosX + 30) {
					enemies[i].isActive = false; // Deactivate the enemy when it goes off screen

					// Decrease health if enemy reaches the character
					if (currentHealthIndex > 0) {
						currentHealthIndex--;
					}
				}
			}

			if (enemies[i].frameCounter >= 5) { // Controls the animation speed
				enemies[i].frameIndex = (enemies[i].frameIndex + 1) % enemies[i].spriteCount; // Update the frame index for animation
				enemies[i].frameCounter = 0;
			}

			// Check for collisions with bullets
			for (int j = 0; j < bullets.size(); j++) {
				if (bullets[j].isActive) {
					if (bullets[j].x >= enemies[i].x && bullets[j].x <= enemies[i].x + 70 && bullets[j].y >= enemies[i].y && bullets[j].y <= enemies[i].y + 70) { // assuming enemy height of 80
						// Bullet hit the enemy
						enemies[i].isActive = false; // Deactivate the enemy
						bullets[j].isActive = false; // Deactivate the bullet
					}
				}
			}
		}
	}
}

// Function to handle jump
void updateJump() {
	if (isJumping) {
		characterPosY += 8 * jumpDirection;
		jumpHeight += 8 * jumpDirection;

		if (jumpHeight >= 150) {
			jumpDirection = -1; // Start falling down
		}
		else if (jumpHeight <= 0) {
			isJumping = false; // End of jump
			jumpDirection = 1; // Reset direction for next jump
			jumpHeight = 0;
		}

		// Update jump animation
		characterFrameIndex = (characterFrameIndex + 1) % 6;
	}
}

// Function for showing the level one page
void drawLevelOnePage() {
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "Background\\img03.bmp", 0);
	iShowBMP2(20, 600, "Menu\\backk.bmp", 0);

	// Draw the character at the current position with the current frame
	// Draw the character at the current position with the current frame
	if (isJumping) {
		iShowBMP2(characterPosX, characterPosY, jumpCharacter[characterFrameIndex], 0);
	}
	else {
		iShowBMP2(characterPosX, characterPosY, levelOneCharacter[characterFrameIndex], 0);
	}

	// Draw the health bar
	iShowBMP2(1020, 600, healthBarSprites[currentHealthIndex], 0);

	// Draw active bullets
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].isActive) {
			iShowBMP2(bullets[i].x, bullets[i].y, "Objects\\img01.bmp", 0);
			bullets[i].x += 20; // Move the bullet to the right

			// Deactivate the bullet if it goes off the screen
			if (bullets[i].x >= 900) {
				bullets[i].isActive = false;
			}
		}
	}

	// Draw and update the enemies
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i].isActive) {
			iShowBMP2(enemies[i].x, enemies[i].y, enemies[i].spriteSheet[enemies[i].frameIndex], 0);
		}
	}
	updateEnemies();
	updateJump();
}

void checkGameOver() {
	if (currentHealthIndex <= 0) {
		isGameOver = true; // Set the game over flag
		levelOnePage = 0;  // Stop the level one page
	}
}

void iDraw() {
	iClear();
	if (isGameOver) {
		drawGameOverPage(); // Draw the game over page
	}
	if (homePage == 1) {
		drawHomepage();
	}
	else if (menuPage == 2) {
		drawMenuPage();
	}
	else if (levelOnePage == 1) {
		drawLevelOnePage();
		checkGameOver(); // Check if the game is over
	}
	else if (storyPage == 1) {
		drawStoryPage();
	}
	else if (aboutPage == 1) {
		drawAboutPage();
	}
}

void iMouseMove(int mx, int my) {
}

void iPassiveMouseMove(int mx, int my) {
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			playClickSound();  //click sound
		printf("x=%d y=%d", mx, my);

		if (isGameOver) {
			// Check if "Play Again" button is clicked
			if (mx >= 492 && mx <= 784 && my >= 325 && my <= 413) {
				// Restarting the game
				isGameOver = false;
				currentHealthIndex = 10; // Reset health
				characterPosX = 10; // Reset character X position
				characterPosY = 40; // Reset character Y position
				characterFrameIndex = 0; // Reset character frame index
				frameCounter = 0; // Reset frame counter
				isJumping = false; // Reset jump status
				jumpHeight = 0; // Reset jump height
				jumpDirection = 1; // Reset jump direction

				bullets.clear(); // Clear all bullets
				enemies.clear(); // Clear all enemies

				initializeEnemies(); // Reinitialize enemies

				homePage = 0;
				menuPage = 0;
				levelOnePage = 1;

				drawLevelOnePage(); // Draw the level one page
			}

			// Quit button
			if (mx >= 490 && mx <= 784 && my >= 167 && my <= 250) {
				exit(0); // Exit the game
			}
		}


		if (menuPage == 2) {
			if ((mx >= 198 && mx <= 448) && (my >= 419 && my <= 499)) {
				startButtonClickHandler();  // Start button
			}
			// else if ((mx >= 198 && mx <= 448) && (my >= 256 && my <= 340)) {
			// levelOnePage = 1;  // ScoreBoard button
			// menuPage = 0;
			// }
			else if ((mx >= 198 && mx <= 448) && (my >= 99 && my <= 178)) {
				storyButtonClickHandler();  // Story button
			}
			else if ((mx >= 806 && mx <= 1062) && (my >= 261 && my <= 340)) {
				aboutButtonClickHandler();  // About button
			}
			else if ((mx >= 806 && mx <= 1062) && (my >= 97 && my <= 182)) {
				exitButtonClickHandler();  // Exit button
			}
		}
		else if ((menuPage == 2 || levelOnePage == 1 || aboutPage == 1 || storyPage == 1) && (mx >= 20 && mx <= 120) && (my >= 600 && my <= 700)) {
			backButtonClickHandler();
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
	}
}

void iKeyboard(unsigned char key) {
	if (key == '\r') {  // '\r' is the ASCII code for Enter
		if (homePage == 1) {
			homePage = 2;
		}
	}
	if (key == ' ') {  // Space key for firing bullet
		fireBullet();
	}
}
void iSpecialKeyboard(unsigned char key) {
	if (levelOnePage == 1) {
		if (key == GLUT_KEY_RIGHT) {
			characterPosX += 10; // Move right by 10 units
			frameCounter++;
		}
		if (key == GLUT_KEY_LEFT) {
			characterPosX -= 10; // Move left by 10 units
			frameCounter++;
		}
		if (key == GLUT_KEY_UP) {  // Jump when UP key is pressed
			if (!isJumping) {
				isJumping = true;  // Start the jump
				characterFrameIndex = 0; // Reset jump animation frame
			}
		}

		// Update character frame
		if (frameCounter >= 1) { // Indicates the animation speed
			characterFrameIndex = (characterFrameIndex + 1) % 6;
			frameCounter = 0;
		}
	}
}


int main() {
	iInitialize(1280, 720, "Galactic Fury");
	initializeEnemies(); // Initialize all enemies
	iStart();
	return 0;
}
