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
	void setup();
public:
	int getCmd();
	int getArg();
	int legVirtual (int num);
	int servoVirtual (int num, int pos);
public:
	void moveForward(int num);
	void turn (int num);
private:
	int _mode;
	int _centerLeg;//the left top leg's rank
};
#endif