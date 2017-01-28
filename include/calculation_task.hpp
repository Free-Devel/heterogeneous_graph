//calculation_task.hpp
#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <utility>
#include <condition_variable>
#include <vector>
#include <boost/config.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include "types.hpp"
#include "environment.hpp"
#include "calculation_graph.hpp"
#include "counter.hpp"
#include "cpu_resource.hpp"

namespace heterogeneous_graph {

class resource;
class environment;

class calculation_task
{
protected:
   struct stuff_base
   {
      virtual data_vector run(const data_vector &, environment &) = 0;
      virtual ~stuff_base() {}
   };

   typedef std::unique_ptr<stuff_base> stuff_base_type;
   stuff_base_type stuff_;

   template<typename F>
   struct stuff_type: public stuff_base
   {
      stuff_type(F && fun): fun_(std::move(fun)) {}
      data_vector run(const data_vector &data, environment &env) { return fun_(data, env); }

      F fun_;
   };

public:
   typedef calculation_task self_type;
   typedef self_type *ptr;
   typedef std::shared_ptr<self_type> shared_ptr;
   typedef std::vector<counter::shared_ptr> counter_vector_type;

   calculation_task(
      environment &env
   );

   template <typename F>
   calculation_task(
      F &&stuff,
      environment &env
   ): calculation_task(env)
   {
      stuff_.reset(new stuff_type<F>(std::move(stuff)));
   }

   template <typename F>
   calculation_task(
      boost::uuids::uuid uuid,
      F &&stuff,
      environment &env
   ): calculation_task(env)
   {
      stuff_.reset(new stuff_type<F>(std::move(stuff)));
   }

   virtual ~calculation_task();

   virtual void init() = 0;

   boost::uuids::uuid uuid() const;
   virtual void run() = 0;

   void set_input(const data_vector &input);
   data_vector get_output() const;

   void move_to_ready() const;

   /*template<typename Functor>
   Functor *stuff() { return stuff.target<Functor>(); }

   template<typename Functor>
   const Functor *stuff() const { return _stuff.target<Functor>(); }*/

protected:
   //stuff_type _stuff;
   resource &resource_;
   environment &environment_;
   boost::uuids::uuid uuid_;

   bool end_;
   mutable std::mutex end_mtx_;
   mutable std::condition_variable cond_;
   data_vector in_out_;
};

} // namespace heterogeneous_graph
