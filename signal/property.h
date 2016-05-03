#pragma once 

#include <string>
#include <vector>
#include <memory>

class Property
{
	public:
		virtual int int_value() { return 0; }
		virtual float float_value() { return 0.0; }
		virtual std::string string_value() { return ""; }
		virtual std::vector <float> float_array_value() { return std::vector <float> (); }
		virtual std::vector <std::string> string_array_value() { return std::vector <std::string> (); }
};

template <typename T>
class TypedProperty : public Property
{
	public:
		TypedProperty (const T &value) : value(value) { }
		T get_value() { return value; }

	protected:
		const T value;
};

class FloatProperty : public TypedProperty <float>
{
	public:
		FloatProperty(float value) : TypedProperty(value) {}
		virtual int int_value() override { return value; }
		virtual float float_value() override { return value; }
};

class IntProperty : public TypedProperty <int>
{
	public:
		IntProperty(int value) : TypedProperty(value) {}
		virtual int int_value() override { return value; }
		virtual float float_value() override { return value; }
};

class StringProperty : public TypedProperty <std::string>
{
	public:
		StringProperty(std::string value) : TypedProperty(value) {}
		virtual std::string string_value() { return value; }
};

class FloatArrayProperty : public TypedProperty <std::vector <float>>
{
	public:
		FloatArrayProperty(std::vector <float> value) : TypedProperty(value) {}
		virtual std::vector <float> float_array_value() { return value; }
};

class StringArrayProperty : public TypedProperty <std::vector <std::string>>
{
	public:
		StringArrayProperty(std::vector <std::string> value) : TypedProperty(value) {}
		virtual std::vector <std::string> string_array_value() { return value; }
};


class PropertyRef : public std::shared_ptr<Property>
{
	public:
		using std::shared_ptr<Property>::shared_ptr;

		PropertyRef() : std::shared_ptr<Property>() { }
		PropertyRef(Property *property) : std::shared_ptr<Property>(property) { }
		PropertyRef(double value) : std::shared_ptr<Property>(new FloatProperty(value)) { }
		PropertyRef(int value) : std::shared_ptr<Property>(new IntProperty(value)) { }
		PropertyRef(std::string value) : std::shared_ptr<Property>(new StringProperty(value)) { }
		PropertyRef(const char *value) : std::shared_ptr<Property>(new StringProperty(value)) { }
		PropertyRef(std::initializer_list <float> value) : std::shared_ptr<Property>(new FloatArrayProperty(value)) { }
		PropertyRef(std::vector <float> value) : std::shared_ptr<Property>(new FloatArrayProperty(value)) { }
		PropertyRef(std::initializer_list <std::string> value) : std::shared_ptr<Property>(new StringArrayProperty(value)) { }
		PropertyRef(std::vector <std::string> value) : std::shared_ptr<Property>(new StringArrayProperty(value)) { }
};

