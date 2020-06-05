/*!
 * \file ReflectionTestCase.cpp
 *
 * \author zjhlogo
 * \date 2020/01/23
 *
 *
 */
#include <foundation/JsonSerializer.h>
#include <gtest/gtest.h>

#include <rttr/registration>
#include <rttr/registration_friend>
#include <rttr/type>

enum class color
{
    red,
    green,
    blue
};

struct point2d
{
    point2d() {}
    point2d(int x_, int y_)
        : x(x_)
        , y(y_)
    {
    }
    int x = 0;
    int y = 0;
};

struct shape
{
    shape(){};

    shape(NS::tstring n)
        : name(n)
    {
    }

    void set_visible(bool v) { visible = v; }
    bool get_visible() const { return visible; }

    color color_ = color::blue;
    NS::tstring name = "";
    point2d position;
    std::map<color, point2d> dictionary;

    RTTR_ENABLE()
private:
    bool visible = false;
};

struct circle : shape
{
    circle(){};

    circle(NS::tstring n)
        : shape(n)
    {
    }

    double radius = 5.2;
    std::vector<point2d> points;

    int no_serialize = 100;

    RTTR_ENABLE(shape)
};

static std::vector<circle*> g_values;

RTTR_REGISTRATION
{
    rttr::registration::enumeration<color>("color")(rttr::value("red", color::red), rttr::value("blue", color::blue), rttr::value("green", color::green));
    rttr::registration::class_<point2d>("point2d").constructor()(rttr::policy::ctor::as_object).property("x", &point2d::x).property("y", &point2d::y);

    rttr::registration::class_<shape>("shape")
        .property("visible", &shape::get_visible, &shape::set_visible)
        .property("color", &shape::color_)
        .property("name", &shape::name)
        .property("position", &shape::position)
        .property("dictionary", &shape::dictionary);

    rttr::registration::class_<circle>("circle")
        .property("radius", &circle::radius)
        .property("points", &circle::points)
        .property("no_serialize", &circle::no_serialize)(rttr::metadata("NO_SERIALIZE", true));

    rttr::registration::property("g_values", &g_values);
}

// TEST(ReflectionTestCase, reflectEnable)
// {
//     circle c_1("Circle #1");
//     c_1.set_visible(true);
//     c_1.points = std::vector<point2d>(2, point2d(1, 1));
//     c_1.points[1].x = 23;
//     c_1.points[1].y = 42;
//     c_1.position.x = 12;
//     c_1.position.y = 66;
//     c_1.radius = 5.123;
//     c_1.color_ = color::red;
//     c_1.dictionary = {{{color::green, {1, 2}}, {color::blue, {3, 4}}, {color::red, {5, 6}}}};
//     c_1.no_serialize = 12345;
//     NS::tstring json_string = NS::JsonSerializer::toJson(c_1); // serialize the circle to 'json_string'
//
//     circle c_2("Circle #2");                        // create a new empty circle
//     NS::JsonSerializer::fromJson(json_string, c_2); // deserialize it with the content of 'c_1'
//
//     ASSERT_EQ(c_1.radius, c_2.radius);
//     ASSERT_EQ(c_1.points.size(), c_2.points.size());
//     ASSERT_NE(c_1.no_serialize, c_2.no_serialize);
//     ASSERT_EQ(c_1.color_, c_2.color_);
// }

// TEST(ReflectionTestCase, reflectArray)
// {
//     g_values.push_back(new circle("name1"));
//     g_values.push_back(new circle("name2"));
//     g_values.push_back(new circle("name3"));
//     g_values.push_back(new circle("name4"));
// 
//     NS::tstring json_string = NS::JsonSerializer::toJsonGlobal(rttr::type::get_global_property("g_values"), g_values);
// 
//     g_values.clear();
//     NS::JsonSerializer::fromJsonGlobal(json_string, rttr::type::get_global_property("g_values"), g_values);
// 
//     ASSERT_EQ(g_values[2]->name, "name3");
// }
