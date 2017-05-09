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

using boost::timer::cpu_timer;

const size_t n_workers = boost::thread::hardware_concurrency();

struct info_job
{
   double start;
   double end;
   unsigned int n;
};

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
      [](const hg::data_vector &data, hg::environment &) { return data; }, 
      cg, 
      env,
      vk::broadcast
   );

   for(size_t i = 0; i < n_workers; ++i) {
      hg::init_node<vk::synchronous>(
         start_node + i + 1,
         [i](const hg::data_vector &data, hg::environment &)
         {
            info_job ij_in = hg::unmarshall<info_job>(data);

            double delta = (ij_in.end - ij_in.start) / n_workers;
            double start = ij_in.start + (i * delta);
            double end = start + delta;
            size_t n = ij_in.n / n_workers;

            double t = (end - start) / n;
            double integration = 0.0;
            for(size_t x = 0; x < n; ++x)
               integration += sin(start + (t * x)) * t;

            return hg::marshall(integration);
         },
         cg,
         env
      );
   }

   hg::init_node<vk::synchronous>(
      end_node, 
      [](const hg::data_vector &data, hg::environment &)
      {
         std::vector<double> ijv_in = hg::unmarshall_vector<double>(data);

         double integration = 0;
         for(size_t i = 0; i < ijv_in.size(); ++i)
            integration += ijv_in[i];

         return hg::marshall(integration);
      }, 
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
