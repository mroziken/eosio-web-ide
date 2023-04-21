#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT tictactoe : public contract {
  public:
    using contract::contract;

    TABLE game{
      name host;
      name challenger;

      uint64_t primary_key() const { return host.value; }
      uint64_t by_challenger() const {return challenger.value;}
    };
    typedef multi_index<"games"_n, game,
    indexed_by<"bychallenger"_n, const_mem_fun<game, uint64_t, &game::by_challenger>>> games;
 

    ACTION welcome(name host, name challenger){
      require_auth(get_self());
      print("Welcome, challengers ", host," and ",challenger,"!");
    }

    ACTION create( name &challenger, name &host ){
      require_auth(host);

      check(host != challenger, "Host and challenger cannot be the same player.");
      check(host != get_self(), "Host cannot be the contract account.");
      check(challenger != get_self(), "Challenger cannot be the contract account.");


      games existing_games(get_self(), get_self().value);
      auto game_iterator = existing_games.find(host.value);

      check(game_iterator == existing_games.end(), "Game already exists between host and challenger.");

      existing_games.emplace(get_self(), [&](auto& new_game) {
          new_game.host = host;
          new_game.challenger = challenger;
      });
    }

  ACTION findgames(name challenger) {
    games games_by_challenger(get_self(), get_self().value);
    auto game_iterator = games_by_challenger.lower_bound(challenger.value);
    while (game_iterator != games_by_challenger.end() && game_iterator->challenger == challenger) {
      print("Game between ", game_iterator->host, " and ", game_iterator->challenger, "\n");
      ++game_iterator;
    }
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

EOSIO_DISPATCH(tictactoe, (welcome)(create)(close)(findgames))