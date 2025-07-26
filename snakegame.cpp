#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <random>
#include <ctime>
#include <windows.h>
#include <chrono>
#define   Yin_Li_Kun      return 
char direction = 'D';  
int x,y;
int point = 0;
const int width = 20;
const int height = 20;
const int left = 10;
const int top = 10;
const int right = 10 + width;
const int bottom = 10 + height;
char map[height][width]={};
auto last_move = std::chrono::system_clock::now();
const int move = 200;


struct FoodPosition
{
    int x,y;
};
void gotoxy(short x, short y) 
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
struct SnakeBody
{
    int x,y;
};
SnakeBody snakehead;
SnakeBody body;
//食物的坐标表示
std::vector<FoodPosition> food;
//蛇头的生成函数
void snakehead_generate()
{
    
    snakehead.x=width/2;
    snakehead.y=height/2;
}
//蛇身的坐标表示(考虑用队列表示)
std::deque<SnakeBody> snakebody;
void body_generate()//初始化蛇身(包括蛇头)
{
    snakebody.clear();
    snakebody.push_front(snakehead);//蛇头
    for(int i = 1; i<=3 ;++ i)
    {   
        body.x = snakehead.x - i ;
        body.y = snakehead.y ;
        snakebody.push_back(body);

    }


}
//食物生成函数
void food_generate()
{
    FoodPosition newfood;
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<int> distributionx(1,width-2);
    static std::uniform_int_distribution<int> distributiony(1,height-2);
    newfood.x = distributionx(generator);
    newfood.y = distributiony(generator);
    food.push_back(newfood);
}
//地图边界,以及蛇的初始化
void map_update(int width,int height)
{
    for(int i = 0;i<height;++i)
        {
            for(int j = 0; j < width;++j)
            { 
                    if(i==0|| i == height-1 || j==0 || j == width-1)
                        map[i][j]= '#';   
                    else
                        map[i][j]= ' ';
            }
                
        }
    bool ishead = true;
    for (auto &it : snakebody)
        {
            if(ishead)
                {
                    map[it.y][it.x]= 'O';
                    ishead = false;
                }
                else
                {
                    map[it.y][it.x]='o';
                }

        }


            //更新食物(食物位置放这似不妥)
    for(auto &f :food)
        {
        map[f.y][f.x] = '*'; 
        }
} 
        // 更新蛇的长度,同时把食物也更新掉
void update()
{
    bool eatfood = false;
    for(auto it = food.begin(); it!= food.end();++it)
    {     
        if(snakehead.x == it->x && snakehead.y == it->y)//食到饭
         {
             eatfood = true; 
             food.erase(it); 
             food_generate(); 
             point += 5;
             break;
         }
    }
        SnakeBody new_head;
        new_head.x= snakehead.x;
        new_head.y= snakehead.y;
        snakebody.push_front(new_head);
        if(!eatfood) // 如果没有食到饭
        {
             snakebody.pop_back();
        }
       
}
void map_print()
{
    gotoxy(0,0); 
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0 ; j< width; ++j)
        {
            std::cout<<map[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}
        //形成游戏窗口
        void window(int left, int top, int right, int bottom);

void mani_direction() 
{
    if(GetAsyncKeyState('W') & 0x8000 && direction != 'S') 
    direction = 'W';
    if(GetAsyncKeyState('S') & 0x8000 && direction != 'W') 
    direction = 'S';
    if(GetAsyncKeyState('A') & 0x8000 && direction != 'D') 
    direction = 'A';
    if(GetAsyncKeyState('D') & 0x8000 && direction != 'A') 
    direction = 'D';
}
void move666() 
{
    if(direction == 'W') 
    snakehead.y--;
    if(direction == 'S') 
    snakehead.y++;
    if(direction == 'A') 
    snakehead.x--;
    if(direction == 'D') 
    snakehead.x++;
}
void gameover()
{

    if(snakehead.x <= 0 || snakehead.x >= width-1 ||
      snakehead.y <= 0 || snakehead.y >= height-1)
    {
      std::cout<<"Game Over"<<std::endl;
      throw std::runtime_error("Game Over");
    }
    else if(snakehead.x == snakebody.front().x && 
    snakehead.y == snakebody.front().y)
    {
      std::cout<<"Game Over"<<std::endl;
      throw std::runtime_error("Game Over");
    }
    // 检查蛇头是否与蛇身相撞
    for(auto it = snakebody.begin() + 1; it != snakebody.end();++it)
    {
        if(snakehead.x == it->x && snakehead.y == it->y)
        {
            std::cout<<"Game Over"<<std::endl;
            throw std::runtime_error("Game Over");
        }
    }

}
void Hide()
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0}); 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
int main()
{      
    Hide();   
    snakehead_generate();  
    food_generate();
    body_generate();
    
    while(true)
    { 
        mani_direction();
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_move);
        if(duration.count() >= move)
        {
            move666();
            gameover();
            update();
            map_update(width, height);
            map_print();
            last_move= now;
            std::cout<<"你的分数是:"<<point<<std::endl;
        }
       
        Sleep(5); 
    }
    Yin_Li_Kun 0;
}
//鲁棒性很强,暴赞！
