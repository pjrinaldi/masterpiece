#include "MPEditor.h"

MPEditor::MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	// initialize controls
	BRect r = Bounds();
	r.bottom = r.bottom - 50;
	editorTextView = new BTextView(r, NULL, r, B_FOLLOW_ALL, B_WILL_DRAW);	
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new EditorMenu(), 0, 0)
		.Add(new BScrollView("scroll_editor", editorTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 1)
		.SetInsets(0, 0, 0, 0)
	);
	
	currentideaID = ideaID; // pass current idea id selected to editor window for use
	
	mpdb = OpenSqliteDB(); // open mpdb db
	if(mpdb == NULL) // if db doesn't exist
	{
		eAlert = new ErrorAlert("1.6 Sql Error: Sql DB was not opened properly.");
		eAlert->Launch();
	}
	if(currentideaID != -1) // if id has a real value
	{
		// Pull data from db and populate thoughtview with it
		sqlValue = sqlite3_prepare_v2(mpdb, "select ideatext from ideatable where ideaid = ?", -1, &ideaStatement, NULL);
		if(sqlValue == SQLITE_OK) // sql statement was prepared properly
		{
			if(sqlite3_bind_int(ideaStatement, 1, currentideaID) == SQLITE_OK)
			{
				sqlite3_step(ideaStatement);
				editorTextView->SetText(sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0)));
			}
			else
			{
				eAlert = new ErrorAlert("1.7 Sql Error: Sql Bind failed.");
				eAlert->Launch();
			}
			sqlite3_finalize(ideaStatement);
		}
		else
		{
			eAlert = new ErrorAlert("1.8 Sql Error: Sql Prepare failed.");
		}
	}
}
void MPEditor::MessageReceived(BMessage* msg)
{
	BRect r(Bounds());
	switch(msg->what)
	{
		case MENU_NEW_THT: // open another untitled editor window
			tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
			tmpEditor->Show();
			break;
		case MENU_EDT_THT: // edit current idea name for editing
			printf(" must open edit name dialog\r\n");
			break;
		case MENU_SAV_THT: // save current idea progress
			if(currentideaID == -1) // if its untitled insert new thought, then show saveidea to apply a name...
			{
				sqlValue = sqlite3_prepare_v2(mpdb, "insert into ideatable (ideaname, ideatext, ismp) values('untitled', ?, 0)", -1, &ideaStatement, NULL);
				if(sqlValue == SQLITE_OK) // sql statement was prepared properly
				{
					if(sqlite3_bind_text(ideaStatement, 1, editorTextView->Text(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
					{
						sqlite3_step(ideaStatement);
						sqlite3_finalize(ideaStatement);
						xPos = (r.right - r.left) / 2;
						yPos = (r.bottom - r.top) / 2;
						saveIdea = new SaveIdea(BMessage(UPDATE_TITLE), BMessenger(this), xPos, yPos, sqlite3_last_insert_rowid(mpdb));
						saveIdea->Show();
					}
				}
			}
			else // already exists, just update ideatext and save new information
			{
				sqlValue = sqlite3_prepare_v2(mpdb, "update ideatable set ideatext = ? where ideaid = ?", -1, &ideaStatement, NULL);
				sqlite3_bind_text(ideaStatement, 1, editorTextView->Text(), -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(ideaStatement, 2, currentideaID);
				sqlite3_step(ideaStatement);
				sqlite3_finalize(ideaStatement);
				printf("must just write sql\r\n");
			}
			break;
		case MENU_PRV_THT:
			printf("save data, export to python html one and open data in preview window or webpositive");
			break;
		case MENU_PUB_THT:
			printf("save data, open publish to window, export to python and save as name in publish window");
			break;
		case MENU_HLP_THT:
			printf("open help topic window");
			break;
		case MENU_KEY_THT:
			printf("open keyboard reference window");
			break;
		case MENU_MRK_THT:
			printf("open markup reference window");
			break;
		case MENU_ABT_THT:
			printf("open about window");
			break;
		case UPDATE_TITLE:
			if(msg->FindString("updatetitle", &updateTitle) == B_OK)
			{
				tmpString = "Masterpiece Editor - ";
				tmpString += updateTitle;
				this->SetTitle(tmpString);
			}
			else
			{
				// put error alert here...
			}
			break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
bool MPEditor::QuitRequested(void)
{
	launcherMessage.MakeEmpty();
	launcherMessage.AddInt64("showLauncher", 1);
	launcherMessenger.SendMessage(&launcherMessage);
	return true;
}