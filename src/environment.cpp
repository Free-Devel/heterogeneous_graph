//environment.cpp
#ifdef WIN32
#include "windows.h"
#endif // WIN32
#include <mutex>
#include <algorithm>
#include <stdexcept>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "environment.hpp"
#include "cpu_resource.hpp"
#include "calculation_task.hpp"

namespace heterogeneous_graph {

environment::environment(
)
{
   new_resource(&cpu_res_);
#ifdef NDEBUG
#ifdef WIN32 
   if(!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
      throw hg_exception("Could not set the priority of current process");
#endif // WIN32
#endif // NDEBUG
}

environment::~environment()
{
   std::for_each(_resources.begin(), _resources.end(), [](auto resource) { resource.second->stop(); } );
}

void environment::new_resource(resource::pointer pr)
{
   _resources.insert(std::make_pair(pr->uuid(), pr));
}

cpu_resource &environment::get_main_resource()
{
   return cpu_res_;
}

resource &environment::get_resource(const boost::uuids::uuid &uuid)
{
   return *_resources[uuid];
}

boost::uuids::uuid environment::new_promise_future()
{
   boost::uuids::uuid uuid = boost::uuids::random_generator()();
   {
      std::unique_lock<std::mutex> lock(_pf_mutex);
      _promise_future_map.insert(uuid, new fp_pair);
   }

   return uuid;
}

void environment::set_promise(const data_type &data, const boost::uuids::uuid &uuid)
{
   promise_future_map_type::iterator pos;
   {
      std::unique_lock<std::mutex> lock(_pf_mutex);
      pos = _promise_future_map.find(uuid);
   }
   if(pos == _promise_future_map.end())
      throw std::runtime_error("Error: Promise not found.");

   pos->second->_promise.set_value(data);
}

void environment::set_promise(const data_type &data, const boost::any &any)
{
   set_promise(data, boost::any_cast<boost::uuids::uuid>(any));
}

data_type environment::get_future(const boost::uuids::uuid &uuid)
{
   promise_future_map_type::iterator pos;
   {
      std::unique_lock<std::mutex> lock(_pf_mutex);
      pos = _promise_future_map.find(uuid);
   }
   if(pos == _promise_future_map.end())
      throw std::runtime_error("Error: Future not found.");

   future_type &fp = pos->second->_future;

   fp.wait();
   return fp.get();
}

data_type environment::get_future(const boost::any &any)
{
   return get_future(boost::any_cast<boost::uuids::uuid>(any));
}

void environment::declare(const std::string &name, const data_type &value)
{
   variable_map_type::const_iterator pos = _variables.find(name);
   if(pos != _variables.end())
      throw;

   _variables.insert(std::make_pair(name, variable(value)));
}

data_type environment::read(const std::string &name) const
{
   variable_map_type::const_iterator pos = _variables.find(name);
   if(pos == _variables.end())
      throw;
      
   return pos->second.read();
}

void environment::write(const std::string &name, const data_type &value)
{
   variable_map_type::const_iterator pos = _variables.find(name);
   if(pos == _variables.end())
      throw;

   _variables[name].write(value);   
}

void environment::push(calculation_task::shared_ptr ct)
{
   _tasks.insert(std::make_pair(ct->uuid(), ct));
}

} // namespace heterogeneous_graph 
