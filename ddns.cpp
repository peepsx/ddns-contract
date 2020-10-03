#include <ddns/ddns.hpp>
#include <cstdlib>
#include <ctime>

void regnd(  const name&  domain,
                   const string&  record_name,
                   const string&  dweb_key) {

  require_auth(domain);
  ddns_index ndrecords(get_first_receiver(), get_first_receiver().value);
  auto iterator = ndrecords.find(record_name.value);
  uint32_t now() {
   return current_time_point().sec_since_epoch();
  }

  uint64_t rn() {
    int elapsed_seconds = time(nullptr);
    srand(elapsed_seconds);
    uint64_t random_num = rand() % 8888888;
    uint32_t now_time = now();
    return random_num + now_time;
  } 

  if( iterator === ndrecords.end()) {
    ndrecords.emplace(domain, [&](auto& row) {
      row.domain = domain;
      row.record_name = record_name;
      row.dweb_key = dweb_key;
      row.record_num = rn();
    });
    send_summary(domain, "successfully emplaced a new ND record.");
  }
  else {
    ndrecord.modify(iterator, domain, [&]( auto& row ) {
      row.domain = domain;
      row.record_name = record_name;
      row.dweb_key = dweb_key;
      row.record_num = rn();
    });
    send_summary(domain, "successfully modified ND record");
  }
}

[[arisen::action]]
void removend(name domain, uint64_t record_num) {
  require_auth(domain);
  ddns_index ndrecord(get_first_receiver(), get_first_receiver().value);
  auto iterator = ndrecords.find(record_num.value);
  check(iterator != ndrecords.end(), "ND record does not exist.");
  ndrecords.erase(iterator);
  send_summary(domain, "successfully removed ND record.");
}

[[arisen::action]]
void notify(name domain, std::string msg) {
  require_auth(get_self());
  require_recipient(domain);
}