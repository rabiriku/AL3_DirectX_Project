#pragma once
#include"Vector3.h"

class Collider {
private:
	//Õ“Ë”¼Œa
	float radius_ = 1.0f;

public:
	//”¼Œa‚ðŽæ“¾
	int getradius() { return radius_; } 
	
	// ”¼Œa‚ðŽæ“¾
	void setradius(int radius_) { this->radius_ = radius_; }

	virtual void OnCollision();

	virtual Vector3 GetworldPosition();

};
