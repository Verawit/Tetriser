#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include <cmath>

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

    Music music;

    SoundBuffer scoreup;
    Sound scoreup_sound;
    scoreup_sound.setBuffer(scoreup);

    SoundBuffer over;
    Sound over_sound;
    over_sound.setBuffer(over); 


    if (!music.openFromFile("audio/bgmm.wav")) {
        std::cout << "ERROR";
    }

    if (!scoreup.loadFromFile("audio/scoreup.wav")) {
        std::cout << "ERROR";
    }

    if (!over.loadFromFile("audio/gameover.wav")) {
        std::cout << "ERROR";
    }

    
    music.play();
    music.setVolume(70);
    
    Texture texture1, texture2, texture3, texture4, texture5, texture6;
    texture1.loadFromFile("images/tiles.png");
    texture2.loadFromFile("images/background.png");
    texture3.loadFromFile("images/frame.png");
    texture4.loadFromFile("images/numbers.png");
    texture5.loadFromFile("images/gameover.png");
    texture6.loadFromFile("images/finalscore.png");

    Sprite s(texture1), background(texture2), frame(texture3), nums(texture4), gameover(texture5), scorefinal(texture6);

    int score = 0;
    int scoreDigits;
    int number[10] = { 0,1,2,3,4,5,6,7,8,9 };

    int comboCount = 0;
    int comboMultiplier = 1;
    float timeSinceLastClear = 0.0f;
    const float comboWindow = 2.0f;

    int xPos = 4; 
    bool rotate = 0;
    int colorNum = 1 + rand() % 7;
    float timer = 0, delay = 0.01;
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
                
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed) {
                    window.close();
                }
                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::R) {
                        std::cout << "restart";
                        score = 0;
                        comboCount = 0;
                        comboMultiplier = 1;
                        timeSinceLastClear = 0.0f;
                        gameOver = false;

                        // Clear the game grid
                        for (int i = 0; i < M; i++) {
                            for (int j = 0; j < N; j++) {
                                field[i][j] = 0;
                            }
                        }

                        // Set up a new Tetrimino piece
                        colorNum = 1 + rand() % 7;
                        n = rand() % 7;
                        for (int i = 0; i < 4; i++) {
                            a[i].x = block[n][i] % 2;
                            a[i].y = block[n][i] / 2;
                        }
                        for (int i = 0; i < 4; i++) {
                            a[i].x += 4;
                        }
                    }
                }
                if (gameOver == false) {
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
            }


            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                delay = 0.05;
            }

        if (gameOver == false) {
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
                    n = rand() % 7;
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
                            over_sound.play();
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
                else {
                    //นับคะแนน
                    scoreup_sound.play();
                    score += 100 * comboMultiplier;
                    comboCount++;
                    if (comboCount > 1) {
                        comboMultiplier++;
                    }

                    
                    std::string scoreStr = std::to_string(score);

                    timeSinceLastClear = 0.0f;
                    std::cout << score << std::endl;
                    
                }
            }

            timeSinceLastClear += time;

            if (timeSinceLastClear > comboWindow) {
                comboCount = 0;
                comboMultiplier = 1;
            }

            xPos = 0;
            rotate = 0;
            delay = 0.3;

            //วาดพื่นหลัง
            window.clear(Color::White);
            window.draw(background);

            scoreDigits = (log10(score) + 1);

            std::string scoreStr = std::to_string(score);

            for (int i = 0; i < scoreDigits; i++) {
                nums.setPosition(320, 80);
                nums.setTextureRect(IntRect((int(scoreStr[scoreStr.length() - i - 1]) - 48) * 18, 0, 18, 18));
                nums.move(i * -18, 0);
                window.draw(nums);
            }

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


            //วาดรูปบล็อกตามสีตอนถึงพื่น
            for (int i = 0; i < 4; i++) {
                s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
                s.setPosition(a[i].x * 18, a[i].y * 18);
                s.move(28, 31); //offset
                window.draw(s);
            }

            if (gameOver) {
                window.clear(Color::White);
                window.draw(background);
                window.draw(frame);
                gameover.setPosition(30, 100);
                window.draw(gameover);
                scorefinal.setPosition(30, 150);
                window.draw(scorefinal);
                for (int i = 0; i < scoreDigits; i++) {
                    nums.setPosition(125, 200);
                    nums.setTextureRect(IntRect((int(scoreStr[scoreStr.length() - i - 1]) - 48) * 18, 0, 18, 18));
                    nums.move(i * -18, 0);
                    window.draw(nums);
                }
                window.display();
                continue;
            }
            
            window.draw(frame);
            window.display();
        }
    }
    
    return 0;
}
