#include "JsonObjects.h"
#include "File.h"

#ifndef CORE_JSON_PARSER_H_INCLUDED
#define CORE_JSON_PARSER_H_INCLUDED

namespace gce {

	////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief JsonParser is a static class made to Deserialize a Json file into memory objects.
	////////////////////////////////////////////////////////////////////////////////////////////////
	class JsonParser
	{
	public:
		JsonParser();
		~JsonParser() = default;

		static JsonPair* ParsePair(File* file, JsonValue* caller);
		
		static File* RunFile(String path, File::Type type);
		static void StopReading(File* file);

		static String ReadKey(File* file);
		static JsonType ReachValue(File* file);
		static void SkipUntil(File* file, char8 target, char8 stop);
		static void SkipUntilLitteral(File* file);
		static void SkipUntilDigit(File* file, char8 stop);

		static JsonObject* ParseObject(File* file, JsonValue* caller);
		static JsonArray* ParseArray(File* file, JsonValue* caller);	
		static JsonValue* ParseNumber(File* file, JsonValue* caller); 
		static JsonValue* ParseString(File* file, JsonValue* caller);
		static JsonValue* ParseLitteral(File* file, JsonValue* caller);

		static Vector<JsonObject*>* Deserialize(String path);

		static String Indent(uint32 level);
		static bool IsSimpleValue(JsonValue* pValue);
		static bool IsSimpleArray(JsonArray* pArray);
		static bool IsSimpleObject(JsonObject* pObject);

		static String SerializeValue(JsonValue* pValue, uint32 indentLevel = 0);
		static File* Serialize(JsonObject* pRootObject, String path);
	};
}

#endif