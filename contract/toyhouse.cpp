#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT toyhouse : public contract {
  public:
    using contract::contract;

    ACTION welcome(name arrival){
      print("Welcome to the Toy House");
    }

    private:
};