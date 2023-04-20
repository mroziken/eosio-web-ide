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

      games existing_games(get_self(), get_self().value);
      auto game_iterator = existing_games.find(host.value);

      check(game_iterator == existing_games.end(), "Game already exists between host and challenger.");

      existing_games.emplace(get_self(), [&](auto& new_game) {
          new_game.host = host;
          new_game.challenger = challenger;
      });
    }

    ACTION findgames(name &challenger){
      games games_by_challenger(get_self(), get_self().value);
      auto game_iterator = games_by_challenger.lower_bound(challenger.value);
      auto game_iterator_term = games_by_challenger.upper_bound(challenger.value);
      while (game_iterator != game_iterator_term){
        if(game_iterator->challenger == challenger){
          print("Game between ",game_iterator->host, " and ", game_iterator->challenger, "\n");
        }
        else{
          print("xxx = ", game_iterator->challenger);
        }
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