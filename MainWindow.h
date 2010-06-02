#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "NewWindow.h"

#include <Window.h>
#include <MenuBar.h>
#include <View.h>
#include <Picture.h>
#include <PictureButton.h>

#define MENU_NEW_MSG	'new'
#define MENU_OPN_MSG	'open'

class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
private:
			BMenuBar	*fMenuBar;
			BMenu		*fileMenu;
			BMenuItem	*newFileMenuItem;
			BMenuItem	*openFileMenuItem;
			BMenuItem	*quitMenuItem;
			NewWindow	*newWindow;
			BBitmap		*testImage;
			BView		*toolbarView;
			BPicture	*onImage;
			BPictureButton *newButton;
};

#endif