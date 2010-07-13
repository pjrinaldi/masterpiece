#include "App.h"

App::App(void)
	:	BApplication("application/x-vnd.pjr-MasterPiece")
{
	MainWindow *mainwin = new MainWindow();
	mainwin->Show();
}


int
main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}