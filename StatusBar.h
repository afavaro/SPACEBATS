#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "HUDComponent.h"
#include "Framework.h"
#include "Shader.h"

#include "btBulletDynamicsCommon.h"

class StatusBar : public HUDComponent{

public:
	StatusBar(Shader* shader, btVector4 rect);
	~StatusBar();

	void render();
		
	void setValue(float val);
	float getValue();

	void add(float val);
	void subtract(float  val);

	void setTopColor(btVector4 color);
	void setBottomColor(btVector4 color);

	void print();
	
private:
	float value;
	btVector4 rect;
	btVector4 topColor, bottomColor;
	Shader* shader;
};

#endif
