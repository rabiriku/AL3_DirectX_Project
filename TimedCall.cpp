#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> f, uint32_t time) {}

void TimedCall::Update() {
	if (isComp = true) {
		return;
	}
	time--;
	if (time <= 0) {
		isComp = true;
		// �R�[���o�b�N�֐��Ăяo��


	}

}
