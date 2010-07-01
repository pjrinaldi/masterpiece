#ifndef OPENMASTERVIEW_H
#define OPENMASTERVIEW_H

#include <Message.h>
#include <Bitmap.h>
#include <String.h>
#include <View.h>
#include <ListView.h>
#include <StringView.h>
#include <ScrollView.h>
#include <Button.h>

#define OPEN_EXISTING_COURSE	'oec'
#define CANCEL_OPEN_COURSE		'coc'

class OpenMasterView : public BView
{
public:
						OpenMasterView(void);
						~OpenMasterView(void);
						
		void			Draw(BRect rect);

		BListView		*openListView;
		BStringView		*openTitleString;
		BButton			*openButton;
		BButton			*cancelButton;
		
private:

protected:
};

#endif