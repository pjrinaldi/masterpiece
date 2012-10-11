#include "CommonFunctions.h"

using namespace pyembed;

void DisplayError(const char* errorNumber, const char* errorType, const char* errorValue)
{
	BString tmpString = errorNumber;
	ErrorAlert* ealert;
	tmpString += ". SqlError: ";
	tmpString += errorType;
	tmpString += " Returned ";
	tmpString += errorValue;
	ealert = new ErrorAlert(tmpString);
	ealert->Launch();
}
BString GetAppDirPath(void)
{
	app_info info;
	be_app->GetAppInfo(&info);
	BPath path(&info.ref);
	path.GetParent(&path);
	return path.Path();
}
BString GetUserDirPath(void)
{
	BPath tmpUserPath;
	status_t result = find_directory(B_USER_DIRECTORY, &tmpUserPath);
	if(result == B_OK) return tmpUserPath.Path();
	else return "-15";
}
void TmpCleanUp(BString tmpExt)
{
	// Get and remove tmp.tmpExt file
	BString tmpPath = GetAppDirPath();
	BEntry removeTmpFile;
	tmpPath += "/tmp.";
	tmpPath += tmpExt;
	printf("tht path: %s\n", tmpPath.String());
	removeTmpFile.SetTo(tmpPath);
	status_t err = removeTmpFile.Remove();
	if(err == B_OK || err == B_ENTRY_NOT_FOUND)
	{
		// don't report to end user
		//printf("expected error: %s\n", strerror(err));
	}
	else
	{
		// report to end user
		ErrorAlert* eAlert = new ErrorAlert("Error when cleaning up Temp Files prior to Exit: ", strerror(err));
		eAlert->Launch();
	}
}
void ExecutePreview(BString tmpData)
{
	int argc = 1;
	char* argvv = "ladida";
	char** argv = &argvv;
	Python py(argc, argv);
	BString tmpInPath; // string path of the tmppub.tht file then string path of tmppub.ext file
	BString tmpOutPath;
	BString pythonString;
	BFile previewFile; // tmppub.tht file
	ErrorAlert* eAlert;
	tmpInPath = GetAppDirPath();
	tmpInPath += "/tmp.tht";
	tmpOutPath = GetAppDirPath();
	tmpOutPath += "/tmp.html";
	pythonString = "output = publish_file(source_path='";
	pythonString += tmpInPath;
	pythonString += "', destination_path='";
	pythonString += tmpOutPath;
	pythonString += "', writer_name='html')";
	previewFile.SetTo(tmpInPath, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE); // B_ERASE_FILE
	if(previewFile.InitCheck() != B_OK)
	{
		eAlert = new ErrorAlert("4.2 Builder Error: Couldn't Write TMP File.");
		eAlert->Launch();
		//printf("Couldn't write file\n");
	}
	previewFile.Write(tmpData, strlen(tmpData));
	previewFile.Unset();
	try
	{
		py.run_string("from docutils.core import publish_file");
		py.run_string(pythonString.String());
	}
	catch(Python_exception ex)
	{
		eAlert = new ErrorAlert("4.3 Builder Error: Python Issue - ", ex.what());
		eAlert->Launch();
	}
	
	BString tmpPath = "/boot/apps/WebPositive file://";
	tmpPath += GetAppDirPath();
	tmpPath += "/tmp.html &";
	system(tmpPath);
}

ErrorAlert::ErrorAlert(BString tmpText)
{
	tmpAlert = new BAlert("Error:", tmpText, "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
	tmpAlert->MoveTo(350, 250);
	tmpAlert->SetShortcut(0, B_ESCAPE);
}

ErrorAlert::ErrorAlert(BString tmpText1, BString tmpText2)
{
	BString tmpString = tmpText1;
	tmpString += tmpText2;
	tmpAlert = new BAlert("Error:", tmpString, "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
	tmpAlert->MoveTo(350, 250);
	tmpAlert->SetShortcut(0, B_ESCAPE);
}

ErrorAlert::~ErrorAlert(void)
{
}

int ErrorAlert::Launch(void)
{
	return tmpAlert->Go(NULL);
}

SqlObject::SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber, sqlite3* openDB)
{
	sqldb = openDB;
	sqlstatement = sqlStatement;
	errornumber = errorNumber;
}
SqlObject::SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber)
{
	sqldb = NULL;
	sqlstatement = sqlStatement;
	errornumber = errorNumber;
	char* sqlErrMsg;
	int	sqlValue;
	BString tmpString;
	sqlErrMsg = 0;
	BString tmpPath = GetAppDirPath(); // for testing purposes, use local one.
	//BString tmpPath = GetUserDirPath(); // for publish purposes, use real location
	if(tmpPath != "-15")
	{
		tmpPath += "/MasterPiece.db";
		sqlValue = sqlite3_open_v2(tmpPath, &sqldb, SQLITE_OPEN_READWRITE, NULL); // open db
		if(sqlite3_errcode(sqldb) == 14) // if error is SQLITE_CANTOPEN, then create db with structure
		{
			sqlValue = sqlite3_open_v2(tmpPath, &sqldb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); 
			if(sqlite3_errcode(sqldb) == 0) // sqlite_ok
			{
				tmpString = "CREATE TABLE ideatable(ideaid integer primary key autoincrement, ideaname text, ideatext text, ismp integer, mpid integer, ordernumber integer);";
				sqlValue = sqlite3_exec(sqldb, tmpString, NULL, NULL, &sqlErrMsg);
				if(sqlValue != SQLITE_OK) // if sql was not successful
				{
					DisplayError("1.1", "OPEN", sqlErrMsg);
				}
			}
			else // some kind of failure
			{
				DisplayError("1.0", "OPEN", sqlite3_errmsg(sqldb));
			}
		}
		else if(sqlite3_errcode(sqldb) == 0) // sqlite_OK, it exists
		{
			//no error, so i will return opendb at end;
		}
		else // if error is not ok or not existing, then display error in alert
		{
			DisplayError("1.2", "OPEN", sqlite3_errmsg(sqldb));
		}
		sqlite3_free(sqlErrMsg);
	}
	else
	{
		DisplayError("1.0", "PATH", " to the User Directory not Found");
	}
}
SqlObject::~SqlObject(void)
{
}
void SqlObject::PrepareSql(const char* sqlQuery)
{
	sqlquery = sqlQuery;
	if(sqlite3_prepare_v2(sqldb, sqlquery, -1, &sqlstatement, NULL) != SQLITE_OK) // sql statement was not prepared
	{
		DisplayError(errornumber, "PREPARE", sqlite3_errmsg(sqldb));
	}
}
void SqlObject::BindValue(int bindPlace, int bindValue)
{
	bindplace = bindPlace;
	bindint = bindValue;
	if(sqlite3_bind_int(sqlstatement, bindplace, bindint) != SQLITE_OK) // sql int bind failed
	{
		DisplayError(errornumber, "BIND INT", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace, double bindValue)
{
	bindplace = bindPlace;
	binddouble = bindValue;
	if(sqlite3_bind_double(sqlstatement, bindplace, binddouble) != SQLITE_OK) // sql double bind failed
	{
		DisplayError(errornumber, "BIND DOUBLE", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace, int64 bindValue)
{
	bindplace = bindPlace;
	bindint64 = bindValue;
	if(sqlite3_bind_int64(sqlstatement, bindplace, bindint64) != SQLITE_OK) // sql int64 bind failed
	{
		DisplayError(errornumber, "BIND INT64", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace, const char* bindValue)
{
	bindplace = bindPlace;
	bindstring = bindValue;
	if(sqlite3_bind_text(sqlstatement, bindplace, bindstring, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql text bind failed
	{
		DisplayError(errornumber, "BIND TEXT", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace) // bind null
{
	bindplace = bindPlace;
	if(sqlite3_bind_null(sqlstatement, bindplace) != SQLITE_OK)
	{
		DisplayError(errornumber, "BIND NULL", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace, const void* bindValue)
{
	bindplace = bindPlace;
	bindblob = bindValue;
	if(sqlite3_bind_blob(sqlstatement, bindplace, bindblob, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql blob bind failed
	{
		DisplayError(errornumber, "BIND BLOB", "MISUSE");
	}
}
int SqlObject::ReturnInt(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_column_int(sqlstatement, returnplace);
}
double SqlObject::ReturnDouble(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_column_double(sqlstatement, returnplace);
}
int64 SqlObject::ReturnInt64(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_column_int64(sqlstatement, returnplace);
}
const char* SqlObject::ReturnText(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_mprintf("%s", sqlite3_column_text(sqlstatement, returnplace));
}
const void* SqlObject::ReturnBlob(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_column_blob(sqlstatement, returnplace);
}
int64 SqlObject::ReturnLastInsertRowID(void)
{
	return sqlite3_last_insert_rowid(sqldb);
}
int SqlObject::StepSql(void)
{
	sqlcode = sqlite3_step(sqlstatement);
	if(sqlcode != SQLITE_ROW && sqlcode != SQLITE_DONE)
	{
		if(sqlcode == SQLITE_BUSY) DisplayError(errornumber, "STEP", "BUSY");
		else if(sqlcode == SQLITE_ERROR) DisplayError(errornumber, "STEP", "ERROR");
		else if(sqlcode == SQLITE_MISUSE) DisplayError(errornumber, "STEP", "MISUSE");
		else DisplayError(errornumber, "STEP", "OTHER ISSUE");
	}
	return sqlcode;
}
void SqlObject::ClearBindings(void)
{
	if(sqlite3_clear_bindings(sqlstatement) != SQLITE_OK)
	{
		DisplayError(errornumber, "CLEAR", "ERROR");
	}
}
void SqlObject::ResetSql(void)
{
	if(sqlite3_reset(sqlstatement) != SQLITE_OK)
	{
		DisplayError(errornumber, "RESET", "ERROR");
	}
}
void SqlObject::FinalizeSql(void)
{
	if(sqlite3_finalize(sqlstatement) != SQLITE_OK)
	{
		DisplayError(errornumber, "FINALIZE", "ERROR");
	}
}
void SqlObject::CloseSql(void)
{
	sqlcode = sqlite3_close(sqldb);
	if(sqlcode != SQLITE_OK)
	{
		if(sqlcode == SQLITE_BUSY) DisplayError(errornumber, "CLOSE", "BUSY");
		else if(sqlcode == SQLITE_ERROR) DisplayError(errornumber, "CLOSE", "ERROR");
		else DisplayError(errornumber, "CLOSE", "OTHER");
	}
}
sqlite3* SqlObject::ReturnSqlDB(void)
{
	return sqldb;
}
