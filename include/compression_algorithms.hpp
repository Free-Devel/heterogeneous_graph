//compression_algorithms.hpp
#pragma once
#define SERIALIZE_KIND 1

#include <sstream>
#include <boost/config.hpp>
#include <boost/serialization/string.hpp>
#if SERIALIZE_KIND == 0 || SERIALIZE_KIND == 1
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#else
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#endif
//#include "config.hpp"

namespace heterogeneous_graph { namespace base {

#if SERIALIZE_KIND == 0 // compressed xml

std::string lzoCompressEncoded(const std::string &srcStr);
std::string lzoDecompressDecoded(const std::string &srcStr, unsigned int factor = 16);

template <typename T>
std::string serialize_compress(const T &elem)
{
   std::stringstream ss;
   boost::archive::binary_oarchive oa(ss);
   oa << boost::serialization::make_nvp("hg_elem", elem);

   return lzoCompressEncoded(ss.str());
}

template <typename T>
T unserialize_decompress(const std::string &elem)
{
   T out;

   std::stringstream ss;
   ss << lzoDecompressDecoded(elem);
   boost::archive::binary_iarchive ia(ss);
   ia >> boost::serialization::make_nvp("hg_elem", out);

   return out;
}

#elif SERIALIZE_KIND == 1 // xml

template <typename T>
std::string serialize_compress(const T &elem)
{
   std::stringstream ss;
   boost::archive::xml_oarchive oa(ss);
   oa << boost::serialization::make_nvp("hg_elem", elem);

   return ss.str();
}

template <typename T>
T unserialize_decompress(const std::string &elem)
{
   T out;

   std::stringstream ss;
   ss << elem;
   boost::archive::xml_iarchive ia(ss);
   ia >> boost::serialization::make_nvp("hg_elem", out);

   return out;
}

#elif SERIALIZE_KIND == 2 // binary

template <typename T>
std::string serialize_compress(const T &elem)
{
   std::stringstream ss;
   boost::archive::binary_oarchive oa(ss);
   oa << boost::serialization::make_nvp("hg_elem", elem);

   return ss.str();
}

template <typename T>
T unserialize_decompress(const std::string &elem)
{
   T out;

   std::stringstream ss;
   ss << elem;
   boost::archive::binary_iarchive ia(ss);
   ia >> boost::serialization::make_nvp("hg_elem", out);

   return out;
}

#endif // SERIALIZE_KIND

} } // namespace base heterogeneous_graph

