
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aclconio.h>

//#include <Logsys.h>
//#include <eoslog.h>
#include "logutil.h"

// ACT 2000
#include <applidl.h>
#include <acldev.h>
#include <aclstr.h>
#include <message.h>

#include "sqlite3.h"

static int tests_callback(void *NotUsed, int argc, char **argv, char **azColName){

	int i;

	for(i=0; i<argc; i++){
		LOG_PRINTF(("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL"));
	}

	return 0;
}

//////////////////////////////////////

static const char * dbname = "I:testes";

//////////////////////////////////////
/// TEST3

void test3(void) {
	sqlite3* db;
	char *zErrMsg = 0;
	int rc;
	int i;


	LOG_PRINTF(("TESTE 3"));

	rc = sqlite3_open(dbname, &db);
	if(rc) {
		LOG_PRINTF(("Error opening %d %s!", sqlite3_errcode(db), sqlite3_errmsg(db)));

		sqlite3_close(db);
		exit(1);
	}
	else {
		LOG_PRINTF(("OPEN"));
	}

	LOG_PRINTF(("add table"));
	rc = sqlite3_exec(db, "create table tamanho (nome varchar(10))", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	for(i=1;i<=20000;i++) {
		// add table!
		char sql[100];

		memset(sql,0x0,sizeof(sql));
		sprintf(sql,"insert into tamanho (nome) values ('%d')", i);
		LOG_PRINTF(("insert %d", i));
		rc = sqlite3_exec(db, sql, tests_callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
			sqlite3_free(zErrMsg);
		}
		else {
			LOG_PRINTF(("OK %d!", i));
		}
	}


	sqlite3_close(db);
}

//////////////////////////////////////
/// TEST2

void test2(void) {
	sqlite3* db;
	char *zErrMsg = 0;
	int rc;
	int i;


	LOG_PRINTF(("TESTE 2"));

	rc = sqlite3_open(dbname, &db);
	if(rc) {
		LOG_PRINTF(("Error opening %d %s!", sqlite3_errcode(db), sqlite3_errmsg(db)));

		sqlite3_close(db);
		exit(1);
	}
	else {
		LOG_PRINTF(("OPEN"));
	}

	LOG_PRINTF(("add table"));
	rc = sqlite3_exec(db, "create table salas (id_sala int primary key, qtd int)", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	LOG_PRINTF(("add table"));
	rc = sqlite3_exec(db, "create table alunos (id_aluno int primary key, nome varchar(50), id_sala int)", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	for(i=1;i<=10;i++) {
		// add table!
		char sql[100];

		memset(sql,0x0,sizeof(sql));
		sprintf(sql,"insert into salas (id_sala,qtd) values (%d,%d)", i,i);
		LOG_PRINTF(("insert %d", i));
		rc = sqlite3_exec(db, sql, tests_callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
			sqlite3_free(zErrMsg);
		}
		else {
			LOG_PRINTF(("OK %d!", i));
		}

		memset(sql,0x0,sizeof(sql));
		sprintf(sql,"insert into alunos (id_aluno,nome,id_sala) values (NULL,'aluno %d',%d)", i, i);
		LOG_PRINTF(("insert %d", i));
		rc = sqlite3_exec(db, sql, tests_callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
			sqlite3_free(zErrMsg);
		}
		else {
			LOG_PRINTF(("OK %d!", i));
		}
	}

	// select!
	LOG_PRINTF(("select"));
	rc = sqlite3_exec(db, "select * from alunos inner join salas on alunos.id_sala = salas.id_sala", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}


	sqlite3_close(db);
}

////////////////////////////////////////////////
// TEST1
////////////////////////////////////////////////

void test1(void) {
	sqlite3* db;
	char *zErrMsg = 0;
	int rc;
	int i;

	LOG_PRINTF(("TESTE 1"));

	rc = sqlite3_open(dbname, &db);
	if(rc) {
		LOG_PRINTF(("Error opening %d %s!", sqlite3_errcode(db), sqlite3_errmsg(db)));

		sqlite3_close(db);
		exit(1);
	}
	else {
		LOG_PRINTF(("OPEN"));
	}

	// add table!
	LOG_PRINTF(("add table"));
	rc = sqlite3_exec(db, "create table pessoas (nome varchar(10), idade int)", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	for(i=1;i<=10;i++) {
		// add table!
		char sql[100];

		memset(sql,0x0,sizeof(sql));
		sprintf(sql,"insert into pessoas (nome,idade) values ('nome %d',%d)", i, i);
		LOG_PRINTF(("insert %d", i));
		rc = sqlite3_exec(db, sql, tests_callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
			sqlite3_free(zErrMsg);
		}
		else {
			LOG_PRINTF(("OK %d!", i));
		}
	}

	// select!
	LOG_PRINTF(("select"));
	rc = sqlite3_exec(db, "select * from pessoas order by idade desc", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	// update!
	LOG_PRINTF(("update"));
	rc = sqlite3_exec(db, "update pessoas set idade=idade*2", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	// select!
	LOG_PRINTF(("select"));
	rc = sqlite3_exec(db, "select * from pessoas order by idade desc", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	// delete!
	LOG_PRINTF(("delete"));
	rc = sqlite3_exec(db, "delete from pessoas where idade < 10", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	// select!
	LOG_PRINTF(("select"));
	rc = sqlite3_exec(db, "select * from pessoas order by idade desc", tests_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		LOG_PRINTF(("SQL error %d: %s\n", sqlite3_errcode(db), zErrMsg));
		sqlite3_free(zErrMsg);
	}
	else {
		LOG_PRINTF(("OK!"));
	}

	sqlite3_close(db);
}

///////////////////////////////////////////////////////////////

int main() {

	LOG_INIT("MAU", LOGSYS_COMM, 0xFFFFFFFFL);
	LOG_PRINTF(("Started!"));

    SVC_WAIT(2000);

	// basico!
	test1();

    SVC_WAIT(2000);

    // inner join
	test2();

	SVC_WAIT(2000);

    // 20000 inserts
	test3();

	SVC_WAIT(2000);

    return 0;

}
