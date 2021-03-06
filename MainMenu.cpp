#include "MainMenu.h"

MainMenu::MainMenu(BRect rect, BString editText, const BMessage &msg, const BMessenger &msgr)
	:	BMenuBar(rect, "editorbar"), statusMessage(msg), statusMessenger(msgr)
{
	// initialize menu and menuitems
	helpMenu = new BMenu("Help");
	topicMenu = new BMenu("ReStructured Markup");
	exampleMenu = new BMenu("ReStructured Examples");
	newThoughtMenuItem = new BMenuItem("New Thought", new BMessage(MENU_NEW_THT));
	editThoughtMenuItem = new BMenuItem(editText, new BMessage(MENU_EDT_THT));
	saveThoughtMenuItem = new BMenuItem("Save Progress", new  BMessage(MENU_SAV_THT));
	exportMenuItem = new BMenuItem("Export", new BMessage(MENU_EXP_THT));
	previewMenuItem = new BMenuItem("Preview", new BMessage(MENU_PRV_THT));
	publishMenuItem = new BMenuItem("Publish", new BMessage(MENU_PUB_THT));
	keyboardRefMenuItem = new BMenuItem("Keyboard Reference", new BMessage(MENU_KEY_THT));
	quickStartMenuItem = new BMenuItem("Quick Start", new BMessage(MENU_QKS_THT));
	cheatMenuItem = new BMenuItem("Cheat Sheet", new BMessage(MENU_CHT_THT));
	guideMenuItem = new BMenuItem("Markup Specification", new BMessage(MENU_RST_THT));
	directMenuItem = new BMenuItem("Directives Guide", new BMessage(MENU_DRT_THT));
	quickExMenuItem = new BMenuItem("Quick Start Source", new BMessage(MENU_QKE_THT));
	cheatExMenuItem = new BMenuItem("Cheat Sheet Source", new BMessage(MENU_CHE_THT));
	mphelpMenuItem = new BMenuItem("MasterPiece Tutorial", new BMessage(MENU_MPH_THT));
	quickRefMenuItem = new BMenuItem("RST Quick Reference", new BMessage(MENU_QKR_THT));
	aboutMenuItem = new BMenuItem("About MasterPiece", new BMessage(MENU_ABT_THT));
	
	// layout helptopics menu
	topicMenu->AddItem(quickStartMenuItem);
	topicMenu->AddItem(cheatMenuItem);
	topicMenu->AddItem(guideMenuItem);
	topicMenu->AddItem(directMenuItem);
	
	//layout examples menu
	exampleMenu->AddItem(quickExMenuItem);
	exampleMenu->AddItem(cheatExMenuItem);
	
	//layout helpMenu
	helpMenu->AddItem(topicMenu);
	helpMenu->AddItem(exampleMenu);
	helpMenu->AddItem(mphelpMenuItem);
	helpMenu->AddItem(quickRefMenuItem);
	helpMenu->AddItem(keyboardRefMenuItem);
	helpMenu->AddSeparatorItem();
	helpMenu->AddItem(aboutMenuItem);
	
	//layout menubar
	AddItem(newThoughtMenuItem);
	AddItem(editThoughtMenuItem);
	AddItem(saveThoughtMenuItem);
	AddItem(exportMenuItem);
	AddItem(previewMenuItem);
	AddItem(publishMenuItem);
	AddItem(helpMenu);
	
	// set enabled/disabled triggers at initialization
}
void MainMenu::MouseDown(BPoint point)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	BMenuBar::MouseDown(point);
}
void MainMenu::KeyDown(const char* bytes, int32 numBytes)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	BMenuBar::KeyDown(bytes, numBytes);
}
