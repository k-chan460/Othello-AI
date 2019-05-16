#include "OthelloAI.hpp"

namespace kchan9
{
	class Kchan9AI: public OthelloAI
	{
	public:

		virtual std::pair<int, int> chooseMove(const OthelloGameState& state);
		std::vector<std::pair<int,int>> all_moves(std::unique_ptr<OthelloGameState>& copy);
		bool myturn(std::unique_ptr<OthelloGameState>& copy, int color);
		OthelloCell checkcell(int color);
		int eval(std::unique_ptr<OthelloGameState>& copy,int color);
		int search(std::unique_ptr<OthelloGameState>& copy, int depth, int color);
	};
}