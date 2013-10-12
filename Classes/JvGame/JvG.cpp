#include "JvG.h"
#include "JvState.h"
#include "JvSave.h"

unsigned int JvG::width;
unsigned int JvG::height;
JvState* JvG::stateP = NULL;
unsigned int JvG::frameRate;
double JvG::elapsed;
double JvG::maxElapsed;
double JvG::timeScale;
JvCamera* JvG::camera = NULL;
JvSave JvG::save;
bool JvG::pause = false;

void JvG::setGameData(unsigned int Width,
					  unsigned int Height)
{
	width = Width;
	height = Height;

	JvG::frameRate = 60;
	JvG::maxElapsed = 0.0333333f;
	JvG::elapsed = 0;
	timeScale = 1;
	//JvU::roundingError = 0.00001;
}

void JvG::switchState(JvState* StateP)
{
	
}

