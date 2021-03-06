#include "MarkupWindow.h"

MarkupWindow::MarkupWindow(BRect frame, const char* title)
	:	BWindow(frame, title, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	// initialize controls      s
	AddShortcut('q', B_COMMAND_KEY, new BMessage(B_QUIT_REQUESTED));
	BRect r = Bounds();
	topicListView = new BListView(BRect(10, 10, 30, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	contentTextView = new BTextView(BRect(0, 0, r.right, 100), NULL, BRect(10, 10, r.right, 100), B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	contentTextView->SetWordWrap(true);
	contentTextView->MakeEditable(false);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	backView->SetHighColor((rgb_color){0, 0, 0, 255});
	AddChild(backView);
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new BScrollView("scroll_topic", topicListView, B_FOLLOW_TOP | B_FOLLOW_LEFT, 0, false, true, B_FANCY_BORDER), 0, 0, 2, 3)
		.Add(new BScrollView("scroll_content", contentTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 2, 0, 6, 10)
		.SetInsets(0, 0, 0, 0)
	);
	topicListView->SetSelectionMessage(new BMessage(LOAD_CONTENT));
}
void MarkupWindow::AddMarkupItem(BString topicstring, BString contentstring)
{
	topicListView->AddItem(new HelpStringItem(topicstring, contentstring));
}
void MarkupWindow::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case LOAD_CONTENT:
			selected = topicListView->CurrentSelection();
			if(selected >= 0) // you selected something
			{
				HelpStringItem* item;
				BString contentPath = GetAppDirPath();
				BFile file;
				item = dynamic_cast<HelpStringItem*>(topicListView->ItemAt(selected));
				contentPath += "/";
				contentPath += item->ReturnContent();
				printf("contentpath: %s\n", contentPath.String());
				if(file.SetTo(contentPath, B_READ_ONLY) == B_OK)
				{
					off_t length;
					char* text;
					file.GetSize(&length);
					text = (char*) malloc(length);
					if(text && (file.Read(text, length)) >= B_OK)
					{
						contentTextView->SetText(text, length);
					}
					free(text);
				}
			}
			break;
		
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
bool MarkupWindow::QuitRequested(void)
{
	return true;
}
