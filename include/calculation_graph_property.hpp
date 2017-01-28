//calculation_graph_property.hpp
#pragma once
#include <boost/config.hpp>
#include <boost/graph/properties.hpp>

// create a tag for our new property
namespace boost {
   enum vertex_stuff_t            { vertex_stuff };
   enum vertex_stuff_uuid_t       { vertex_stuff_uuid };
   enum vertex_calculation_task_t { vertex_calculation_task };
   enum vertex_counter_t          { vertex_counter };
   enum vertex_kind_t             { vertex_kind };
   enum vertex_scope_t            { vertex_scope };
   enum vertex_resource_t         { vertex_resource };

   enum edge_data_t { edge_data };

   BOOST_INSTALL_PROPERTY(vertex, stuff);
   BOOST_INSTALL_PROPERTY(vertex, stuff_uuid);
   BOOST_INSTALL_PROPERTY(vertex, calculation_task);
   BOOST_INSTALL_PROPERTY(vertex, counter);
   BOOST_INSTALL_PROPERTY(vertex, kind);
   BOOST_INSTALL_PROPERTY(vertex, scope);
   BOOST_INSTALL_PROPERTY(vertex, resource);
   
   BOOST_INSTALL_PROPERTY(edge, data);
}   
