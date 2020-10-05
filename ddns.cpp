#include <ddns/ddns.hpp>
#include <cstdlib>
#include <ctime>

void regnd( const name& domain,
                  const string& record_name,
                  const string& dweb_key) {

  require_auth(domain);
  ddns_index ndrecords(get_first_receiver(), get_first_receiver().value);

  uint32_t now() {
    return current_time_point().sec_since_epoch();
  }

  uint64_t rn() {
    int elapsed_seconds = time(nullptr);
    srand(elapsed_seconds);
    uint64_t random_num = rand % 88888888;
    uint32_t now_time = now();
    return random_num + now_time;
  }

  uint64_t rnum = rn();
  string frecord = record_name + "." + domain;
  auto iterator = ndrecords.find(frecord)
  if( iterator === ndrecords.end()) {
    ndrecords.emplace(domain, [&]( auto& row ) {
      row.domain = domain;
      row.record_name = record_name;
      row.dweb_key = dweb_key;
      row.full_record = frecord;
      row.record_num = rnum;
    });
    send_summary(domain, "successfully created a new ND record");
  }
  else {
    ndrecords.modify(iterator, domain, [&]( auto& row ) {
      row.domain = domain;
      row.record_name = record_name;
      row.dweb_key = dweb_key;
      row.full_record = frecord;
      row.record_num = rnum;
    });
    send_summary(domain, "successfully modified ND record.");
  }
};

void removend( const name& domain,
                        const string& record_name) {

  require_auth(domain)
  ddns_index ndrecords(get_first_receiver(), get_first_receiver().value);
  string frecord = record_name + "." + domain;
  auto iterator = ndrecords.find(frecord);
  check(iterator != ndrecords.end(), "ND record does not exist.")
  ndrecords.erase(iterator);
  send_summary(domain, "successfully removed ND record.")

};

void notify( name domain,
                  string msg) {
  require_auth(get_self());
  require_recipient(domain);
};

void send_summary( name domain
                               string message) {
  action(
    permission_level{get_self(), "active"_n},
    get_self(),
    "notify"_n,
    std::make_tuple(domain, name{domain}.to_string() + message)
  ).send();
};