// #include "Snake.h"

// constexpr int cellSize = 20;     // Define or move this to a shared config file
// constexpr int cellCount = 20;   // Define or move this to a shared config file

// void Snake::Draw() {
//     for (unsigned int i = 0; i < body.size(); i++) {
//         float x = body[i].x;
//         float y = body[i].y;

//         Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};

//         DrawRectangleRounded(segment, 0.5, 6, DARKGREEN);
//     }
// }

// void Snake::Update() {
//     if (IsGrow) {
//         body.push_back(Vector2Subtract(body[body.size() - 1], 
//                       Vector2Subtract(body[body.size() - 1], body[body.size() - 2])));
//         IsGrow = false;
//     }
//     body.pop_back();
//     Vector2 next_position = Vector2Add(body[0], direction);

//     if (next_position.x >= cellCount) next_position.x = 0;
//     if (next_position.x < 0) next_position.x = cellCount - 1;
//     if (next_position.y >= cellCount) next_position.y = 0;
//     if (next_position.y < 0) next_position.y = cellCount - 1;

//     body.push_front(next_position);
// }