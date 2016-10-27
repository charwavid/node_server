/*
 * Mysql_Operator.h
 *
 *  Created on: Jul 20, 2016
 *      Author: zhangyalei
 */

#ifndef MYSQL_OPERATOR_H_
#define MYSQL_OPERATOR_H_

#include "boost/unordered_map.hpp"
#include "Mysql_Conn.h"
#include "DB_Operator.h"

class Mysql_Operator : public DB_Operator {
public:
	typedef boost::unordered_map<int, Mysql_Conn *> Connection_Map;
	//typedef boost::unordered_map<std::string, Connection_Map *> DB_Connection_Map;

	Mysql_Operator(void);
	virtual ~Mysql_Operator(void);

	virtual bool connect_to_db(int db_id, std::string &ip, int port, std::string &user, std::string &password, std::string &pool_name);

	virtual v8::Local<v8::Object> load_data(int db_id, DB_Struct *db_struct, Isolate* isolate, int64_t key_index);
	virtual void save_data(int db_id, DB_Struct *db_struct, Isolate* isolate, v8::Local<v8::Object> object);
	virtual void delete_data(int db_id, DB_Struct *db_struct, Isolate* isolate, v8::Local<v8::Object> object);

	virtual int init_db(int db_id, DB_Struct *db_struct);
	virtual int64_t generate_table_index(int db_id, DB_Struct *db_struct, std::string &type);
	virtual int64_t select_table_index(int db_id, DB_Struct *db_struct, std::string &query_name, std::string &query_value);

private:
	Mysql_Conn *get_connection(int db_id);

	v8::Local<v8::Object> load_data_single(DB_Struct *db_struct, Isolate* isolate, sql::ResultSet *result);
	v8::Local<v8::Value> load_data_arg(DB_Struct *db_struct, Isolate* isolate, const Field_Info &field_info, sql::ResultSet *result);
	v8::Local<v8::Array> load_data_vector(DB_Struct *db_struct, Isolate* isolate, const Field_Info &field_info, sql::ResultSet *result);
	v8::Local<v8::Map> load_data_map(DB_Struct *db_struct, Isolate* isolate, const Field_Info &field_info, sql::ResultSet *result);
	v8::Local<v8::Object> load_data_struct(DB_Struct *db_struct, Isolate* isolate, const Field_Info &field_info, sql::ResultSet *result);

	int build_len_arg(DB_Struct *db_struct, const Field_Info &field_info, Block_Buffer &buffer);
	int build_len_vector(DB_Struct *db_struct, const Field_Info &field_info, Block_Buffer &buffer);
	int build_len_struct(DB_Struct *db_struct, const Field_Info &field_info, Block_Buffer &buffer);

private:
	Connection_Map connection_map_;
};

#endif /* MYSQL_OPERATOR_H_ */