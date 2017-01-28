//compression_algorithms.cpp
#include "compression_algorithms.hpp"

#if SERIALIZE_KIND == 0

#include <vector>
#include <boost/lexical_cast.hpp>
#include "exception.hpp"
#include "base64.hpp"
#include "lzo/lzoconf.h"
#include "lzo/lzo1x.h"
#include "lzo/lzo1z.h"
#include "lzo/lzo2a.h"

namespace hg = heterogeneous_graph;
namespace hgb = hg::base;

std::string hgb::lzoCompressEncoded(const std::string &srcStr)
{
   lzo_bytep src = (lzo_bytep)(&srcStr[0]);
   lzo_uint src_len = srcStr.size();

   std::vector<lzo_byte> destVec(srcStr.size());
   lzo_bytep dst = &destVec[0];
   lzo_uint dst_len = srcStr.size();

   std::vector<lzo_byte> wrkmemVec(LZO2A_999_MEM_COMPRESS);
   lzo_voidp wrkmem = &wrkmemVec[0];

   int res = lzo2a_999_compress(src, src_len, dst, &dst_len, wrkmem);
   if(res != LZO_E_OK)
      throw hg::hg_exception("Error lzo compress (" + boost::lexical_cast<std::string>(res) + ").");

   std::string dest = base64_encode(dst, dst_len);
   return dest;
}

std::string hgb::lzoDecompressDecoded(const std::string &srcStr, unsigned int factor)
{
   std::vector<unsigned char> decoded = base64_decode_vector(srcStr);

   lzo_bytep src = &decoded[0];
   lzo_uint src_len = decoded.size();

   std::vector<lzo_byte> destVec(srcStr.size() * factor);
   lzo_bytep dst = &destVec[0];
   lzo_uint dst_len = destVec.size();
   lzo_voidp wrkmem = 0;

   int res = lzo2a_decompress(src, src_len, dst, &dst_len, wrkmem);
   if(res != LZO_E_OK)
      throw hg::hg_exception("Error lzo decompress (" + boost::lexical_cast<std::string>(res) + ").");

   std::string dest(reinterpret_cast<const char *>(dst), dst_len);
   return dest;
}

#endif // SERIALIZE_KIND
