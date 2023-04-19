#include <eosio/eosio.hpp>

using namespace eosio;
using namespace std;

CONTRACT toyhouse : public contract {
  public:
    using contract::contract;

    TABLE toy{
      uint64_t id = 0;
      string toyname;
      string toydescrip;
      int64_t quantity;
      name owner = name("boss");

      auto primary_key() const {return id;}
    };
    typedef multi_index<name("toyshelf"), toy> toyshelf_table;

    ACTION addtoy(int128_t id, string toyname, string toydescrip, uint64_t quantity){
      check(has_auth(name("boss")), "You are not the boss");
      toyshelf_table _toyshelf(get_self(), get_self().value);
      // add the new toy if it does not exist
      if( id == -1){
        _toyshelf.emplace(get_self(), [&](auto& new_toy_row){
        new_toy_row.id = _toyshelf.available_primary_key();
        new_toy_row.toyname = toyname;
        new_toy_row.toydescrip = toydescrip;
        new_toy_row.quantity = quantity;
        });
      } else{
        auto itr = _toyshelf.find(id);
        check(itr != _toyshelf.end(),"toy with given id not found");

        _toyshelf.modify(itr, get_self(), [&](auto& row_to_modify){
          row_to_modify.quantity += quantity;
        });
      }
    }

    ACTION removetoy( uint64_t id, uint64_t quantity){
      require_auth(name("boss"));
      toyshelf_table _toyshelf(get_self(), get_self().value);
      //find toy
      auto itr = _toyshelf.find(id);
      //if does not exists, error
      //print("itr=", itr);
      check(itr != _toyshelf.end(),"toy with given id not found");
      //print("itr=", itr, "itr->quantity=", itr->quantity);
      
      if((itr->quantity - quantity) == 0){
        //if resulting quantity is 0, delete toy row
        _toyshelf.erase(itr);
        print("sucessfully removed toy", id, " x", quantity);
      }
      else{
        //if resulting quantity is <0, error
        //check( (itr->quantity - quantity) > 0, "You are trying to have negative toys");
        if(itr->quantity < quantity){
          print("You are trying to have negative toys");
        }
        else{
          //substract quantity
          _toyshelf.modify(itr, get_self(), [&](auto& row_to_modify){
            row_to_modify.quantity -= quantity;
          });
        }
      }
      
    }

    private:
};