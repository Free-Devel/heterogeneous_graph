//main.cpp
#include <iostream>
#include <boost/make_shared.hpp>
#include <boost/timer/timer.hpp>
#include "environment.hpp"
#include "calculation_graph.hpp"
#include "utility_stuff.hpp"
#include "types.hpp"
#include "execution_functions.hpp"
#include "cpu_resource.hpp"

namespace hg = heterogeneous_graph;
namespace vk = hg::vertex_kind;

const size_t n_workers = boost::thread::hardware_concurrency();

struct info_job
{
   double start;
   double end;
   unsigned int n;

   template <typename Archive>
   inline void serialize(Archive &ar, const unsigned int version)
   {
      ar & boost::serialization::make_nvp("start", start);
      ar & boost::serialization::make_nvp("end", end);
      ar & boost::serialization::make_nvp("n", n);
   }
};

hg::data_vector sin_integrator_start(const hg::data_vector &data, hg::environment &)
{
   return data;
}

struct sin_integrator
{
   sin_integrator(size_t id, size_t tot)
   {
      _id = id;
      _tot = tot;
   }

   hg::data_vector operator()(const hg::data_vector &data, hg::environment &)
   {
      info_job ij_in = hg::unmarshall<info_job>(data);

      double delta = (ij_in.end - ij_in.start) / _tot;
      double start = ij_in.start + (_id * delta);
      double end = start + delta;
      unsigned int n = ij_in.n / _tot;

      double t = (end - start) / n;
      double integration = 0.0;
      for(unsigned int i = 0; i < n; ++i)
         integration += sin(start + (t * i)) * t;

      return hg::marshall(integration);
   }

private:
   size_t _id;
   size_t _tot;
};

hg::data_vector sin_integrator_end(const hg::data_vector &data, hg::environment &)
{
   std::vector<double> ijv_in = hg::unmarshall_vector<double>(data);

   double integration = 0;
   for(size_t i = 0; i < ijv_in.size(); ++i)
      integration += ijv_in[i];

   return hg::marshall(integration);
}

using boost::timer::cpu_timer;

int main()
{
   typedef std::pair<size_t, size_t> link_type;

   cpu_timer tmrP;
   cpu_timer tmrA;

   tmrA.start();

   // Creazione dell'ambiente di esecuzione.
   hg::environment env;

   // Costruzione del grafo di calcolo.

   // Creazione prima dei collegamenti.
   std::vector<link_type> edges(n_workers * 2);
   for(size_t i = 0; i < n_workers; ++i)
      edges[i] = std::make_pair(0, i + 1);
   for(size_t i = 0; i < n_workers; ++i)
      edges[i + n_workers] = std::make_pair(i + 1, n_workers + 1);

   hg::calculation_graph cg(edges.begin(), edges.end(), n_workers + 2);

   hg::vertex_descriptor start_node = hg::begin(cg);
   hg::vertex_descriptor end_node = hg::last(cg);

   // Inserimento dei vari task sui nodi di calcolo.
   hg::init_node<vk::synchronous>(
      start_node, 
      std::move(&sin_integrator_start), 
      cg, 
      env,
      vk::broadcast
   );

   for(size_t i = 0; i < n_workers; ++i) {
      hg::init_node<vk::synchronous>(
         start_node + i + 1,
         std::move(sin_integrator(i, n_workers)),
         cg,
         env
      );
   }

   hg::init_node<vk::synchronous>(
      end_node, 
      std::move(&sin_integrator_end), 
      cg,
      env
   );

   // Definizione dell'input del grafo.
   info_job ij;
   ij.start = 0.0;
   ij.end = 3.1415;
   ij.n = 1000000000;

   tmrP.start();
   // Esecuzione del grafo di calcolo.
   double result = hg::run<info_job, double>(ij, cg, env);
   tmrP.stop();
   tmrA.stop();

   std::cout << "result: " << result << " in elapsed time: " << tmrP.format(3LL, "%wsec.\n");
   std::cout << "Global elapsed time: " << tmrA.format(3LL, "%wsec.\n") << std::endl;

   return 0;
}
