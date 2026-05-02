#ifndef JSON_OBJECTS
#define JSON_OBJECTS

#include <Containers/Vector.hpp>
#include "define.h"


namespace gce {

////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief An enum to differenciate the different Json types to clarify the parsing process.
////////////////////////////////////////////////////////////////////////////////////////////////
enum JsonType : char8
{
	JSON_DEFAULT,
	JSON_OBJ,
	JSON_ARR,
	JSON_NUMBER,
	JSON_STRING,
	JSON_LITTERAL
};

///////////////////////////////////////////////////////////////
/// @brief A mother class made to instanciate Json Objects.
///////////////////////////////////////////////////////////////
class JsonValue
{
public:
	virtual ~JsonValue();
	JsonValue* GetParent();
	void SetParent(JsonValue* parent);

private:
	JsonValue* m_parent = nullptr;
};

//////////////////////////////////////////
/// @brief A pair of Json key / value.
//////////////////////////////////////////
class JsonPair final
{
public:
	void Set(String key, JsonValue* value);
	void SetKey(String key);
	void SetValue(JsonValue* value);
	JsonPair* Get();
	String GetKey();
	JsonValue* GetValue();

private:
	String m_key;
	JsonValue* m_pValue = nullptr;
};

/////////////////////////////
/// @brief A Json Object.
/////////////////////////////
class JsonObject : public JsonValue
{
public:
	JsonObject();
	~JsonObject() override = default;
	void AddPair(String key, JsonValue* value);
	void AddPair(JsonPair* pair);
	JsonPair* GetPair(String key);
	JsonValue* GetElement(String key);
	const Vector<JsonPair*>& GetElements() const;

private:
	bool m_isRoot;
	Vector<JsonPair*> m_elements;
};

////////////////////////////
/// @brief A Json Array.
////////////////////////////
class JsonArray : public JsonValue
{
public:
	JsonArray();
	~JsonArray() override = default;

	void Append(JsonValue* value);
	JsonValue* GetValue(uint64 index);
	const Vector<JsonValue*>& GetValues() const;

private:
	Vector<JsonValue*> m_values;
};

//////////////////////////////
/// @brief A Json Integer.
//////////////////////////////
class JsonInteger : public JsonValue
{
public:
	JsonInteger();
	JsonInteger(int64 value);
	~JsonInteger() override = default;
	int64 GetValue() const { return m_value; }

private:

	int64 m_value;
};

////////////////////////////
/// @brief A Json Float.
////////////////////////////
class JsonFloat : public JsonValue
{
public:
	JsonFloat();
	JsonFloat(float64 value);
	~JsonFloat() override = default;

	float64 GetValue() const { return m_value; }
private:
	float64 m_value;
};

//////////////////////////////
/// @brief A Json Boolean.
//////////////////////////////
class JsonBoolean : public JsonValue
{
public:
	JsonBoolean();
	JsonBoolean(bool value);
	~JsonBoolean() override = default;

	bool GetValue() const { return m_value; }

private:
	bool m_value;
};

/////////////////////////////
/// @brief A Json String.
/////////////////////////////
class JsonString : public JsonValue
{
public:
	JsonString();
	JsonString(String value);
	~JsonString() override = default;

	String GetValue() const { return m_value; }
private:
	String m_value;
};

///////////////////////////
/// @brief A Json Null.
///////////////////////////
class JsonNull : public JsonValue
{
public:
	JsonNull();
	~JsonNull() override = default;
	String GetValue() const { return m_value; }
private:
	String m_value;
};

}
#endif



