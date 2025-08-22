#ifndef control_h
#define control_h

#include "stdint.h"
#include "glinit.h"
#include "model.h"
#include "view.h"

class Control {
private:
	Model& model;
	View& view;
	const uint8_t *key_state;
public:
	void processInput();
	Control(Model& model, View& view);
};

#endif // !control_h
