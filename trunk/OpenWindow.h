#ifndef OPENWINDOW_H
#define OPENWINDOW_H

#include <Window.h>
#include <Button.h>
#include <GridLayout.h>
#include <sqlite3.h>
#include <Alert.h>
#include <Application.h>
#include <Path.h>
#include <Roster.h>
#include <Messenger.h>
#include <Message.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringView.h>
#include <TextView.h>
#include <View.h>

#include "ErrorAlert.h"

#define OPEN_EXISTING_MP	'oemp'
#define	CANCEL_OPEN_MP		'comp'

class OpenWindow : public BWindow
{
	public:
						OpenWindow(const BMessage &msg, const BMessenger &msgr, float mainX, float mainY, const BString commonName);
			void		MessageReceived(BMessage* msg);
			void		OpenMasterPieceDB(void);
			void		OpenExistingSql();
			
	private:
			BListView*		openListView;
			BStringView*	openTitleString;
			BTextView*		titleText;
			BButton*		openButton;
			BButton*		cancelButton;
			ErrorAlert*		eAlert;
			int				returnValue;
			sqlite3*		mpdb;
			int				sqlValue;
			int				selected;
			char*			sqlErrMsg;
			char**			selectResult;
			int				nrow, ncol;
			BString			tmpString;
			BAlert*			userAlert;
			BMessage		mpMessage;
			BMessenger		mpMessenger;
			BGridLayout*	mainGrid;
			BView*			backView;
			BString			CommonName;
};

#endif
