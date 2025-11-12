#include <iostream>
#include <vector>
#include <conio.h>  // for _kbhit() and _getch() on Windows
#include <windows.h> // for Windows console functions
#include <cstdlib> // for rand()
#include <ctime>   // for time()

using namespace std;

// Game constants
const int BOARD_WIDTH = 20;
const int BOARD_HEIGHT = 20;
const int DELAY = 100; // Delay in milliseconds to control game speed

// Direction constants
enum Direction {
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Position structure
struct Position {
    int x;
    int y;
    
    // Check if two positions are equal
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class Snake {
private:
    vector<Position> body;  // Snake body positions (head is at index 0)
    Direction dir;
    int score;

public:
    // Constructor
    Snake(int startX, int startY) {
        body.push_back({startX, startY});  // Head
        dir = RIGHT;  // Start moving right
        score = 0;
    }

    // Get head position
    Position getHead() const {
        return body[0];
    }

    // Get the direction
    Direction getDirection() const {
        return dir;
    }

    // Set the direction
    void setDirection(Direction d) {
        // Prevent 180-degree turns
        if ((d == LEFT && dir != RIGHT) || 
            (d == RIGHT && dir != LEFT) || 
            (d == UP && dir != DOWN) || 
            (d == DOWN && dir != UP)) {
            dir = d;
        }
    }

    // Move the snake
    void move() {
        if (dir == STOP) return;

        Position head = body[0];
        
        // Calculate new head position based on direction
        switch(dir) {
            case LEFT:
                head.x--;
                break;
            case RIGHT:
                head.x++;
                break;
            case UP:
                head.y--;
                break;
            case DOWN:
                head.y++;
                break;
        }

        // Insert new head at the beginning
        body.insert(body.begin(), head);
        
        // Remove the tail (last element)
        body.pop_back();
    }

    // Grow the snake (when eating food)
    void grow() {
        if (body.size() > 0) {
            // Add a new segment at the position of the current tail
            Position tail = body[body.size() - 1];
            body.push_back(tail);
            score += 10;
        }
    }

    // Check if snake collides with itself
    bool checkSelfCollision() const {
        Position head = getHead();
        for (int i = 1; i < body.size(); i++) {
            if (head == body[i]) {
                return true;
            }
        }
        return false;
    }

    // Get the snake's score
    int getScore() const {
        return score;
    }

    // Get snake body
    const vector<Position>& getBody() const {
        return body;
    }

    // Reset the snake
    void reset(int startX, int startY) {
        body.clear();
        body.push_back({startX, startY});
        dir = RIGHT;  // Start moving right
        score = 0;
    }
};

class Food {
private:
    Position pos;

public:
    Food() {
        // Initialize with a random position
        generate();
    }

    // Get food position
    Position getPosition() const {
        return pos;
    }

    // Generate new random position for food
    void generate() {
        pos.x = rand() % BOARD_WIDTH;
        pos.y = rand() % BOARD_HEIGHT;
    }
};

class GameBoard {
private:
    char board[BOARD_HEIGHT][BOARD_WIDTH];

public:
    // Clear the board
    void clear() {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                board[i][j] = ' ';
            }
        }
    }

    // Draw the snake on the board
    void drawSnake(const Snake& snake) {
        const vector<Position>& body = snake.getBody();
        for (int i = 0; i < body.size(); i++) {
            if (body[i].x >= 0 && body[i].x < BOARD_WIDTH && 
                body[i].y >= 0 && body[i].y < BOARD_HEIGHT) {
                // Head is represented by 'O', body segments by 'o'
                board[body[i].y][body[i].x] = (i == 0) ? 'O' : 'o';
            }
        }
    }

    // Draw the food on the board
    void drawFood(const Food& food) {
        Position foodPos = food.getPosition();
        if (foodPos.x >= 0 && foodPos.x < BOARD_WIDTH && 
            foodPos.y >= 0 && foodPos.y < BOARD_HEIGHT) {
            board[foodPos.y][foodPos.x] = 'F';
        }
    }

    // Draw the border
    void drawBorder() {
        // Top border
        for (int i = 0; i < BOARD_WIDTH + 2; i++) {
            cout << "#";
        }
        cout << endl;

        // Middle rows
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            cout << "#";  // Left border
            for (int j = 0; j < BOARD_WIDTH; j++) {
                cout << board[i][j];
            }
            cout << "#";  // Right border
            cout << endl;
        }

        // Bottom border
        for (int i = 0; i < BOARD_WIDTH + 2; i++) {
            cout << "#";
        }
        cout << endl;
    }

    // Display the game board
    void display(const Snake& snake, const Food& food, int score) {
        clear();
        drawSnake(snake);
        drawFood(food);
        
        // Clear the console and draw the border
        system("cls");  // Clear the console on Windows
        
        cout << "Score: " << score << endl;
        drawBorder();
    }
};

// Function to check if snake has collided with walls
bool checkWallCollision(const Snake& snake) {
    Position head = snake.getHead();
    
    // Check if head is outside the board boundaries
    return (head.x < 0 || head.x >= BOARD_WIDTH || 
            head.y < 0 || head.y >= BOARD_HEIGHT);
}

// Function to check if snake has eaten the food
bool checkFoodCollision(const Snake& snake, const Food& food) {
    return snake.getHead() == food.getPosition();
}

// Function to handle user input
void handleInput(Snake& snake) {
    if (_kbhit()) {  // Check if a key has been pressed
        switch (_getch()) {  // Get the pressed key
            case 'a':
            case 'A':
            case 75:  // Left arrow key
                snake.setDirection(LEFT);
                break;
            case 'd':
            case 'D':
            case 77:  // Right arrow key
                snake.setDirection(RIGHT);
                break;
            case 'w':
            case 'W':
            case 72:  // Up arrow key
                snake.setDirection(UP);
                break;
            case 's':
            case 'S':
            case 80:  // Down arrow key
                snake.setDirection(DOWN);
                break;
            case 'x':
            case 'X':  // Exit game
                snake.setDirection(STOP);
                break;
        }
    }
}

// Main game function
void runGame() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));
    
    // Initialize game objects
    Snake snake(BOARD_WIDTH / 2, BOARD_HEIGHT / 2);  // Start in the middle
    Food food;
    GameBoard board;
    
    bool gameOver = false;
    
    while (!gameOver) {
        // Handle user input
        handleInput(snake);
        
        // Check if game should exit
        if (snake.getDirection() == STOP) {
            gameOver = true;
            break;
        }
        
        // Check for food collision before moving
        if (checkFoodCollision(snake, food)) {
            snake.grow();  // Grow the snake
            food.generate();  // Generate new food
            
            // Make sure food doesn't appear on snake
            const vector<Position>& snakeBody = snake.getBody();
            bool foodOnSnake = true;
            while (foodOnSnake) {
                foodOnSnake = false;
                for (const Position& pos : snakeBody) {
                    if (pos == food.getPosition()) {
                        food.generate();
                        foodOnSnake = true;
                        break;
                    }
                }
            }
        }
        
        // Move the snake
        snake.move();
        
        // Check for collisions
        if (checkWallCollision(snake) || snake.checkSelfCollision()) {
            gameOver = true;
        }
        
        // Display the updated game state
        board.display(snake, food, snake.getScore());
        
        // Add a delay to control game speed
        Sleep(DELAY);
    }
    
    // Display game over message
    system("cls");
    cout << "Game Over!" << endl;
    cout << "Final Score: " << snake.getScore() << endl;
    cout << "Press any key to exit...";
    _getch();
}

int main() {
    cout << "Welcome to Snake Game!" << endl;
    cout << "Use arrow keys or WASD to control the snake." << endl;
    cout << "Press X to exit the game." << endl;
    cout << "Press any key to start...";
    _getch();
    
    runGame();
    
    return 0;
}