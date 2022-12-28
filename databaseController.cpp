#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <algorithm>
#include <vector>

#include "databaseController.h"
#include "logs.h"

using namespace std;

DatabaseController::DatabaseController() {
	sqlConnHandle = NULL;
	sqlStmtHandle = NULL;
}

void DatabaseController::databaseDisconnection() {
    try {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        writeStatus("���������� � ����� ������ ���������");
    }
    catch (...) {
        writeError("������������ ������ ���������� � ����� ������");
    }
}

void DatabaseController::databaseConnection() {
 
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        databaseDisconnection();
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        databaseDisconnection();
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        databaseDisconnection();

    writeStatus("������� ���������� � ����� ������");
  
    switch (SQLDriverConnect(sqlConnHandle,
        NULL,
        (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=NEUROBORT\\SQLEXPRESS;DATABASE=NeuralNetworkModeling;Trusted=true;",
        SQL_NTS,
        retconstring,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT)) {
    case SQL_SUCCESS:
        writeStatus("�������� ���������� � SQL Server");
        break;
    case SQL_SUCCESS_WITH_INFO:
        writeStatus("�������� ���������� � SQL Server");
        break;
    case SQL_INVALID_HANDLE:
        writeStatus("�� ������� ������������ � Server");
        databaseDisconnection();
    case SQL_ERROR:
        writeStatus("�� ������� ������������ � Server");
        databaseDisconnection();
    default:
        break;
    }  
}

void DatabaseController::insertWorkTime(float work_time) {
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        databaseDisconnection();
    else {
        writeStatus("��������� ������� �� ���������� ������");
        SQLRETURN r = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"INSERT INTO Test(work_time) VALUES (?)", SQL_NTS);
        r = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &work_time, 0, NULL);
        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            writeError("������ ���������� ������� ������");
            databaseDisconnection();
        }
        else {
            writeStatus("����� ������ ������� ���������");
        }
    }
}


void DatabaseController::deleteWorkTime(int id) {
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        databaseDisconnection();
    else {
        writeStatus("��������� ������� �� �������� ������");
        SQLRETURN r = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"DELETE FROM Test WHERE id = ?", SQL_NTS);
        r = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &id, 0, NULL);
        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            writeError("������ �������� ������� ������");
            databaseDisconnection();
        }
        else {
            writeStatus("����� ������ ������� �������");
        }
    }
}

vector<string> DatabaseController::getImagesAddress() {
    vector<string> address;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        databaseDisconnection();
    else {
        writeStatus("��������� ������� �� ��������� ������� �����������");
        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT address FROM Images", SQL_NTS)) {
            writeError("������ ��������� ������� �����������");
            databaseDisconnection();
        }
        writeStatus("������ ����������� ��������");
        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
            char buff[300];
            SQLGetData(sqlStmtHandle, 1, SQL_C_CHAR, buff, 300, NULL);
            string tmp = buff;
            int len = tmp.length();
            while (isspace(tmp[len - 1])) {
                tmp.erase(len - 1, 1);
                len = tmp.length();
            }
            address.push_back(tmp);
        }
        return address;
    }
}


vector<int> DatabaseController::getImagesId() {
    vector<int> id;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        databaseDisconnection();
    else {
        writeStatus("��������� ������� �� ��������� id �����������");
        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT id FROM Images", SQL_NTS)) {
            writeError("������ ��������� if �����������");
            databaseDisconnection();
        }
        writeStatus("id ����������� ��������");
        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
            int buff;
            SQLGetData(sqlStmtHandle, 1, SQL_C_LONG, &buff, 0, NULL);
            id.push_back(buff);
        }
        return id;
    }
}


void DatabaseController::setImageData(int id, int width, int height, string selection) {
    SQLLEN cbValue = SQL_NTS;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        databaseDisconnection();
    else {
        writeStatus("��������� ������� �� ���������� ���������� �� �����������");
        SQLRETURN r = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"UPDATE Images SET width = ?, height = ?, selection = ? WHERE id = ?;", SQL_NTS);
        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &width, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &height, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR, 15, 0, &selection, selection.length(), &cbValue);
        SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);
        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            writeError("������ ���������� ���������� �� �����������");
            databaseDisconnection();
        }
        else {
            writeStatus("���������� �� ����������� ������� ���������");
        }
    }
}

void DatabaseController::setNmdlExperimetData(int id_image, float clouds, float time) {
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        databaseDisconnection();
    else {
        writeStatus("��������� ������� �� ���������� ������ �� ������������");
        SQLRETURN r = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"INSERT INTO NmdlTest(id_image, clouds, timeCalc) VALUES (?, ?, ?)", SQL_NTS);
        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id_image, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &clouds, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &time, 0, NULL);
        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            writeError("������ ���������� ������ �� ������������");
            databaseDisconnection();
        }
        else {
            writeStatus("������ �� ������������ ������� ���������");
        }
    }
}
