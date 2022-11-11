#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <random>
#include <cstdlib>


// CLASSIC PONG
// BOUNCE THE BALL USING: "W" AND "S" OR USING ARROW UP AND DOWN (DEPENDS ON THE SIDE OF TWO PLAYERS). IF SOMEONE DON'T CATCH THE BALL, THE GAME WILL RESET. BY USING
// ENTER YOU WILL RESUME THE GAME



float GetRandomNumber(std::mt19937& random_generator, float from, float to) {
    std::uniform_real_distribution<float> dis(from, to);
    return dis(random_generator);
}
std::mt19937 random_generator(1337);


class Players
{
public:
    sf::RectangleShape sprite;
    Players(int pos_x, int pos_y)
    {
        sprite = sf::RectangleShape(sf::Vector2f(20, 100));
        sprite.setFillColor(sf::Color(255,0,0));
        sprite.setPosition(pos_x,pos_y);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }


    float player_speed = 0.5;

    void move_down()
    {
        sprite.move(0,player_speed);
    }

    void move_up()
    {
        sprite.move(0,-player_speed);
    }


    void collision()
    {
        if(sprite.getPosition().y >= 600-sprite.getSize().y)
        {
            sprite.move(0,-player_speed);
        }

        if(sprite.getPosition().y <= 0)
        {
            sprite.move(0,player_speed);
        }
    }

    float pos_x()
    {
        return sprite.getPosition().x;
    }

    float pos_y()
    {
        return sprite.getPosition().y;
    }
};

class Ball
{

public:
    sf::CircleShape sprite;
    Ball()
    {
        sprite = sf::CircleShape(15);
        sprite.setFillColor(sf::Color(0,150,0));
        sprite.setPosition(390,290);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }


    float speed_x = 0.2;
    float speed_y = 0.2;

    void movement()
    {
        sprite.move(speed_x,speed_y);
    }

    void collision()
    {


        if(sprite.getPosition().y <= 0)
        {
            speed_y *= -1;
        }

        if(sprite.getPosition().y >= 580)
        {
            speed_y *= -1;
        }

    }



    void out_of_map()
    {
        if(sprite.getPosition().x >= 780 || sprite.getPosition().x <= 0)
        {

            sprite.setPosition(390,290);
            speed_x = 0;
            speed_y = 0;

        }
    }

    float pos_x()
    {
        return sprite.getPosition().x;
    }

    float pos_y()
    {
        return sprite.getPosition().y;
    }



    void boucing_ball_off_the_paddles(Players& player1,Players& player2)
    {
        if(speed_x < 0)
        {
           if((pos_y()+15) >= player1.pos_y() && pos_y() <= (player1.pos_y() + 100))
           {
               if((pos_x()-15) <= (player1.pos_x()) && pos_x() >= (player1.pos_x()))
               {
                   speed_x *= -1;
                   float mid_y = player1.pos_y() + 50;
                   float diff_in_y = mid_y - pos_y();
                   float reducton = 50/0.2;
                   float new_vel_y = diff_in_y/reducton;
                   speed_y = -new_vel_y;
               }
           }
        }
        else
        {
            if((pos_y()+15) >= player2.pos_y() && pos_y() <= (player2.pos_y() + 100))
            {
                if((pos_x()+30) >= player2.pos_x() && pos_x() <= (player2.pos_x()))
                {
                    speed_x *= -1;
                    float mid_y = player2.pos_y() + 50;
                    float diff_in_y = mid_y - pos_y();
                    float reducton = 50/0.2;
                    float new_vel_y = diff_in_y/reducton;
                    speed_y = -new_vel_y;
                }
            }
        }
    }
};

void player_keyboard(Players& player1,Players& player2)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        player1.move_down();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        player1.move_up();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        player2.move_down();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player2.move_up();
    }
}

int main() {

    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");



    Players player1(100,250);
    Players player2(680,250);

    Ball ball;




    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && ball.speed_x == 0)
            {
                float rand_x = GetRandomNumber(random_generator,-1,1);
                float rand_y = GetRandomNumber(random_generator,-1,1);

                if(rand_x >= 0)
                {
                    ball.speed_x = 0.2;
                }
                else
                    ball.speed_x = -0.2;

                if(rand_y >= 0)
                {
                    ball.speed_y = 0.2;
                }
                else
                    ball.speed_y = -0.2;

            }
        }



        window.clear(sf::Color::Black);



        player1.draw(window);
        player1.collision();



        player2.draw(window);
        player2.collision();
        player_keyboard(player1, player2);


        ball.draw(window);
        ball.movement();
        ball.collision();
        ball.out_of_map();
        ball.boucing_ball_off_the_paddles(player1,player2);

        window.display();

    }

    return 0;
}
