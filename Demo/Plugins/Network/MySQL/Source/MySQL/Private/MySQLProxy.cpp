// Copyright Epic Games, Inc. All Rights Reserved.

#include "MySQLProxy.h"
#include "MySQLPrivate.h"

#include "ImageUtils.h"
//#include "Windows/AllowWindowsPlatformTypes.h"

#include "fstream"

//MYSQL_RES *Result;	// Lookup result-set variable
//const char *SQLQuery;	// SQL cmd turn to const char*
//MYSQL_ROW Row;		// Row of data-struct

using std::fstream;
using std::ios;

// char-char-FString
struct FCharProxy
{
	FCharProxy() { Size = 0; Buffer = nullptr; }
	
	explicit FCharProxy(const FString& InStr)
	{
		const TCHAR* QueryTChar = *InStr;
		size_t Length;
		const _locale_t Local = _create_locale(LC_ALL, "Chinese");
		_wcstombs_s_l(&Length, nullptr, 0, QueryTChar, 0, Local);

		Size = 0;
		Buffer = new char[Length];
		_wcstombs_s_l(&Size, Buffer, Length, QueryTChar, _TRUNCATE, Local);
	}
	
	~FCharProxy()
	{
		if (Size != 0)
			delete[] Buffer;
	}

	const char* GetBuffer() const { return Buffer; }

private:
	size_t Size;
	char* Buffer;
};

struct FWCharProxy
{
	FWCharProxy() { Size = 0; Buffer = nullptr; }
	
	explicit FWCharProxy(const char* Input)
	{
		const size_t Length = 1 + strlen(Input);
		Buffer = new wchar_t[Length];
		Size = 0;
		const _locale_t Local = _create_locale(LC_ALL, "Chinese");
		errno_t Ret = _mbstowcs_s_l(&Size, Buffer, Length, Input, _TRUNCATE, Local);
	}
	
	~FWCharProxy()
	{
		if (Size != 0)
			delete[] Buffer;
	}

	const wchar_t* GetBuffer() const { return Buffer; }

private:
	
	size_t Size;
	wchar_t* Buffer;
};

void UMySQLProxy::Connect(
	const FString& Server,
	const FString& DBName,
	const FString& UserID,
	const FString& Password,
	bool& IsSuccess,
	FString& ErrorMassage)
{
	const char* Host = FCharProxy(Server).GetBuffer();		//Server IP
	const char* DB = FCharProxy(DBName).GetBuffer();		//SQL name
	const char* User = FCharProxy(UserID).GetBuffer();		//Mysql UserName
	const char* Pass = FCharProxy(Password).GetBuffer();	//Mysql Password

	mysql_init(&MySQL);  //初始化 Mysql 上下文
	mysql_options(&MySQL, MYSQL_OPT_CONNECT_TIMEOUT, &OutTime);// Set time out
	mysql_options(&MySQL, MYSQL_OPT_RECONNECT, &ReConnection);// Set Reconnect delay
	
	if (!mysql_real_connect(&MySQL, Host, User, Pass, DB, 3306, nullptr, 0))
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);

		UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");
	}
	
	mysql_query(&MySQL, "SET NAMES GB2312");//解决中文乱码问题
}

void UMySQLProxy::DisConnect()
{
	mysql_close(&MySQL);
}

/* Search MySQL，then return a array of row-struct*/
void UMySQLProxy::SelectDataTableRows(
	const FString& FieldName,
	const FString& TableName,
	bool& IsSuccess,
	FString& ErrorMassage,
	TArray<FString>& TableHead,
	TArray<FMySQLDataRow>& Rows)
{
	Rows.Empty();
	TableHead.Empty();
	
	mysql_query(&MySQL, "SET NAMES GB2312");//解决中文乱码问题

	// 1.Execute SQL cmd
	const FString Query = "select " + FieldName + " from " + TableName;// cmd, note the space
	const char* SQLQuery = FCharProxy(Query).GetBuffer();
	if (mysql_real_query(&MySQL, SQLQuery, strlen(SQLQuery)) != 0)// execute
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);
		
		UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");
		
		// 2.Get result
		MYSQL_RES* Result = mysql_use_result(&MySQL);
		const MYSQL_FIELD* Field = mysql_fetch_field(Result);
		while (Field != nullptr)// Get table-struct
		{
			TableHead.Add(FWCharProxy(Field->name).GetBuffer());
		}

		const int32 FieldNum = mysql_num_fields(Result);// Get field num
		//const int32 RowNum = mysql_num_rows(Result);// Get row num
		
		// 3.遍历询结果
		const MYSQL_ROW Row = mysql_fetch_row(Result);
		if (Row != nullptr)
		{
			FMySQLDataRow MySQLDataRow;
			for (int32 i = 0; i < FieldNum; i++)
			{
				MySQLDataRow.RowData.Add(FWCharProxy(Row[i]).GetBuffer());
			}
			
			Rows.Add(MySQLDataRow);
		}

		// 4.Clean Result, if not, the next search shall fail
		mysql_free_result(Result);
	}
}

void UMySQLProxy::InsertRow(
		const FString& TableName,
		const FString& Values,
		bool& IsSuccess,
		FString& ErrorMassage)
{
	mysql_query(&MySQL, "SET NAMES GB2312"); //解决中文乱码问题
	
	const FString Query = "insert into " + TableName + " values(" + Values + ")";
	const char* SQLQuery = FCharProxy(Query).GetBuffer();

	if (mysql_real_query(&MySQL, SQLQuery, strlen(SQLQuery)) != 0) // execute
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);
		
		UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");
	}
}

void UMySQLProxy::DeleteRow(
	const FString& TableName,
	const FString& Field,
	const FString& Value,
	bool& IsSuccess,
	FString& ErrorMassage)
{
	mysql_query(&MySQL, "SET NAMES GB2312"); //解决中文乱码问题
	
	const FString Query = "delete from " + TableName + " where " + Field + "='" + Value + "'";
	const char* SQLQuery = FCharProxy(Query).GetBuffer();
	if (mysql_real_query(&MySQL, SQLQuery, strlen(SQLQuery)) != 0)// execute
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);

		UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");
	}
}

void UMySQLProxy::UpdateTable(
	const FString& TableName,
	const FString& Field,
	const FString& NewValue,
	const FString& FormerField,
	const FString& FormerValue,
	bool& IsSuccess,
	FString& ErrorMassage)
{
	mysql_query(&MySQL, "SET NAMES GB2312");//解决中文乱码问题
	
	const FString Query = "update " + TableName + " set " + Field + "='" + NewValue + "' where " + FormerField + "='" + FormerValue + "'";
	const char* SQLQuery = FCharProxy(Query).GetBuffer();
	if (mysql_real_query(&MySQL, SQLQuery, strlen(SQLQuery)) != 0)//执行SQL语句
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);

		UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");
	}
}

void UMySQLProxy::UpdateBinaryFile(
	const FString& TableName,
	const FString& FieldName,
	const FString& ID,
	const FString& InFilePath, 
	bool& IsSuccess,
	FString& ErrorMassage)
{
	mysql_query(&MySQL, "SET NAMES GB2312");//解决中文乱码问题
	
	MYSQL_STMT* Stmt = mysql_stmt_init(&MySQL);
	if (!Stmt)
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");

		const FString Query = "update " + TableName + " set " + FieldName + "=? where id=" + ID;
		if (mysql_stmt_prepare(Stmt, FCharProxy(Query).GetBuffer(), Query.Len()))
		{
			IsSuccess = false;
			ErrorMassage = mysql_stmt_error(Stmt);

			UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
		}
		
		// InFilePath, need full path name
		fstream File(FCharProxy(InFilePath).GetBuffer(), ios::in | ios::binary);
		if (!File.is_open())
		{
			IsSuccess = false;
			ErrorMassage = InFilePath + "open failed!!!";

			UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
		}
		else
		{
			File.seekg(0, ios::end);
			const int FileSize = File.tellg();
			File.seekg(0, ios::beg);
			char *Data = new char[FileSize];
			File.read(Data, FileSize);
			File.close();

			MYSQL_BIND Bind[1] = { nullptr };
			Bind[0].buffer_type = MYSQL_TYPE_MEDIUM_BLOB;
			Bind[0].buffer = Data;
			Bind[0].buffer_length = FileSize;
			
			if (mysql_stmt_bind_param(Stmt, Bind) != 0)
			{
				IsSuccess = false;
				ErrorMassage = mysql_stmt_error(Stmt);

				UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
			}
			else
			{
				if (mysql_stmt_execute(Stmt) != 0)
				{
					IsSuccess = false;
					ErrorMassage = mysql_stmt_error(Stmt);

					UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
				}
			}

			delete Data;
		}
		
		mysql_stmt_close(Stmt);
	}
}

void UMySQLProxy::SelectImage(
	const FString& TableName,
	const FString& ID,
	const int ImageIndex,
	bool& IsSuccess,
	FString& ErrorMassage,
	UTexture2D*& Texture)
{
	mysql_query(&MySQL, "SET NAMES GB2312");

	const FString Query = "select * from " + TableName + " where id=" + ID;
	const char* SQLQuery = FCharProxy(Query).GetBuffer();
	if (mysql_real_query(&MySQL, SQLQuery, strlen(SQLQuery)) != 0)
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);

		UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");
		
		MYSQL_RES* Result = mysql_use_result(&MySQL);
		const MYSQL_ROW Row = mysql_fetch_row(Result);
		if (Row == nullptr)
		{
			IsSuccess = false;
			ErrorMassage = mysql_error(&MySQL);

			UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
		}
		else
		{
			IsSuccess = true;
			ErrorMassage = TEXT("");
			
			const unsigned long *Lens = mysql_fetch_lengths(Result);
			const FString OutFilePath = FPaths::ProjectDir() + "/Content/Temporary";
			fstream File(FCharProxy(OutFilePath).GetBuffer(), ios::out | ios::binary);
			if (!File.is_open())
			{
				IsSuccess = false;
				ErrorMassage = TEXT("Openning file failed!");

				UE_LOG(LogMySQL, Error, TEXT("%s"), *ErrorMassage);
			}
			else
			{
				IsSuccess = true;
				ErrorMassage = TEXT("");
				File.write(Row[ImageIndex], Lens[ImageIndex]);
				File.close();
			}

			Texture = FImageUtils::ImportFileAsTexture2D(OutFilePath);
		}
		
		mysql_free_result(Result);
	}
}

UMySQLProxy::UMySQLProxy()
{
	MySQL = MYSQL();
	OutTime = 3;
	ReConnection = 1;
}

void UMySQLProxy::BeginDestroy()
{
	UObject::BeginDestroy();

	DisConnect();
}

