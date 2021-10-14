#include <string>
#include <iostream>

#include "http_filter.h"
#include "envoy/server/filter_config.h"
#include "shared_data.h"

namespace Envoy {
namespace Http {

HttpSampleDecoderFilterConfig::HttpSampleDecoderFilterConfig(
    const sample::Decoder& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

HttpSampleDecoderFilter::HttpSampleDecoderFilter(HttpSampleDecoderFilterConfigSharedPtr config)
    : config_(config), cluster_header_("") {}

HttpSampleDecoderFilter::~HttpSampleDecoderFilter() {}

void HttpSampleDecoderFilter::onDestroy() {}

const LowerCaseString HttpSampleDecoderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string HttpSampleDecoderFilter::headerValue() const {
  return config_->val();
}

const std::string HttpSampleDecoderFilter::readClusterHeader() const {

  std::cout << "attaching to shared memory" << std::endl;
  if(shm_allocate(SHM_ATTACH) == 0) {
    cout << "could not attach to shared memory" << std::endl;
    return DEFAULT_ROUTE_DESTINATION.data();
  }

  std::cout << "attached to shared memory" << std::endl;
  print_shm_info();

  if (shared_data->signal == 0) {
    std::cout << "no data avialble in shared memory" << std::endl;
    return DEFAULT_ROUTE_DESTINATION.data();
  }

  const std::string clusterHeader{shared_data->str};
  std::cout << "read cluster header from shared memory, cluster_header: " << clusterHeader << std::endl;
  shared_data->signal = 0;

  if (shm_release()) {
    std::cerr << "error deattachnig the shared memory, terminating" << std::endl;
  }

  return clusterHeader ;
}

FilterHeadersStatus HttpSampleDecoderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {
  std::cout << "BEGIN: decodeHeaders" << std::endl;

  // read header from shared memory and store in `cluster_header_`
  cluster_header_ = readClusterHeader();
  std::cout << "cluster_header: " << cluster_header_ << std::endl;
  headers.addCopy(LowerCaseString("cluster_header"), LowerCaseString(cluster_header_));

  // add a header
  headers.addCopy(headerKey(), headerValue());
  // ENVOY_LOG(trace, "key: {}, value: {}", config_->key(), config_->val());
  std::cout <<"key: " << config_->key() << "value: " << config_->val() << std::endl;

  return FilterHeadersStatus::Continue;
}

FilterDataStatus HttpSampleDecoderFilter::decodeData(Buffer::Instance& data, bool) {
  // ENVOY_LOG(trace, "request body: {}", data.toString());
  std::cout << "BEGIN: decodeData" << std::endl;
  std::cout << "request body: " << data.toString() << std::endl;
  return FilterDataStatus::Continue;
}

void HttpSampleDecoderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
