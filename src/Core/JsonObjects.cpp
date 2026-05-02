#include "JsonObjects.h"

using namespace gce;

JsonValue::~JsonValue()
{
	delete m_parent;
}

void JsonValue::SetParent(JsonValue* parent)
{
	m_parent = parent;
}

JsonValue* JsonValue::GetParent()
{
	return m_parent;
}



// Pair
void JsonPair::Set(String key, JsonValue* value)
{
	m_key = key;
	m_pValue = value;
}

JsonPair* JsonPair::Get()
{
	return this;
}

void JsonPair::SetKey(String key)
{
	m_key = key;
}

void JsonPair::SetValue(JsonValue* value)
{
	m_pValue = value;
}

String JsonPair::GetKey()
{
	return m_key;
}

JsonValue* JsonPair::GetValue()
{
	return m_pValue;
}



// Object
JsonObject::JsonObject() : m_elements(), m_isRoot(false) {}

void JsonObject::AddPair(String key, JsonValue* value)
{
	JsonPair* pPair = new JsonPair();
	pPair->Set(key, value);
	m_elements.PushBack(pPair);
}

void JsonObject::AddPair(JsonPair* pair)
{
	m_elements.PushBack(pair);
}

JsonPair* JsonObject::GetPair(String key)
{
	for (auto it = m_elements.begin(); it != m_elements.end(); it++)
	{
		if ((*it)->GetKey() == key)
			return (*it);
	}

	return nullptr;
}

JsonValue* JsonObject::GetElement(String key)
{
	for (auto it = m_elements.begin(); it != m_elements.end(); it++)
	{
		if ((*it)->GetKey() == key)
			return (*it)->GetValue();
	}

	return nullptr;
}

const Vector<JsonPair*>& gce::JsonObject::GetElements() const
{
	return m_elements;
}



// Array
JsonArray::JsonArray() : m_values() {}

void JsonArray::Append(JsonValue* value)
{
	m_values.PushBack(value);
}

JsonValue* JsonArray::GetValue(uint64 index)
{
	return m_values[index];
}

const Vector<JsonValue*>& gce::JsonArray::GetValues() const
{
	return m_values;
}

// Integer
JsonInteger::JsonInteger() : m_value(0) {}

JsonInteger::JsonInteger(int64 value) : m_value(value) {}


// Float
JsonFloat::JsonFloat() : m_value(0.0f) {}

JsonFloat::JsonFloat(float64 value) : m_value(value) {}

// Boolean
JsonBoolean::JsonBoolean() : m_value(false) {}

JsonBoolean::JsonBoolean(bool value) : m_value(value) {}

// String
JsonString::JsonString() : m_value("") {}

JsonString::JsonString(String value) : m_value(value) {}

// Null
JsonNull::JsonNull() : m_value("null") {}
