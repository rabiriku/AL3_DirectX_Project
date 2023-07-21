#pragma once
#include<functional>

class TimedCall {
public:
	//コンストラクタ
	TimedCall(std::function<void(void)>f, uint32_t time);
	//更新
	void Update();
	//完了ならtrueを返す
	bool IsFinished() { return isComp; }

private:
	std::function<void(void)> f;
	uint32_t time;
	bool isComp;


};
