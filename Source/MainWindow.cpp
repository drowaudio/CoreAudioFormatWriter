/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#include "MainWindow.h"
#include "AudioDemo/AudioDemoTabComponent.h"

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
    centreWithSize (800, 600);
    setVisible (true);
    
    setContentOwned (new AudioDemoTabComponent(), false);
}

MainAppWindow::~MainAppWindow()
{
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}
