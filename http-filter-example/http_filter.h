#include <string>

#include "server/config/network/http_connection_manager.h"

namespace Envoy {
namespace Http {

const LowerCaseString headerKey = LowerCaseString("via");
const std::string headerValue = "sample-filter";

class HttpSampleDecoderFilter : public Http::StreamDecoderFilter {
public:
  HttpSampleDecoderFilter();
  ~HttpSampleDecoderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(HeaderMap& headers, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  FilterTrailersStatus decodeTrailers(HeaderMap&) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) override;

private:
  StreamDecoderFilterCallbacks* decoder_callbacks_;
};

} // Http
} // Envoy