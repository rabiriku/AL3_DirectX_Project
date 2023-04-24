#pragma once
#include "Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"

class Player {
public:

void initialize(Model* model, uint32_t textureHandle);


void update();


void Draw(ViewProjection&viewProjection);

private:

WorldTransform worldTransform_;

Model* model_ = nullptr;

uint32_t textureHundle_=0u;


};
