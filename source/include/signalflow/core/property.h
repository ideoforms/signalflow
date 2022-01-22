#pragma once

#include "signalflow/core/constants.h"
#include <memory>
#include <string>
#include <vector>

namespace signalflow
{

typedef enum : unsigned int
{
    SIGNALFLOW_PROPERTY_TYPE_NONE,
    SIGNALFLOW_PROPERTY_TYPE_INT,
    SIGNALFLOW_PROPERTY_TYPE_FLOAT,
    SIGNALFLOW_PROPERTY_TYPE_FLOAT_ARRAY,
    SIGNALFLOW_PROPERTY_TYPE_STRING,
    SIGNALFLOW_PROPERTY_TYPE_STRING_ARRAY
} signalflow_property_type_t;

class Property
{
public:
    virtual ~Property() {}

    virtual signalflow_property_type_t get_property_type() { return SIGNALFLOW_PROPERTY_TYPE_NONE; }

    virtual int int_value() { return 0; }
    virtual float float_value() { return 0.0; }
    virtual std::string string_value() { return ""; }
    virtual std::vector<float> float_array_value() { return std::vector<float>(); }
    virtual std::vector<std::string> string_array_value() { return std::vector<std::string>(); }

    virtual void set_int_value(int value) { UNUSED(value); }
    virtual void set_float_value(float value) { UNUSED(value); }
    virtual void set_string_value(std::string value) { UNUSED(value); }
    virtual void set_float_array_value(std::vector<float> value) { UNUSED(value); }
    virtual void set_string_array_value(std::vector<std::string> value) { UNUSED(value); }
};

template <typename T>
class TypedProperty : public Property
{
public:
    TypedProperty(const T &value)
        : value(value) {}
    void set(T new_value) { value = new_value; }

protected:
    T value;
};

class FloatProperty : public TypedProperty<float>
{
public:
    FloatProperty(float value)
        : TypedProperty(value) {}

    virtual signalflow_property_type_t get_property_type() override { return SIGNALFLOW_PROPERTY_TYPE_FLOAT; }
    virtual int int_value() override { return value; }
    virtual float float_value() override { return value; }
    virtual std::string string_value() override { return std::to_string(value); }
    virtual void set_int_value(int new_value) override { value = new_value; }
    virtual void set_float_value(float new_value) override { value = new_value; }
};

class IntProperty : public TypedProperty<int>
{
public:
    IntProperty(int value)
        : TypedProperty(value) {}

    virtual signalflow_property_type_t get_property_type() override { return SIGNALFLOW_PROPERTY_TYPE_INT; }
    virtual int int_value() override { return value; }
    virtual float float_value() override { return value; }
    virtual std::string string_value() override { return std::to_string(value); }
    virtual void set_int_value(int new_value) override { value = new_value; }
    virtual void set_float_value(float new_value) override { value = new_value; }
};

class StringProperty : public TypedProperty<std::string>
{
public:
    StringProperty(std::string value)
        : TypedProperty(value) {}

    virtual signalflow_property_type_t get_property_type() override { return SIGNALFLOW_PROPERTY_TYPE_STRING; }
    virtual std::string string_value() override { return value; }
    virtual void set_string_value(std::string new_value) override { value = new_value; }
};

class FloatArrayProperty : public TypedProperty<std::vector<float>>
{
public:
    FloatArrayProperty(std::vector<float> value)
        : TypedProperty(value) {}

    virtual signalflow_property_type_t get_property_type() override { return SIGNALFLOW_PROPERTY_TYPE_FLOAT_ARRAY; }
    virtual std::vector<float> float_array_value() override { return value; }
    // TODO: virtual std::string string_value() override {  }
    virtual void set_float_array_value(std::vector<float> new_value) override { value = new_value; }
};

class StringArrayProperty : public TypedProperty<std::vector<std::string>>
{
public:
    StringArrayProperty(std::vector<std::string> value)
        : TypedProperty(value) {}

    virtual signalflow_property_type_t get_property_type() override { return SIGNALFLOW_PROPERTY_TYPE_STRING_ARRAY; }
    virtual std::vector<std::string> string_array_value() override { return value; }
    // TODO: virtual std::string string_value() override {  }
    virtual void set_string_array_value(std::vector<std::string> new_value) override { value = new_value; }
};

template <class T>
class PropertyRefTemplate : public std::shared_ptr<T>
{
public:
    using std::shared_ptr<Property>::shared_ptr;

    PropertyRefTemplate()
        : std::shared_ptr<T>() {}
    PropertyRefTemplate(Property *property)
        : std::shared_ptr<T>(property) {}
    PropertyRefTemplate(double value)
        : std::shared_ptr<T>(new FloatProperty(value)) {}
    PropertyRefTemplate(int value)
        : std::shared_ptr<T>(new IntProperty(value)) {}
    PropertyRefTemplate(std::string value)
        : std::shared_ptr<T>(new StringProperty(value)) {}
    PropertyRefTemplate(const char *value)
        : std::shared_ptr<T>(new StringProperty(value)) {}
    PropertyRefTemplate(std::initializer_list<float> value)
        : std::shared_ptr<T>(new FloatArrayProperty(value)) {}
    PropertyRefTemplate(std::vector<float> value)
        : std::shared_ptr<T>(new FloatArrayProperty(value)) {}
    PropertyRefTemplate(std::initializer_list<std::string> value)
        : std::shared_ptr<T>(new StringArrayProperty(value)) {}
    PropertyRefTemplate(std::vector<std::string> value)
        : std::shared_ptr<T>(new StringArrayProperty(value)) {}
};

typedef PropertyRefTemplate<Property> PropertyRef;

}
