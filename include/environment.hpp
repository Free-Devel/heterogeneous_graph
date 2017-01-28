//environment.hpp
#pragma once
#include <map>
#include <utility>
#include <mutex>
#include <future>
#include <boost/uuid/uuid.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include "resource.hpp"
#include "cpu_resource.hpp"
#include "variable.hpp"
#include "exception.hpp"
#include "types.hpp"
//#include "calculation_task.hpp"

namespace heterogeneous_graph {

// Forward declaration
class calculation_task;

class environment
{
   typedef std::map<boost::uuids::uuid, resource::pointer> resource_map_type;
   typedef std::map<std::string, variable> variable_map_type;
public:
   typedef std::shared_ptr<calculation_task> calculation_task_shared_ptr;

   environment();
   virtual ~environment();

   // resource section
   cpu_resource &get_main_resource();
   void new_resource(resource::pointer pr);
   resource &get_resource(const boost::uuids::uuid &uuid);

   // stuff section
   virtual void push(calculation_task_shared_ptr ct);

   /*template<typename Functor>
   Functor *stuff(const boost::uuids::uuid &uuid)
   {
      boost::container::flat_map<boost::uuids::uuid, calculation_task_shared_ptr>::const_iterator pos = _tasks.find(uuid);
      if(pos == _tasks.end())
         throw hg_exception("Task not found.");

      return pos->second->stuff<Functor>();
   }

   template<typename Functor>
   const Functor *stuff(const boost::uuids::uuid &uuid) const
   {
      boost::container::flat_map<boost::uuids::uuid, calculation_task_shared_ptr>::const_iterator pos = _tasks.find(uuid);
      if(pos == _tasks.end())
         throw hg_exception("Task not found.");

      return pos->second->stuff<Functor>();
   }*/

   // promise/future section
   boost::uuids::uuid new_promise_future();
   void set_promise(const data_type &data, const boost::uuids::uuid &uuid);
   void set_promise(const data_type &data, const boost::any &any);
   data_type get_future(const boost::uuids::uuid &uuid);
   data_type get_future(const boost::any &any);

   // variable section
   void declare(const std::string &name, const data_type &value);
   data_type read(const std::string &name) const;
   void write(const std::string &name, const data_type &value);

private:
   boost::container::flat_map<boost::uuids::uuid, calculation_task_shared_ptr> _tasks;

   //promise-future section
   typedef std::promise<data_type> promise_type;
   typedef std::shared_future<data_type> future_type;
   struct fp_pair{
      fp_pair() { _future = _promise.get_future(); }
      promise_type _promise;
      future_type _future;
   };
   typedef boost::ptr_map<boost::uuids::uuid, fp_pair> promise_future_map_type;
   promise_future_map_type _promise_future_map;
   mutable std::mutex _pf_mutex;

   resource_map_type _resources;
   cpu_resource cpu_res_;

   variable_map_type _variables;
};

} // namespace heterogeneous_graph
