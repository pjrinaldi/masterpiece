#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include "pyembed.h"

#include <sqlite3.h>
#include <Application.h>
#include <Roster.h>
#include <Path.h>
#include <String.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <File.h>
#include <Entry.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <Message.h>

#include "ErrorAlert.h"

void	DisplayError(const char* errorNumber, const char* errorType, const char* errorValue);
void	ExecutePreview(const char* tmpData);
void	ExecutePublish(BMessage* tmpMsg, const char* tmpData, BString tmpExt);
BString	GetAppDirPath(void);
BString GetUserDirPath(void);
bool	CheckExistingScripts(const char* scripttype);

class SqlObject
{
	public:
								SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber);
								SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber, sqlite3* openDB);
								~SqlObject(void);
				void			PrepareSql(const char* sqlQuery);
				void			BindValue(int bindPlace, int bindValue);
				void			BindValue(int bindPlace, double bindValue);
				void			BindValue(int bindPlace, int64 bindValue);
				void			BindValue(int bindPlace, const char* bindValue);
				void			BindValue(int bindPlace, const void* bindValue);// blob bind
				void			BindValue(int bindPlace);
				int				ReturnInt(int returnPlace);
				double			ReturnDouble(int returnPlace);
				int64			ReturnInt64(int returnPlace);
				const char*		ReturnText(int returnPlace);
				const void*		ReturnBlob(int returnPlace);
				int				StepSql(void);
				void			ClearBindings(void);
				int64			ReturnLastInsertRowID(void);
				void			ResetSql(void);
				void			FinalizeSql(void);
				void			CloseSql(void);
				sqlite3*		ReturnSqlDB(void);
	private:
				sqlite3*		sqldb;
				const char*		sqlquery;
				const char*		errornumber;
				const char*		bindstring;
				const char*		returnstring;
				const void*		bindblob;
				const void*		returnblob;
				ErrorAlert*		ealert;
				sqlite3_stmt*	sqlstatement;
				int				sqlcode;
				int				bindplace;
				int				returnplace;
				int				bindint;
				int				returnint;
				double			binddouble;
				double			returndouble;
				int64			bindint64;
				int64			returnint64;
				BString			tmpstring;
				
};
#endif
