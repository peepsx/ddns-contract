#include <arisen/arisen.hpp>
#include <arisen/print.hpp>

using namespace arisen;
using std::string;

/**
  * @defgroup ddns
  * @ingroup ddnscontracts
  * 
  * dDNS Contract
  * 
  * @details ddns contract defines the structures and actions that allow users to
  * add, modify or delete records for a decentralized domain.
  * @{
  */
class [[arisen::contract("ddns")]] ddns : public arisen::contract {
  public:
    ddns(name receiver, name code, datastream<const char*> ds): contract(receiver, code, ds) {}
    /**
      * Add action
      *
      * @details Allows `domain` to add a dDNS record.
      * @param domain - The domain the record is being created for.
      * @param rname - The record name
      * @param ttl - The time-to-live (TTL) for the record
      * @param rtype - The record type
      * @param rclass - The record class
      * @param rdata - The record data
      *
      * @pre Record name must not exist with same type, for the given domain
      * @pre The record type must be valid
      * @pre The record class must be valid
      * If validation is successful, a new dDNS record will be created for the domain.
      */
    [[arisen::action]]
    void add( const name& domain,
                   const name& rname,
                   const uint64_t& ttl,
                   const string& rtype,
                   const string& rclass,
                   const string& rdata);

    /**
      * Erase Action
      *
      * @details Erase an already existent record from `domain`
      * 
      * @param domain - The domain the record is being removed from
      * @param rname - The record being removed
      *
      * @pre Record must exist
      * If validation is successful, the record for the given domain will be erased.
    */
    [[arisen::action]]
    void erase( const name& domain,
                      const name& rname);

    [[arisen::action]]
    void notify( const name& domain,
                      const string& msg);

  private:
    struct [[arisen::table]] record {
      name domain;
      name rname;
      uint64_t ttl;
      string rtype;
      string rclass;
      string rdata;

      uint64_t primary_key() const {  return rname.value; }
      uint64_t get_secondary_1() const { return domain.value; }
    };

    void send_summary(  const name& domain,
                                     const string& message);

    typedef arisen::multi_index<"records"_n, record,
      indexed_by<"byrecord"_n, const_mem_fun<record, uint64_t, &record::get_secondary_1>>
    > record_index;

/** @}*/ // end of @defgroup ddns
};