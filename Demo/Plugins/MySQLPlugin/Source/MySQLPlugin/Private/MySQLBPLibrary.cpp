// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "MySQLBPLibrary.h"
#include "Misc/FileHelper.h"
#include "ImageUtils.h"
//#include "Windows/AllowWindowsPlatformTypes.h"

#include "MySQLPlugin.h"
#include "mysql.h"
#include "fstream"
//#include "codecvt"

constexpr int OutTime = 3;//连接超时设定
constexpr int ReConnection = 1;//自动重连

MYSQL MySQL;//mysql上下文；
MYSQL_RES *res;//查询结果集变量
MYSQL_FIELD *field = 0;
MYSQL_ROW row;//数据行，结构体
FMySQLDataRow m_row;
MYSQL_STMT *stmt;

const char* Host;    //服务器IP
const char* DB;      //数据库名称
const char* User;	 //Mysql 登录账户
const char* Pass;	 //mysql 登录密码

FString Query;      //mysql SQL语句   用户输入的FString 类型语句
const char *sql;    //mysql SQL语句   转换后的 const char*  类型语句

int fieldnum = 0;   //表的列数
int rownum = 0;     //表的行数


//FString转char*

char* FStringToChar(const FString& Query)
{
	const TCHAR* QueryTChar = *Query;
	size_t Length;
	
	const _locale_t Local = _create_locale(LC_ALL, "Chinese");
	_wcstombs_s_l(&Length, nullptr, 0, QueryTChar, 0, Local);

	size_t ConvertedSize = 0;
	char *CharBuffer = (char*)malloc(Length);
	_wcstombs_s_l(&ConvertedSize, CharBuffer, Length, QueryTChar, _TRUNCATE, Local);

	return CharBuffer;
}

//char* 转FString;
wchar_t* CharToWChar(const char* Input)
{
	const size_t Length = 1 + strlen(Input);
	wchar_t* Result = new wchar_t[Length];

	size_t ConvertedSize = 0;

	const _locale_t Local = _create_locale(LC_ALL, "Chinese");
	errno_t Ret = _mbstowcs_s_l(&ConvertedSize, Result, Length, Input, _TRUNCATE, Local);

	return Result;
}


/*登录*/
void UMySQLPluginBPLibrary::Connection(
	const FString& Server,
	const FString& DBName,
	const FString& UserID,
	const FString& Password,
	bool& IsSuccess,
	FString& ErrorMassage)
{
	Host = FStringToChar(Server);
	DB = FStringToChar(DBName);
	User = FStringToChar(UserID);
	Pass = FStringToChar(Password);

	mysql_init(&MySQL);  //初始化 Mysql 上下文
	mysql_options(&MySQL, MYSQL_OPT_CONNECT_TIMEOUT, &OutTime); //连接超时设定
	mysql_options(&MySQL, MYSQL_OPT_RECONNECT, &ReConnection); //自动重连
	
	if (!mysql_real_connect(&MySQL, Host, User, Pass, DB, 3306, 0, 0))
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");
	}
	
	mysql_query(&MySQL, "SET NAMES GB2312");//解决中文乱码问题
}

void UMySQLPluginBPLibrary::Close()
{
	mysql_close(&MySQL);
}

/*查询表，返回表的行结构数组*/
void UMySQLPluginBPLibrary::SelectDataTableRows(FString FeildName, FString TableName,
	bool & IsSuccess, FString & errorMassage, TArray<FString>&TableHead, TArray<FMySQLDataRow>& Rows)
{
	Rows.Empty();
	TableHead.Empty();
	mysql_query(&MySQL, "SET NAMES GB2312");//解决中文乱码问题

	//1.执行SQL语句
	Query = "select " + FeildName + " from " + TableName;//语句，这里要注意空格；
	sql = FStringToChar(Query);


	if (mysql_real_query(&MySQL, sql, strlen(sql)) != 0)//执行SQL语句
	{
		IsSuccess = false;
		errorMassage = mysql_error(&MySQL);
	}
	else
	{
		IsSuccess = true;
		errorMassage = NULL;
		//2.获取结果集
		res = mysql_use_result(&MySQL);

		while ((field = mysql_fetch_field(res)) != NULL)//获取表结构
		{
			TableHead.Add(CharToWChar(field->name));
		}

		fieldnum = mysql_num_fields(res);//获取表列数
		rownum = mysql_num_rows(res);    //获取表行数

		//3.遍历询结果
		while ((row = mysql_fetch_row(res)) != NULL)
		{
			m_row.RowData.Empty();

			for (int i = 0; i < fieldnum; i++)
			{
				m_row.RowData.Add(CharToWChar(row[i]));
			}
			Rows.Add(m_row);
		}

		//4.清理查询结果集，如果不清理，下次执行该查询函数将失败
		mysql_free_result(res);
	}

}

void UMySQLPluginBPLibrary::InsertRow(
		const FString& TableName,
		const FString& Values,
		bool& IsSuccess,
		FString& ErrorMassage)
{
	mysql_query(&MySQL, "SET NAMES GB2312"); //解决中文乱码问题
	Query = "insert into " + TableName + " values(" + Values + ")";
	sql = FStringToChar(Query);

	if (mysql_real_query(&MySQL, sql, strlen(sql)) != 0) //执行SQL语句
	{
		IsSuccess = false;
		ErrorMassage = mysql_error(&MySQL);
	}
	else
	{
		IsSuccess = true;
		ErrorMassage = TEXT("");
	}
}

void UMySQLPluginBPLibrary::deleteRow(FString TableName, FString Field, FString value,
	bool & IsSuccess, FString & errorMassage)
{
	mysql_query(&mysql, "SET NAMES GB2312");//解决中文乱码问题
	Query = "delete from " + TableName + " where " + Field + "='" + value + "'";
	sql = FStringToChar(Query);

	if (mysql_real_query(&mysql, sql, strlen(sql)) != 0)//执行SQL语句
	{
		IsSuccess = false;
		errorMassage = mysql_error(&mysql);
	}
	else
	{
		IsSuccess = true;
		errorMassage = NULL;
	}
}

void UMySQLPluginBPLibrary::UpdateTable(FString TableName, FString Field,
	FString NewValue, FString FormerField, FString FormerValue, bool & IsSuccess, FString & errorMassage)
{
	mysql_query(&mysql, "SET NAMES GB2312");//解决中文乱码问题
	Query = "update " + TableName + " set " + Field + "='" + NewValue + "' where " + FormerField + "='" + FormerValue + "'";
	sql = FStringToChar(Query);

	if (mysql_real_query(&mysql, sql, strlen(sql)) != 0)//执行SQL语句
	{
		IsSuccess = false;
		errorMassage = mysql_error(&mysql);
	}
	else
	{
		IsSuccess = true;
		errorMassage = NULL;
	}
}

void UMySQLPluginBPLibrary::UpdateBinaryFile(FString TableName, FString FieldName, 	FString ID, FString inFile, 
	bool &IsSuccess, FString &errorMassage)
{
	mysql_query(&mysql, "SET NAMES GB2312");//解决中文乱码问题
	stmt = mysql_stmt_init(&mysql);
	if (!stmt)
	{
		IsSuccess = false;
		errorMassage = mysql_error(&mysql);
	}
	else
	{
		IsSuccess = true;
		errorMassage = NULL;

		Query = "update " + TableName + " set " + FieldName + "=? where id=" + ID;

		if (mysql_stmt_prepare(stmt, FStringToChar(Query), Query.Len()))
		{
			IsSuccess = false;
			errorMassage = mysql_stmt_error(stmt);
		}
		FString inFilename = inFile;//   例如："D:/Documents/Pictures/Camera Roll/mysql_read.jpg"; 含路径的全名
		fstream file(FStringToChar(inFilename), ios::in | ios::binary);
		if (!file.is_open())
		{
			IsSuccess = false;
			errorMassage = inFilename + "open failed!!";
		}
		else
		{
			file.seekg(0, ios::end);
			int filesize = file.tellg();
			file.seekg(0, ios::beg);
			char *data = new char[filesize];
			file.read(data, filesize);


			file.close();

			MYSQL_BIND bind[1] = { 0 };
			bind[0].buffer_type = MYSQL_TYPE_MEDIUM_BLOB;
			bind[0].buffer = data;
			bind[0].buffer_length = filesize;


			if (mysql_stmt_bind_param(stmt, bind) != 0)
			{
				IsSuccess = false;
				errorMassage = mysql_stmt_error(stmt);
			}
			else
			{
				if (mysql_stmt_execute(stmt) != 0)
				{
					IsSuccess = false;
					errorMassage = mysql_stmt_error(stmt);
				}
			}

			delete data;
		}
		mysql_stmt_close(stmt);

	}
}

void UMySQLPluginBPLibrary::Selectimage(FString TableName,FString ID, int Image_Indxe,
	bool &IsSuccess, FString &errorMassage, UTexture2D* &Texture)
{
	mysql_query(&mysql, "SET NAMES GB2312");

	Query = "select * from " + TableName + " where id=" + ID;
	sql = FStringToChar(Query);

	if (mysql_real_query(&mysql, sql, strlen(sql)) != 0)
	{
		IsSuccess = false;
		errorMassage = mysql_error(&mysql);
	}
	else
	{
		IsSuccess = true;
		errorMassage = NULL;
		res = mysql_use_result(&mysql);
		if ((row = mysql_fetch_row(res)) == NULL)
		{
			IsSuccess = false;
			errorMassage = mysql_error(&mysql);
		}
		else
		{
			IsSuccess = true;
			errorMassage = NULL;
			unsigned long *lens = mysql_fetch_lengths(res);

			FString outFilename = FPaths::ProjectDir() + "/Content/Temporary";
			fstream file2(FStringToChar(outFilename), ios::out | ios::binary);
			if (!file2.is_open())
			{
				IsSuccess = false;
				errorMassage = "open file is failed!";
			}
			else
			{
				IsSuccess = true;
				errorMassage = NULL;
				file2.write(row[Image_Indxe], lens[Image_Indxe]);
				file2.close();
			}

			Texture = FImageUtils::ImportFileAsTexture2D(outFilename);
		}
		mysql_free_result(res);
	}



}

//查找指定目录下指定扩展名的文件
void UMySQLPluginBPLibrary::GetFiles(FString inPath,  FString Fileform, TArray<FString>&FileList)
{
		
	FString SearchedFiles = inPath + "/"+Fileform;
	
	IFileManager::Get().FindFiles(FileList, *SearchedFiles, true, false);
	
}




