#pragma once
#include"Vector3.h"

class Collider {
private:
	//�Փ˔��a
	float radius_ = 1.0f;

public:
	//���a���擾
	int getradius() { return radius_; } 
	
	// ���a���擾
	void setradius(int radius_) { this->radius_ = radius_; }

	virtual void OnCollision();

	virtual Vector3 GetworldPosition();

};
