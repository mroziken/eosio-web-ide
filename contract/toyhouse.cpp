#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT toyhouse : public contract {
  public:
    using contract::contract;

    ACTION welcome(name arrival){
      check(has_auth(name("boss")), "You are not the boss");
      check(arrival == name("boss"), "Please don't lie about your name");
      print("Welcome to the Toy House");
    }

    private:
};