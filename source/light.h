#ifndef ___LIGHT___
#define ___LIGHT___
const int lightNum = 3;
const int rank2pin[lightNum] = {0, 1, 2};
class Light
{
public:
	Light();
	//此处返回的不是真正的光强，而是一个电压值，这个数字越小光强越大
	int operator [] (int rank) {return lights[rank];}//得到光强
	void setup();//初始化
	void echo(int rank);//显示摸个光敏传感器的数值
	void echoAll();//显示所有光敏传感器的数值
	void update();//更新光敏传感器的数值
private:
	int lights[lightNum];
};

#endif