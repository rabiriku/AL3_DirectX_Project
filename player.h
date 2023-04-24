#pragma once
#include"GameScene.h"
class Player {
public:

void initialize();


void update();


void Draw();

private:

WorldTransform worldTransform_;

Model* model_ = nullptr;

uint32_t textureHundle_=0u;

Player* player_ = nullptr;
};
