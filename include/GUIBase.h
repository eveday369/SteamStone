#pragma once
#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>

class GUIBase
{
public:
	GUIBase(void);
	virtual ~GUIBase(void);
private:
	void CreateCEGUI(void);
};

