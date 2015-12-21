#ifndef ___DISTANCE___
#define ___DISTANCE___
const int distanceNum = 3;
const int distanceIn[distanceNum] = {2, 4, 6};
const int distanceOut[distanceNum] = {3, 5, 7};
class Distance
{
  public:
    float distance(int rank = 0) {
      return distances[rank];
    }
    float operator[] (int rank) {
      return distances[rank];
    }
    void update();//更新距离，每回合都应当调用一次
    void setup();//初始化
    void echo (int rank = 0);//在蓝牙上显示
    void echoAll();//显示全部
    Distance();
  private:
    float distances[distanceNum];
};
#endif
