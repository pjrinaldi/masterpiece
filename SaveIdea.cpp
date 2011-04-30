#include "SaveIdea.h"

SaveIdea::SaveIdea(const BMessage &msg, const BMessenger &msgr, float mainX, float mainY, int currentID)
	:	BWindow(BRect(0, 0, 358, 60), "Save As", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), updatetitleMessage(msg), updatetitleMessenger(msgr)
{
	// initialize controls
	BRect textFrame(0, 0, 300, 10);
	titleText = new BTextView(textFrame, NULL, textFrame, B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW);
	titleText->MakeResizable(false);
	titleText->SetWordWrap(false);
	titleString = new BStringView(BRect(10, 10, 200, 30), NULL, "Enter Thought Title:");
	saveButton = new BButton(BRect(190, 50, 270, 75), NULL, "Save", new BMessage(SAVE_IDEA));
	cancelButton = new BButton(BRect(190, 50, 270, 75), NULL, "Cancel", new BMessage(CANCEL_SAVE));
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(titleString, 0, 0, 2, 1)
		.Add(titleText, 2, 0, 3, 1)
		.Add(cancelButton, 3, 1)
		.Add(saveButton, 4, 1)
		.Add(BSpaceLayoutItem::CreateGlue(), 0, 2, 2, 1)
		.SetInsets(2, 5, 2, 2)
	);
	MoveTo(mainX, mainY); // move to window position
	
	currentideaID = currentID; // make current idea id available to 
	mpdb = OpenSqliteDB(); // open db
	if(mpdb == NULL) // if db failed
	{
		eAlert = new ErrorAlert("1.14 Sql Error: Sql DB was not opened properly.");
		eAlert->Launch();
	}
}
void SaveIdea::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case SAVE_IDEA: // save thought name to sql
			if(currentideaID > 0)
			{
				// do what i need here...
				sqlValue = sqlite3_prepare_v2(mpdb, "update ideatable set ideaname = ? where ideaid = ?", -1, &ideaStatement, NULL);
				if(sqlValue == SQLITE_OK) // sql statement was prepared properly
				{
					if(sqlite3_bind_text(ideaStatement, 1, titleText->Text(), -1, SQLITE_TRANSIENT) == SQLITE_OK) // bind was successful
					{
						if(sqlite3_bind_int(ideaStatement, 2, currentideaID) == SQLITE_OK) // bind was successful
						{
							sqlite3_step(ideaStatement); // execute update finished
							sqlite3_finalize(ideaStatement); // finish the statement
							updatetitleMessage.MakeEmpty();
							updatetitleMessage.AddString("updatetitle", titleText->Text());  // prepare message with idea name
							updatetitleMessenger.SendMessage(&updatetitleMessage); // send message with prepare name to editor
						}
						else
						{
							eAlert = new ErrorAlert("1.17 Sql Error: Sql Bind Failed.");
							eAlert->Launch();
						}
					}
					else
					{
						eAlert = new ErrorAlert("1.16 Sql Error: Sql Bind Failed.");
						eAlert->Launch();
					}
				}
				else
				{
					eAlert = new ErrorAlert("1.15 Sql Error: Sql Prepare Failed.");
					eAlert->Launch();
				}
			}
			else
			{
				eAlert = new ErrorAlert("5.1 Save Error: There is no idea to save.");
				eAlert->Launch();
			}
			this->Close();
			break;
		case CANCEL_SAVE: // cancel without saving name
			this->Close();
			break;
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}