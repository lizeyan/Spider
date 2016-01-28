#ifndef ___CONTROL___
#define ___CONTROL___
//运行模式:
#define CONTROLED 1//受控
#define AUTO 2//自动寻光

#define OUT 2//每条腿最外侧的舵机
#define MID 1//每条腿中间的舵机
#define IN 0//每条腿最内侧的舵机

//命令
#define MV 0//move
#define TL 1//turn left
#define TR 2//turn right
#define SC 3//设置舵机角度
#define SL 6//设置关卡数
#define CHANGE_CONTROLED 4//改变到受控模式
#define CHANGE_AUTO 5//改变到自动模式

#define LEGNUM 6//腿的数目
#define SERVONUM 18//舵机的总数目
//leg 0 ~ 5
//servo 0 ~ 17
class Control
{
public:
	Control (): _mode(CONTROLED) {}
	int mode() {return _mode;}//当前模式
	int setMode (int x) {_mode = x;}//设置模式
	int getCenterLeg(){return _centerLeg;}//得到当前的前进方向
	void setup();//初始化
public:
	int getCmd();//从缓冲区取出一个命令
	int getArg();//从缓冲区取出一个数字
	int legVirtual (int num);//当前方向对应的num号腿实际上应该是哪个腿
	int servoVirtual (int num, int pos);//同上
public:
	void standUp();//站起来
	void moveForward(int num);//向前num步
	void turn (int num);//旋转主方向
	void turnTo (int rank);//旋转到某个方向
private:
	int _mode;
	int _centerLeg;//the left top leg's rank//当前主方向的0号腿实际是几号腿
/********************************
	^		0	5				相对于前进方向，前进的代码中统一认为腿的序号是这样的
	|	1			4			然后用legVirtual转换到实际的腿的编号
	|		2	3				转弯实际上就是变换虚拟的0号腿的位置
***************************************/
};	
#endif