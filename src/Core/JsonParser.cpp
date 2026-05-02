#include "File.h"
#include "JsonParser.h"
#include "JsonObjects.h"

namespace gce
{
	bool IsFloat(String toVerify)
	{
		const uint8 size = static_cast<uint8>(toVerify.size());

		int8 commaCount = 0;

		for (uint64 i = 0; i < size; i++)
		{
			if (toVerify[i] == '.') {
				commaCount++;
				if (commaCount > 1) return false;
				continue;
			}

			if (i == 0 && toVerify[i] == '-')
				continue;

			uint16 ASCII = (uint16)toVerify[i];

			if (47 < ASCII && ASCII < 58)
				continue;
			else
				return false;
		}

		if(commaCount == 0)
			return false;

		return true;
	}

	float ConvertStringToFloat(String toConvert)
	{
		if (!IsFloat(toConvert))
			return 0.0f;

		const uint8 size = static_cast<uint8>(toConvert.size());
		float32 sign = 1.0f;
		uint8 commaIndex = -1;
		float32 result = 0.0f;
		float divider = 10.0f;

		for (uint32 i = 0; i < size; i++)
		{
			if (i == 0 && toConvert[i] == '-')
			{
				sign = -1.0f;
				continue;
			}

			if (toConvert[i] == '.')
			{
				commaIndex = i;
				break;
			}

			result = result * 10 + (toConvert[i] - '0');
		}

		for (uint32 j = commaIndex + 1; j < toConvert.size(); j++)
		{
			result += (toConvert[j] - '0') / divider;
			divider *= 10.0f;
		}

		return result * sign;
	}

	int64 ConvertStringToInt(String toConvert)
	{
		int64 result = 0;
		int8 sign = 1;

		uint64 i = 0;

		while (toConvert[i] != '\0')
		{
			if (i == 0 && toConvert[i] == '-')
			{
				sign = -1;
				i++;
				continue;
			}

			if (47 < toConvert[i] && toConvert[i] < 58)
			{
				result = toConvert[i] * 10;
				i++;
			}
			else
				return NULL;
		}

		return result * sign;
	}

	bool IsDigit(char8 c)
	{
		return (47 < (uint64)c && (uint64)c < 58);
	}

	bool IsInt(String toVerify)
	{
		uint64 size = toVerify.size();

		for (uint64 i = 0; i < size; i++)
		{
			if (!IsDigit(toVerify[i]))
				return false;
		}
		return true;
	}

	bool IsLitteral(char8 toVerify)
	{
		return (((96 < (uint64)toVerify && (uint64)toVerify < 123) or (64 < (uint64)toVerify && (uint64)toVerify < 91)));
	}

	char8 GetCursorChar(gce::File* file)
	{
		file->MoveCursor(-1);
		return file->ReadByte();
	}
}

using namespace gce;

JsonParser::JsonParser() {}

///////////////////////////////////////////////////////////////////////////////
/// @brief Opens a file using the specified path and access mode.
/// @param path Path to the file to open.
/// @param type File access mode (READ_ONLY, OVER_WRITE, etc.).
/// @return Pointer to the opened File, or nullptr if the file could not be opened.
/// @note Automatically deletes the File object on failure to avoid memory leaks.
///////////////////////////////////////////////////////////////////////////////
File* JsonParser::RunFile(String path, File::Type type)
{
	File* pFile = new File(path, type);
	if (!pFile->IsOpened())
	{
		delete pFile;
		return nullptr;
	}
	return pFile;
}

////////////////////////////////////////////////////////////////////
/// @brief Closes the Json File and delete
/// @param path The path to the Json File
////////////////////////////////////////////////////////////////////
void JsonParser::StopReading(File* file)
{
	file->Close();
	if (!file->IsOpened())
		file->~File();
}

//////////////////////////////////////////////////////
/// @brief Reads an entire Json Key.
/// @param file A pointer to the Json file opened.
/// @return A String which contains the read key.
//////////////////////////////////////////////////////
String JsonParser::ReadKey(File* file)
{
	file->MoveCursor(-1);
	char8 current = file->ReadByte();
	String buffer = "";

	while (current != '"')
		current = file->ReadByte();

	current = file->ReadByte();

	while (current != '"')
	{
		buffer += current;
		current = file->ReadByte();
	}

	return buffer;
}

////////////////////////////////////////////////////////////////////
/// @brief Browse the file to find the nature of the next value.
/// @param file A pointer to the Json file opened.
/// @return The nature of the next value.
////////////////////////////////////////////////////////////////////
JsonType JsonParser::ReachValue(File* file)
{
	bool reachedClosing = false;
	JsonType type = JsonType::JSON_DEFAULT;
	char8 current;

	while (type == JsonType::JSON_DEFAULT and reachedClosing == false)
	{
		current = file->ReadByte();
		switch (current)
		{
		case ('{'):
			type = JsonType::JSON_OBJ;
			break;

		case ('['):
			type = JsonType::JSON_ARR;
			break;
		case (']'):
			reachedClosing = true;
			break;
		default :
			if (gce::IsDigit(current) or current == '-')
			{
				type = JsonType::JSON_NUMBER;
				break;
			}

			else if (current == '"')
			{
				type = JsonType::JSON_STRING;
				break;
			}

			else if (current == 'f' or current == 't' or current == 'n')
			{
				type = JsonType::JSON_LITTERAL;
				break;
			}

			continue;
		}
	}

	file->MoveCursor(-1);
	return type;
}

//////////////////////////////////////////////////////////////////////
/// @brief Skips characters while they aren't matching the target.
/// @param file A pointer to the Json file opened.b
/// @param target The char to search for.
/// @param stop An intruder char. If ran across, we exit.
//////////////////////////////////////////////////////////////////////
void JsonParser::SkipUntil(File* file, char8 target, char8 stop)
{
	char8 current = gce::GetCursorChar(file);
	while (current != target)
	{
		if (current == stop)
			break;
		current = file->ReadByte();
	}
}

//////////////////////////////////////////////////////////////////////
/// @brief Skips characters while they aren't matching a litteral.
/// @param file A pointer to the Json file opened.
//////////////////////////////////////////////////////////////////////
void JsonParser::SkipUntilLitteral(File* file)
{
	char8 current = gce::GetCursorChar(file);
	while (current != 'f' and current != 't' and current != 'n')
	{
		current = file->ReadByte();
	}

	file->MoveCursor(-1);
}

///////////////////////////////////////////////////////////////////
/// @brief Skips characters while they aren't matching a digit.
/// @param file A pointer to the Json file opened.
/// @param stop An intruder char. If ran across, we exit.
///////////////////////////////////////////////////////////////////
void JsonParser::SkipUntilDigit(File* file, char8 stop)
{
	file->MoveCursor(-1);
	char8 current = file->ReadByte();

	while (true)
	{
		if (current == stop)
			break;
		
		if (gce::IsDigit(current) || current == '-' || current == '+')
		{
			file->MoveCursor(-1);
			break;
		}
		current = file->ReadByte();
	}
}

////////////////////////////////////////////////////////////
/// @brief Parses a Json Object.
/// @param file A pointer to the Json file opened.
/// @param caller A pointer to the parent Object.
/// @return Returns a pointer to the Json Object parsed.
////////////////////////////////////////////////////////////
JsonObject* JsonParser::ParseObject(File* file, JsonValue* caller = nullptr)
{
	JsonObject* pJsonObject = new JsonObject();
	pJsonObject->SetParent(caller);

	JsonPair* pPair = nullptr;

	file->MoveCursor(1);
	char8 current = file->ReadByte();
	while (current != '}')
	{
		if (current == ' ' or current == '\r' or current == '\n')
		{
			//current = file->AddCursor(1);
			current = file->ReadByte();
			continue;
		}
		if (current == '}')
			continue;
		
		pPair = ParsePair(file, pJsonObject);
		pJsonObject->AddPair(pPair);

		//current = file->AddCursor(1);
		current = file->ReadByte();
		
		if (current != ',' and current != '}')
			//current = file->AddCursor(1);
			current = file->ReadByte();

	}

	return pJsonObject;
}

////////////////////////////////////////////////////////////
/// @brief Parses a Json Array.
/// @param file A pointer to the Json file opened.
/// @param caller A pointer to the parent Array.
/// @return Returns a pointer to the Json Array parsed.
////////////////////////////////////////////////////////////
JsonArray* JsonParser::ParseArray(File* file, JsonValue* caller = nullptr)
{
	JsonArray* pJsonArray = new JsonArray();
	pJsonArray->SetParent(caller);    
	
	//file->AddCursor(-1);
	char8 current = file->ReadByte();

	JsonValue* pValue = nullptr;
	
	while (current != ']')
	{
		JsonType type = ReachValue(file);
		switch (type)
		{
		case (JsonType::JSON_OBJ):
		{
			SkipUntil(file, '{', ']');
			pValue = ParseObject(file, pJsonArray);
			pJsonArray->Append(pValue);
			break;
		}
		case (JsonType::JSON_ARR):
		{
			file->MoveCursor(-1);
			SkipUntil(file, '[', ']');
			pValue = ParseArray(file, pJsonArray);
			pJsonArray->Append(pValue);
			break;
		}
		case(JsonType::JSON_NUMBER):
		{
			SkipUntilDigit(file, ']');
			pValue = ParseNumber(file, pJsonArray);
			pJsonArray->Append(pValue);
			break;
		}
		case(JsonType::JSON_STRING):
		{
			SkipUntil(file, '"', ']');
			file->MoveCursor(-1);
			pValue = ParseString(file, pJsonArray);
			pJsonArray->Append(pValue);
			break;
		}
		case (JsonType::JSON_LITTERAL):
		{
			SkipUntilLitteral(file);
			pValue = ParseLitteral(file,pJsonArray);
			pJsonArray->Append(pValue);
			break;
		}
		case (JsonType::JSON_DEFAULT):
			current = ']';
			continue;
		}
		current = gce::GetCursorChar(file);
		
		file->MoveCursor(1);

		current = gce::GetCursorChar(file);

		if (current == ']')
			break;
	}
	file->MoveCursor(1);
	return pJsonArray;
}

////////////////////////////////////////////////////////////
/// @brief Parses a Json Integer.
/// @param file A pointer to the Json file opened.
/// @param caller A pointer to the parent Object.
/// @return Returns a pointer to the Json Integer parsed.
////////////////////////////////////////////////////////////
JsonValue* JsonParser::ParseNumber(File* file, JsonValue* caller = nullptr)
{	
	char8 current = file->ReadByte();
	String buffer = "";
	
	while (current != '}' and 
		current != ']' and 
		current != ',' and 
		current != '\r' and 
		current != '\n')
	{
		if (gce::IsDigit(current) || current == '.')
			buffer += current;
		current = file->ReadByte();
	}

	if (!gce::IsFloat(buffer))
	{
		JsonInteger* pJsonInt = new JsonInteger(stoi(buffer));
		pJsonInt->SetParent(caller);
		file->MoveCursor(-1);	
		return pJsonInt;
	}
	else
	{
		JsonFloat* pJsonFloat = new JsonFloat(stof(buffer));
		pJsonFloat->SetParent(caller);
		file->MoveCursor(-1);
		return pJsonFloat;
	}	
}

////////////////////////////////////////////////////////////
/// @brief Parses a Json String.
/// @param file A pointer to the Json file opened.
/// @param caller A pointer to the parent Object.
/// @return Returns a pointer to the Json Object parsed.
////////////////////////////////////////////////////////////
JsonValue* JsonParser::ParseString(File* file, JsonValue* caller = nullptr)
{
	String buffer = "";
	file->MoveCursor(1);
	char8 current = file->ReadByte();

	while (current != '"' and current != ',')
	{
		if (current == '\\')
		{
			current = file->ReadByte();
			switch (current)
			{
			case '"': buffer += '"'; break;
			case '\\': buffer += '\\'; break;
			case '/': buffer += '/'; break;
			case 'b': buffer += '\b'; break;
			case 'f': buffer += '\f'; break;
			case 'n': buffer += '\n'; break;
			case 'r': buffer += '\r'; break;
			case 't': buffer += '\t'; break;
			default:
				break;
			}
		}
		else
		{
			buffer += current;
		}

		current = file->ReadByte();
	}

	JsonValue* pJson = new JsonString(buffer);

	pJson->SetParent(caller);
	return pJson;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Parses a Json String. This function can parse Integers, Floats, Null, and Booleans.
/// @param file A pointer to the Json file opened.
/// @param caller A pointer to the parent Object.
/// @return Returns a pointer to the Json Litteral parsed.
//////////////////////////////////////////////////////////////////////////////////////////////////
JsonValue* JsonParser::ParseLitteral(File* file, JsonValue* caller)
{
	JsonValue* pJson = nullptr;

	String buffer = "";
	char8 current = file->ReadByte();

	while (gce::IsLitteral(current))
	{
		buffer += current;
		current = file->ReadByte();
	}

	if (buffer == "false")
	{
		pJson = new JsonBoolean(false);
	}
	else if (buffer == "true")
	{
		pJson = new JsonBoolean(true);
	}
	else if (buffer == "null")
	{
		pJson = new JsonNull();
	}
	else
		return nullptr;

	pJson->SetParent(caller);
	return pJson;
}

///////////////////////////////////////////////////////////////////////////
/// @brief This function parses a key/value Json pair.
/// @param file A pointer to the Json File currently opened.
/// @param caller A pointer to the parent object. Used for recursivity.
///////////////////////////////////////////////////////////////////////////
JsonPair* JsonParser::ParsePair(File* file, JsonValue* caller = nullptr)
{
	String key = ReadKey(file);
	JsonType type = ReachValue(file);

	JsonValue* pValue = nullptr;

	JsonPair* pPair = new JsonPair();

	switch (type)
	{
	case (JsonType::JSON_OBJ):
		pValue = ParseObject(file, caller);
		break;

	case (JsonType::JSON_ARR):
		pValue = ParseArray(file, caller);
		break;

	case (JsonType::JSON_NUMBER) :
		pValue = ParseNumber(file, caller);
		break;
		
	case (JsonType::JSON_STRING) :
		pValue = ParseString(file, caller);
		break;
	case (JsonType::JSON_LITTERAL) :
		pValue = ParseLitteral(file, caller);
		break;
	}

	pValue->SetParent(caller);
	pPair->Set(key, pValue);

	return pPair;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Converts Json content into memory using JsonObjects.h instances.
/// @param path The path to the Json file.
/// @return A pointer to the root JsonObject.
///////////////////////////////////////////////////////////////////////////////
Vector<JsonObject*>* JsonParser::Deserialize(String path)
{
	Vector<JsonObject*>* jsonVector = new Vector<JsonObject*>();

	File* pFile = RunFile(path, File::READ_ONLY);
	
	while (pFile->GetCursor() < pFile->GetSize())
	{
		jsonVector->PushBack(ParseObject(pFile, nullptr));
	}

	StopReading(pFile);

	return jsonVector;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Returns an indentation string.
/// @param level Indentation level.
/// @return A string containing (level * 2) spaces.
///////////////////////////////////////////////////////////////////////////////
String JsonParser::Indent(uint32 level)
{
	return String(level * 2, ' '); 
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether the JSON value is of a primitive type.
/// @param pValue Pointer to a JsonValue.
/// @return True if the value is a string, boolean, float, integer, or null. False otherwise.
///////////////////////////////////////////////////////////////////////////////
bool JsonParser::IsSimpleValue(JsonValue* pValue)
{
	return dynamic_cast<JsonString*>(pValue) || 
		dynamic_cast<JsonBoolean*>(pValue) ||
		dynamic_cast<JsonFloat*>(pValue) ||
		dynamic_cast<JsonInteger*>(pValue) ||
		dynamic_cast<JsonNull*>(pValue);
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether a JSON array contains only simple values.
/// @param pArray Pointer to a JsonArray.
/// @return True if all elements are simple. False otherwise.
///////////////////////////////////////////////////////////////////////////////
bool JsonParser::IsSimpleArray(JsonArray* pArray)
{
	const auto& pValues = pArray->GetValues();
	for (JsonValue* pValue : pValues)
	{
		if (!IsSimpleValue(pValue))
			return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Checks if a JSON object contains only one key with a simple value.
/// @param pObject Pointer to a JsonObject.
/// @return True if the object has one key with a simple value. False otherwise.
///////////////////////////////////////////////////////////////////////////////
bool JsonParser::IsSimpleObject(JsonObject* pObject)
{
	const auto& pPairs = pObject->GetElements();

	if (pPairs.Size() == 1)
		return false;

	JsonValue* pValue = pPairs[0]->GetValue();

	if (dynamic_cast<JsonObject*>(pValue) || dynamic_cast<JsonArray*>(pValue))
		return false;

	return false;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Recursively serializes a JSON value into a formatted string.
/// @param pValue Pointer to the JsonValue to serialize.
/// @param indentLevel Current indentation level (used for formatting).
/// @return A string representing the serialized JSON.
///////////////////////////////////////////////////////////////////////////////
String JsonParser::SerializeValue(JsonValue* pValue, uint32 indentLevel)
{
	if (pValue == nullptr)
		return "null";

	const String indent = Indent(indentLevel + 1);

	//CASE OBJECT
	if (auto pObject = dynamic_cast<JsonObject*>(pValue))
	{
		const auto& pPairs = pObject->GetElements();

		//CASE EMPTY OBJECT
		if (pPairs.Empty())
			return "{}";

		//CASE SIMPLE OBJECT
		if (pPairs.Size() == 1 && IsSimpleObject(pObject))
		{
			auto* pPair = pPairs[0];
			return "{ \"" + pPair->GetKey() + "\": " + SerializeValue(pPair->GetValue(), indentLevel+1) + " }";
		}

		//CASE COMPLEXE OBJECT
		String result = "{\n";
		for (size_t i = 0; i < pPairs.Size(); i++)
		{
			auto* pPair = pPairs[i];
			result += indent + "\"" + pPair->GetKey() + "\": " + SerializeValue(pPair->GetValue(), indentLevel+1);
			if (i < pPairs.Size() - 1)
				result += ",";
			result += "\n";
		}
		result += Indent(indentLevel) + "}";
		return result;
	}

	//CASE ARRAY
	else if (auto pArray = dynamic_cast<JsonArray*>(pValue))
	{
		const auto& pValues = pArray->GetValues();

		//CASE EMPTY ARRAY
		if (pValues.Empty())
			return "[]";

		//SIMPLE ARRAY
		if (IsSimpleArray(pArray)) 
		{
			String result = "[";
			for (size_t i = 0; i < pValues.Size(); i++)
			{
				result += SerializeValue(pValues[i]);
				if (i < pValues.Size() - 1)
					result += ", ";
			}
			result += "]";
			return result;
		}
		
		//COMPLEX ARRAY
		String result = "[\n";
		for (size_t i = 0; i < pValues.Size(); ++i)
		{
			result += indent + SerializeValue(pValues[i], indentLevel + 1);
			if (i < pValues.Size() - 1)
				result += ",";
			result += "\n";
		}
		result += Indent(indentLevel) + "]";
		return result;
	}

	//CASE STRING
	else if (auto pString = dynamic_cast<JsonString*>(pValue))
		return "\"" + pString->GetValue() + "\"";

	//CASE BOOLEAN
	else if (auto pBool = dynamic_cast<JsonBoolean*>(pValue))
		return pBool->GetValue() ? "true" : "false";

	//CASE FLOAT
	else if (auto pFloat = dynamic_cast<JsonFloat*>(pValue))
		return std::to_string(pFloat->GetValue());

	//CASE INT
	else if (auto pInt = dynamic_cast<JsonInteger*>(pValue))
		return std::to_string(pInt->GetValue());

	//CASE NULL
	else if (dynamic_cast<JsonNull*>(pValue))
		return "null";

	return "BLANK";
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Serializes a root JsonObject and writes its content to a JSON file.
/// @param pRootObject Pointer to the root JsonObject to serialize.
/// @param path Destination path for the output .json file.
/// @return Pointer to the written File, or nullptr if the file could not be opened.
/// @note Internally uses RunFile() to handle file opening and SerializeValue() to generate JSON.
///////////////////////////////////////////////////////////////////////////////
File* JsonParser::Serialize(JsonObject* pRootObject, String path)
{
	File* pFile = RunFile(path, File::OVER_WRITE);
	if (pFile == nullptr)
		return nullptr;
	String jsonContent = SerializeValue(pRootObject);
	pFile->Write(jsonContent);
	StopReading(pFile);
	return pFile;
}
