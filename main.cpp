#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point {
    int x, y;
};

Point a[4], b[4];

//
int block[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

bool checkCollision() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) {
            return 1;
        }else if (field[a[i].y][a[i].x]) {
            return 1;
        }
    }

    return 0;
};


int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(400, 480), "TetrisTetruy");

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");

    Sprite s(t1), background(t2), frame(t3);

    int xPos = 4; 
    bool rotate = 0;
    int colorNum = 1 + rand() % 7;
    float timer = 0, delay = 0.3;
    bool gameOver = false;

    Clock clock;

    int n = rand() % 7;
    for (int i = 0;i < 4;i++) {
        a[i].x = block[n][i] % 2;
        a[i].y = block[n][i] / 2;
    }
    //game start

    while (window.isOpen())
    {
        if(gameOver == false){
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;

            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed) {
                    window.close();
                }

                if (e.type == Event::KeyPressed)
                    if (e.key.code == Keyboard::Up) {
                        rotate = true;
                    }
                    else if (e.key.code == Keyboard::Left) {
                        xPos = -1;
                    }
                    else if (e.key.code == Keyboard::Right) {
                        xPos = 1;
                    }
            }

            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                delay = 0.05;
            }

            //ขยับ
            for (int i = 0;i < 4;i++) {
                b[i] = a[i];
                a[i].x += xPos;
            }
            if (checkCollision() == true) {
                for (int i = 0;i < 4;i++) {
                    a[i] = b[i];
                }
            }
            //หมุน
            if (rotate) {
                Point center = a[1]; //center of rotation
                for (int i = 0; i < 4; i++) {
                    int x = a[i].y - center.y;
                    int y = a[i].x - center.x;
                    a[i].x = center.x - x;
                    a[i].y = center.y + y;
                }
                if (checkCollision() == true) {
                    for (int i = 0;i < 4;i++) {
                        a[i] = b[i];
                    }
                }
            }

            //Tick
            if (timer > delay) {
                for (int i = 0;i < 4;i++) {
                    b[i] = a[i];
                    a[i].y += 1;
                }

                if (checkCollision() == true) {
                    for (int i = 0; i < 4; i++) {
                        field[b[i].y][b[i].x] = colorNum;
                    }


                    colorNum = 1 + rand() % 7;
                    int n = rand() % 7;
                    for (int i = 0;i < 4;i++) {
                        a[i].x = block[n][i] % 2;
                        a[i].y = block[n][i] / 2;
                    }
                    for (int i = 0;i < 4;i++) {
                        a[i].x += 4;
                    }

                    //gameover
                    for (int i = 0; i < 4; i++) {
                        if (field[a[i].y][a[i].x] != 0) {
                            std::cout << "GAMEOVER ";
                            gameOver = true;

                        }
                    }

                }

                timer = 0;
            }

            //เช็คแถว
            int k = M - 1;
            for (int i = M - 1;i > 0;i--) {
                int count = 0;
                for (int j = 0;j < N;j++) {
                    if (field[i][j]) {
                        count++;
                    }
                    field[k][j] = field[i][j];
                }
                if (count < N) {
                    k--;
                }
            }

            xPos = 0;
            rotate = 0;
            delay = 0.3;

            //วาดพื่นหลัง
            window.clear(Color::White);
            window.draw(background);

            for (int i = 0;i < M;i++) {
                for (int j = 0;j < N;j++) {
                    if (field[i][j] == 0) {
                        continue;
                    }
                    s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                    s.setPosition(j * 18, i * 18);
                    s.move(28, 31); //offset
                    window.draw(s);
                }
            }

            for (int i = 0; i < 4; i++) {
                s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
                s.setPosition(a[i].x * 18, a[i].y * 18);
                s.move(28, 31); //offset
                window.draw(s);
            }

            window.draw(frame);
            window.display();
        }
    }
    
    return 0;
}
