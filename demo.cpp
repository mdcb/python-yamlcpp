/*
 *    Copyright (C) 2012  Matthieu Bec, GMTO Corp.
 *
 *    This file is part of gmt-fwk-io.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <boost/python.hpp>
#include <yaml-cpp/yaml.h>

using namespace boost::python;


class demo
{

  public:

  void set ( YAML::Node node )
  {
    std::cerr << YAML::Dump(node) << std::endl;
  }

  YAML::Node get (void)
  {
    YAML::Node ret;
    ret["int"]=1;
    ret["float"]=1.1;
    ret["string"]="one";
    ret["list"][0]=2;
    ret["list"][1]=2.2;
    ret["list"][2]="two";
    ret["dict"]["a"]=3;
    ret["list"]["b"]=3.3;
    ret["list"]["c"]="three";
    return ret;
  }

};


//-----------------------------------------------------
// module definition
//-----------------------------------------------------


BOOST_PYTHON_MODULE(boostyamlcpp_demo)
{
  PyEval_InitThreads();

  std::cerr << "demo initialized\n";

  class_<demo, boost::noncopyable>("demo", "demo doc string", init<>())
  .def("set", &demo::set)
  .def("get", &demo::get)
  ;

}

