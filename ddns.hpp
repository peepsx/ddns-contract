#pragma once
#include <arisen/arisen.hpp>
#include <arisen/print.hpp>
#include <string>

/**
 * @defgroup ddns dweb
 * @ingroup dwebcontracts
 * 
 * ddns contract
 * 
 * @details The dDNS contract defines the structures and actions that allow users to create, modify
 * and remove a NameDrive (ND) record for a dTLD.
 * @{
 */
class [[arisen::contract("ddns")]] ddns : public arisen::contract {

  public: 
    ddns(name receiver, name code, datastream<const char*> ds): contract(receiver, code, ds) {}

    /**
     * Regnd Action.
     * 
     * @details Allows `domain` owner to create or modify a `record_name` for a 
     * NameDrive (ND) and its corresponding `dweb_key`.
     * @param domain - the domain the ND record is associated with.
     * @param record_name - the name of the ND record ie. `nd1`.
     * @param dweb_key - the ND's dWeb discovery key.
     * @param record_num - (auto) primary key/indice for ND record.
     * @pre dWeb discovery key must be valid.
     * @pre `record_name` must be unique.
     *
     * If validation is successful a new entry in the ddns table for `domain` will be created.
     */
     [[arisen::action]]
     void regnd( const name& domain,
                       const string& record_name,
                       const string& dweb_key);
     
   /**
    *
    * RemoveND Action
    * @details This action removes a NameDrive record from the Root System.
    * @param `domain` - Domain associated with the record being removed.
    * @param `record_name` - ND record_name being removed.
    */
    [[arisen::action]]
    void removend( const name& domain,
                            const string& record_name);
    

    [[arisen::notify]]
    void notify( name domain,
                      string msg);
  
  private:
    struct [[arisen::table]] rdata {
      name domain;
      string record_name;
      string dweb_key;
      string full_record;
      uint64_t record_num;
      
      uint64_t primary_key() const { return record_num; }
      uint64_t get_secondary_1() const { return domain.value; }
    };

   void send_summary( name domain,
                                  string message);

   typedef arisen::multi_index<"rdatadb"_n, rdata,
     indexed_by<"bydomain"_n, const_mem_fun<rdata, uint64_t, &rdata::get_secondary_1>>
   > ddns_index;
   
};
/** @}*/ // end of @defgroup ddns dweb