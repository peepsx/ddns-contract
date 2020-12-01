#include "ddns.hpp"

void add( const name&  domain,
               const name&  rname,
               const uint64_t&  ttl,
               const string&  rtype,
               const string&  rclass,
               const string& rdata) {

  require_auth(domain);
  record_index ddnsrecords(get_first_receiver(), get_first_receiver().value);
  auto iterator = ddnsrecords.find(rname.value);
  if( iterator == ddnsrecords.end() )
  {
    ddnsrecords.emplace(rname, [&]( auto& row ) {
      row.domain = domain;
      row.rname = rname;
      row.ttl = ttl;
      row.rtype = rtype;
      row.rclass = rclass;
      row.rdata = rdata;      
    });
    // send_summary(domain, " successfully added record");
  }
  else {
    ddnsrecords.modify(iterator, rname, [&]( auto& row ) {
      row.domain = domain;
      row.rname = rname;
      row.ttl = ttl;
      row.rtype = rtype;
      row.rclass = rclass;
      row.rdata = rdata;
    });
   // send_summary(domain, " successfully modified record");
  }
};

void erase(  const name&  domain,
                   const name&  rname) {
  require_auth(domain);
  record_index ddnsrecords(get_first_receiver(), get_first_receiver().value);
  auto iterator = ddnsrecords.find(rname.value);
  check(iterator != ddnsrecords.end(), "Record does not exist");
  ddnsrecords.erase(iterator);
 // send_summary(domain, " successfully erased dDNS record");
};

void notify(  const name&  domain,
                   const string&  msg) {
  require_auth(get_self());
  require_recipient(domain);
};

// void send_summary(  const name&  domain,
//                                  const string&  message) {
//   action(
//     permission_level{get_self(), "active"_n},
//     get_self(),
//     "notify"_n,
//     std::make_tuple(domain, name{domain}.to_string() + message)
//   ).send();  
// };