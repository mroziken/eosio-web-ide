#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT tictactoe : public contract {
  public:
    using contract::contract;

    TABLE game{
      name host;
      name challenger;

      uint64_t primary_key() const { return host.value; }
    };
    typedef multi_index<"games"_n, game> games;


    ACTION welcome(name host, name challenger){
      require_auth(get_self());
      print("Welcome, challengers ", host," and ",challenger,"!");
    }

    ACTION create( const name &challenger, name &host ){
      require_auth(host);

      games existing_games(get_self(), get_self().value);
      auto game_iterator = existing_games.find(host.value);

      check(game_iterator == existing_games.end(), "Game already exists between host and challenger.");

      existing_games.emplace(get_self(), [&](auto& new_game) {
          new_game.host = host;
          new_game.challenger = challenger;
      });
    }

    ACTION close(name challenger, name host){
      require_auth(host);

      games existing_games(get_self(),get_self().value);
      auto game_iterator = existing_games.find(host.value);

      check(game_iterator != existing_games.end(), "Game does not exist!");

      existing_games.erase(game_iterator);
    }

    private:
};