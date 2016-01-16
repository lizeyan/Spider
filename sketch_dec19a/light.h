#ifndef ___LIGHT___
#define ___LIGHT___
const int lightNum = 3;
const int rank2pin[lightNum] = {0, 1, 2};
class Light
{
public:
	Light();
	int operator [] (int rank) {return lights[rank];}
	void setup();
	void echo(int rank);
	void echoAll();
	void update();
private:
	int lights[lightNum];
};

#endif