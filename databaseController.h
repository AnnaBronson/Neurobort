// �����, ����������� ������ � ����� ������
#ifndef _DATABASE_CONTROLLER_H_
#define _DATABASE_CONTROLLER_H_

#include <iostream>
#include <vector>
// �����, ��������������� ��� ������ � ����� ������
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000


class DatabaseController {
public:
    // ��������� ��� �����������
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLHANDLE sqlEnvHandle;
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
    // �����������
    DatabaseController();
    // ����������� � ���� ������
    void databaseConnection();
    // ���������� �� ���� ������
    void databaseDisconnection();
    void insertWorkTime(float work_time);
    void deleteWorkTime(int id);
    // ��������� ������� �����������
    vector<string> getImagesAddress();
    // ��������� ������� �����������
    vector<int> getImagesId();
    // ��������� ���������� �� ������������
    void setImageData(int id, int width, int height, string selection);
    // ���������� ������ � �����������
    void setNmdlExperimetData(int id_image, float clouds, float time);
};
#endif
