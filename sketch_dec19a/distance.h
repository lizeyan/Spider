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
    void update();//update data
    void setup();//set up
    void echo (int rank = 0);//
    void echoAll();
    Distance();
  private:
    float distances[distanceNum];
};
#endif
