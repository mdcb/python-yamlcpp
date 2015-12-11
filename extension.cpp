/*
 *    Copyright (C) 2012  Matthieu Bec, mdcb808@gmail.com
 *
 *    This file is part of python-yamlcpp.
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

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <exception>
#include <yaml-cpp/yaml.h>
#include <iostream>

using namespace boost::python;

//-----------------------------------------------------
// YAML::Node to python converter
//-----------------------------------------------------

struct yamlnode_topython_converter
{
  static PyObject * convert(YAML::Node const & node)
  {
    switch (node.Type())
      {

      case YAML::NodeType::Null:
      {
        //std::cerr << "DEBUG converting NodeType::Null\n";
        return incref(Py_None);
      }

      case YAML::NodeType::Scalar:
      {
        object result;

        //std::cerr << "DEBUG converting NodeType::Scalar\n";
        try
          {
            result = object(node.as<int64_t>());
            //std::cerr << "DEBUG converting NodeType::Scalar int64_t\n";
          }

        catch (const YAML::BadConversion & e1)
          {
            try
              {
                result = object(node.as<double>());
                //std::cerr << "DEBUG converting NodeType::Scalar double\n";
              }

            catch (const YAML::BadConversion & e2)
              {
                try
                  {
                    result = object(node.as<bool>());
                    //std::cerr << "DEBUG converting NodeType::Scalar bool\n";
                  }

                catch (const YAML::BadConversion & e3)
                  {
                    try
                      {
                        result = object(node.as<std::string>());
                        //std::cerr << "DEBUG converting NodeType::Scalar string\n";
                      }

                    catch (const YAML::BadConversion & e3)
                      {
                        throw std::runtime_error("unable to convert yaml node scalar");
                      }
                  }
              }
          }

        return incref(result.ptr());
      }

      case YAML::NodeType::Sequence:
      {
        list result;

        //std::cerr << "DEBUG converting NodeType::Sequence\n";
        for (auto it = node.begin(); it != node.end(); ++it) // XXX auto
          {
            PyObject * obj = /*yamlnode_topython_converter::*/convert(*it);
            object pyobj = object(handle<>(obj));
            result.append(pyobj);
          }

        return incref(result.ptr());
      }

      case YAML::NodeType::Map:
      {
        dict result;

        //std::cerr << "DEBUG converting NodeType::Map\n";
        for (auto it = node.begin(); it != node.end(); ++it)
          {
            PyObject * kobj = /*yamlnode_topython_converter::*/convert(it->first);
            object pykobj = object(handle<>(kobj));
            PyObject * vobj = /*yamlnode_topython_converter::*/convert(it->second);
            object pyvobj = object(handle<>(vobj));
            result[pykobj] = pyvobj;
          }

        return incref(result.ptr());
      }

      case YAML::NodeType::Undefined:
      default:
      {
        throw std::runtime_error("undefined yaml node");
      }
      }
  }
};


struct yamlnode_frompython_converter
{
  yamlnode_frompython_converter()
  {
    converter::registry::push_back(
      &convertible,
      &construct,
      type_id<YAML::Node>());
  }

  static void * convertible(PyObject * obj_ptr)
  {
    return obj_ptr; // TODO, assume True.
  }


  static void yp_convert(PyObject * obj_ptr, YAML::Node & node)
  {
    if (PyBytes_Check(obj_ptr)) // NB: strings are not bytes
      {
        //std::cerr << "DEBUG converting bytes\n";
        node = std::string(PyBytes_AsString(obj_ptr));
      }

    else if (PyUnicode_Check(obj_ptr))
      {
        //std::cerr << "DEBUG converting unicode\n";
        node = std::string(PyBytes_AsString(PyUnicode_AsASCIIString(obj_ptr)));
      }

    else if (PySequence_Check(obj_ptr)) // test before mapping because list have mapping in python3
      {
        //std::cerr << "DEBUG converting sequence\n";
        node = YAML::Node(YAML::NodeType::Sequence);

        for (Py_ssize_t i = 0; i < PySequence_Length(obj_ptr); i++)
          {
            PyObject * elt = PySequence_GetItem(obj_ptr, i);
            YAML::Node elty;
            yp_convert(elt, elty);
            node.push_back(elty);
          }
      }

    else if (PyMapping_Check(obj_ptr))
      {
        //std::cerr << "DEBUG converting map\n";
        node = YAML::Node(YAML::NodeType::Map);

        PyObject * keyvals = PyMapping_Items(obj_ptr);

        for (Py_ssize_t i = 0; i < PyMapping_Length(keyvals); i++)
          {
            PyObject * kv = PyList_GetItem(keyvals, i);
            PyObject * k = PyTuple_GetItem(kv, 0);
            PyObject * v = PyTuple_GetItem(kv, 1);
            YAML::Node * peltk = new YAML::Node(); // refcount
            YAML::Node eltv;
            yp_convert(k, *peltk);
            yp_convert(v, eltv);
            node[*peltk] = eltv;
          }



      }

    else if (PyBool_Check(obj_ptr))
      {
        //std::cerr << "DEBUG converting bool\n";
        node = static_cast<bool>(PyLong_AsLong(obj_ptr));
      }

    else if (PyLong_Check(obj_ptr))
      {
        //std::cerr << "DEBUG converting long\n";
        node = PyLong_AsLong(obj_ptr);
      }

    else if (PyFloat_Check(obj_ptr))
      {
        //std::cerr << "DEBUG converting float\n";
        node = PyFloat_AsDouble(obj_ptr);
      }

    else
      {
        //std::cerr << "DEBUG conversion not supported\n";
        throw std::runtime_error("invalid type for conversion.");
      }
  }

  // Convert obj_ptr into a YAML::Node
  static void construct(
    PyObject * obj_ptr,
    converter::rvalue_from_python_stage1_data * data)
  {
    YAML::Node node;

    yp_convert(obj_ptr, node);

    // Grab pointer to memory into which to construct the new YAML::Node
    void * storage = ((converter::rvalue_from_python_storage<YAML::Node> *) data)->storage.bytes;

    // in-place construct the new YAML::Node using the character data
    // extracted from the python object
    new(storage) YAML::Node(node);

    // Stash the memory chunk pointer for later use by boost.python
    data->convertible = storage;
  }
};

//-----------------------------------------------------
// module definition
//-----------------------------------------------------


BOOST_PYTHON_MODULE(yamlcpp)
{
  to_python_converter<YAML::Node, yamlnode_topython_converter>();
  yamlnode_frompython_converter();
}


