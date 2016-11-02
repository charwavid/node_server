/*
 * Mysql_Operator.h
 *
 *  Created on: Jul 20, 2016
 *      Author: zhangyalei
 */

#ifndef MYSQL_OPERATOR_H_
#define MYSQL_OPERATOR_H_

#include <unordered_map>
#include "Mysql_Conn.h"
#include "DB_Operator.h"

class Mysql_Operator : public DB_Operator {
public:
	typedef std::unordered_map<int, Mysql_Conn *> Connection_Map;
	//typedef std::unordered_map<std::string, Connection_Map *> DB_Connection_Map;

	Mysql_Operator(void);
	virtual ~Mysql_Operator(void);

	virtual bool connect_to_db(int db_id, std::string &ip, int port, std::string &user, std::string &password, std::string &pool_name);
	virtual int init_db(int db_id, DB_Struct *db_struct);
	virtual int64_t generate_table_index(int db_id, DB_Struct *db_struct, std::string &type);
	virtual int64_t select_table_index(int db_id, DB_Struct *db_struct, std::string &query_name, std::string &query_value);

	//数据库加载出来的数据转换城js object，保存数据时候通过object取数据
	virtual v8::Local<v8::Object> load_data(int db_id, DB_Struct *db_struct, Isolate* isolate, int64_t key_index);
	virtual void save_data(int db_id, DB_Struct *db_struct, Isolate* isolate, v8::Local<v8::Object> object);

	///数据库加载出来的数据转换城buffer，缓存在内存中，保存数据时候通过buffer取数据
	virtual int load_data(int db_id, DB_Struct *db_struct, int64_t key_index, std::vector<Bit_Buffer *> &buffer_vec);
	virtual void save_data(int db_id, DB_Struct *db_struct, Bit_Buffer *buffer);

	//删除数据时候传入索引数组
	virtual void delete_data(int db_id, DB_Struct *db_struct, Isolate* isolate, v8::Local<v8::Object> object);

private:
	Mysql_Conn *get_connection(int db_id);

	v8::Local<v8::Object> load_data_single(DB_Struct *db_struct, Isolate* isolate, sql::ResultSet *result);
	v8::Local<v8::Value> load_data_arg(DB_Struct *db_struct, Isolate* isolate, const Field_Info &field_info, sql::ResultSet *result);
	v8::Local<v8::Array> load_data_vector(DB_Struct *db_struct, Isolate* isolate, const Field_Info &field_info, sql::ResultSet *result);
	v8::Local<v8::Map> load_data_map(DB_Struct *db_struct, Isolate* isolate, const Field_Info &field_info, sql::ResultSet *result);
	v8::Local<v8::Object> load_data_struct(DB_Struct *db_struct, Isolate* isolate, const Field_Info &field_info, sql::ResultSet *result);

	void load_data_single(DB_Struct *db_struct, sql::ResultSet *result, Bit_Buffer &buffer);
	void load_data_arg(DB_Struct *db_struct, const Field_Info &field_info, sql::ResultSet *result, Bit_Buffer &buffer);
	void load_data_vector(DB_Struct *db_struct, const Field_Info &field_info, sql::ResultSet *result, Bit_Buffer &buffer);
	void load_data_struct(DB_Struct *db_struct, const Field_Info &field_info, sql::ResultSet *result, Bit_Buffer &buffer);

	int build_len_arg(DB_Struct *db_struct, const Field_Info &field_info, Byte_Buffer &buffer);
	int build_len_vector(DB_Struct *db_struct, const Field_Info &field_info, Byte_Buffer &buffer);
	int build_len_struct(DB_Struct *db_struct, const Field_Info &field_info, Byte_Buffer &buffer);

	//根据byte_buffer生成bit_buffer
	int build_bit_buffer_arg(DB_Struct *db_struct, const Field_Info &field_info, Bit_Buffer &bit_buffer, Byte_Buffer &byte_buffer);
	int build_bit_buffer_vector(DB_Struct *db_struct, const Field_Info &field_info, Bit_Buffer &bit_buffer, Byte_Buffer &byte_buffer);
	int build_bit_buffer_struct(DB_Struct *db_struct, const Field_Info &field_info, Bit_Buffer &bit_buffer, Byte_Buffer &byte_buffer);

	//根据bit_buffer生成byte_buffer
	int build_byte_buffer_arg(DB_Struct *db_struct, const Field_Info &field_info, Bit_Buffer &bit_buffer, Byte_Buffer &byte_buffer);
	int build_byte_buffer_vector(DB_Struct *db_struct, const Field_Info &field_info, Bit_Buffer &bit_buffer, Byte_Buffer &byte_buffer);
	int build_byte_buffer_struct(DB_Struct *db_struct, const Field_Info &field_info, Bit_Buffer &bit_buffer, Byte_Buffer &byte_buffer);

private:
	Connection_Map connection_map_;
};

#endif /* MYSQL_OPERATOR_H_ */
