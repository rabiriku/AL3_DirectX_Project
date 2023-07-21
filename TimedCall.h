#pragma once
#include<functional>

class TimedCall {
public:
	//�R���X�g���N�^
	TimedCall(std::function<void(void)>f, uint32_t time);
	//�X�V
	void Update();
	//�����Ȃ�true��Ԃ�
	bool IsFinished() { return isComp; }

private:
	std::function<void(void)> f;
	uint32_t time;
	bool isComp;


};
