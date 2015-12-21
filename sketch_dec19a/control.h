#ifndef ___CONTROL___
#define ___CONTROL___

#define CONTROLED 1
#define FIND_LIGHT 2

#define OUT 2
#define MID 1
#define IN 0

#define MV 0
#define TL 1
#define TR 2
#define SC 3

#define LEGNUM 6
#define SERVONUM 18
//leg 0 ~ 5
//servo 0 ~ 17
class Control
{
public:
	Control (): _mode(CONTROLED) {}
	int mode() {return _mode;}
	void setup();//初始化
public:
	int getCmd();//从缓冲区取出一个命令
	int getArg();//从缓冲区取出一个数字
	int legVirtual (int num);//当前方向对应的num号腿实际上应该是哪个腿
	int servoVirtual (int num, int pos);//同上
public:
	void standUp();//站起来
	void moveForward(int num);//向前num步！！！
	void turn (int num);//旋转主方向
private:
	int _mode;
	int _centerLeg;//the left top leg's rank//当前主方向的0号腿实际是记号几号腿
/********************************
	^		0	5				相对于前进方向，前进的代码中统一认为腿的序号是这样的
	|	1			4			然后用legVirtual转换到实际的腿的编号
	|		2	3				转弯实际上就是变换虚拟的0号腿的位置
***************************************/
};	
#endif