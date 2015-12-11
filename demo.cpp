#include <iostream>
#include <boost/python.hpp>
#include <yaml-cpp/yaml.h>

using namespace boost::python;

class demo
{

public:

  YAML::Node ret;

  demo()
  {
    ret["int"] = 1;
    ret["float"] = 1.1;
    ret["string"] = "one";
    ret["list"][0] = 2;
    ret["list"][1] = 2.2;
    ret["list"][2] = "two";
    ret["dict"]["a"] = 3;
    ret["dict"]["b"] = 3.3;
    ret["dict"]["c"] = "three";
  }

  void set(const YAML::Node & node)
  {
    std::cerr << YAML::Dump(node) << std::endl;
    ret = node;
  }

  YAML::Node get(void)
  {
    return ret;
  }

  YAML::Node & getref(void)
  {
    return ret;
  }

};


//-----------------------------------------------------
// module definition
//-----------------------------------------------------


BOOST_PYTHON_MODULE(yamlcpp_demo)
{
  PyEval_InitThreads();

  std::cerr << "yamlcpp demo initialized\n";

  class_<demo, boost::noncopyable>("demo", "demo doc string", init<>())
  .def("set", &demo::set)
  .def("get", &demo::get)
  ;

}

