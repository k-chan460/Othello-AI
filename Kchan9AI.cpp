#include "Kchan9AI.hpp"
#include <ics46/factory/DynamicFactory.hpp>
#include "OthelloAI.hpp"
//#include "OthelloGameState.hpp"

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, kchan9::Kchan9AI, "kchan9AI (Required)");


std::vector<std::pair<int,int>> kchan9::Kchan9AI::all_moves(std::unique_ptr<OthelloGameState>& copy){
	std::vector<std::pair<int,int>> moves;
	for(unsigned int x = 0; x < copy->board().width(); x++){
		for(unsigned int y = 0; y < copy->board().height(); y++){
			if(copy->isValidMove(x,y)){
				moves.push_back(std::pair<int,int> (x,y));
			}
		}
	}
	return moves;
}

bool kchan9::Kchan9AI::myturn(std::unique_ptr<OthelloGameState>& copy, int color){
	if(color == 0 && copy->isWhiteTurn()){
		return true;
	}
	else if(color == 1 && copy->isBlackTurn()){
		return true;
	}
	return false;
}

OthelloCell kchan9::Kchan9AI::checkcell(int color){
	if(color == 0){
		return OthelloCell::white;
	}
	return OthelloCell::black;
}

int kchan9::Kchan9AI::eval(std::unique_ptr<OthelloGameState>& copy,int color){
	int score;
	OthelloCell cell = checkcell(color);
	if(color == 0){//white
		score = copy->whiteScore() - copy->blackScore();
	}
	else{
		score = copy->blackScore() - copy->whiteScore();
	}
// corners
	if(copy->board().cellAt(0,0) == cell){
		score += 5;
	}
	if(copy->board().cellAt(0,copy->board().height()-1) == cell){
		score += 5;
	}
	if(copy->board().cellAt(copy->board().width()-1,copy->board().height()-1) == cell){
		score += 5;
	}
	if(copy->board().cellAt(copy->board().width()-1,0) == cell){
		score += 5;
	}

	if(copy->board().cellAt(0,0) != cell){
		score -= 5;
	}
	if(copy->board().cellAt(0,copy->board().height()-1) != cell){
		score -= 5;
	}
	if(copy->board().cellAt(copy->board().width()-1,copy->board().height()-1) != cell){
		score -= 5;
	}
	if(copy->board().cellAt(copy->board().width()-1,0) != cell){
		score -= 5;
	}

// before corners
	// if(copy->board().cellAt(0,1) == cell){
	// 	score -= 5;
	// }
	// if(copy->board().cellAt(1,1) == cell){
	// 	score -= 5;
	// }
	// if(copy->board().cellAt(1,0) == cell){
	// 	score -= 5;
	// }

	// if(copy->board().cellAt(0,copy->board().height()-2) == cell){
	// 	score -= 5;
	// }
	// if(copy->board().cellAt(1,copy->board().height()-2) == cell){
	// 	score -= 5;
	// }
	// if(copy->board().cellAt(1,copy->board().height()-1) == cell){
	// 	score -= 5;
	// }

	// if(copy->board().cellAt(copy->board().width()-2,0) == cell){
	// 	score -= 5;
	// }
	// if(copy->board().cellAt(copy->board().width()-2,1) == cell){
	// 	score -= 5;
	// }
	// if(copy->board().cellAt(copy->board().width()-1,1) == cell){
	// 	score -= 5;
	// }

	// if(copy->board().cellAt(copy->board().width()-2,copy->board().height()-2) == cell){
	// 	score -= 5;
	// }
	// if(copy->board().cellAt(copy->board().width()-2,copy->board().height()-1) == cell){
	// 	score -= 5;
	// }
	// if(copy->board().cellAt(copy->board().width()-1,copy->board().height()-2) == cell){
	// 	score -= 5;
	// }

	return score;
}

int kchan9::Kchan9AI::search(std::unique_ptr<OthelloGameState>& copy, int depth, int color){
	std::vector<std::pair<int,int>> moves = all_moves(copy);
	int plus = -9999;
	int minus = 9999;
	int points;
	if(copy->isGameOver()){
		return eval(copy,color);
	}
	if(depth == 0 || moves.size() == 0){
		return eval(copy,color);
	}
	else if(myturn(copy,color)){
		for(unsigned int i = 0; i < moves.size(); i++){
			std::unique_ptr<OthelloGameState> second = copy->clone();
			second->makeMove(moves[i].first,moves[i].second);
			points = search(second,depth-1,color);
			if(points >= plus){ 
				plus = points;
			}
		}
		return plus;
	}
	else if(myturn(copy,color) == false){
		for(unsigned int i = 0; i < moves.size(); i++){
			std::unique_ptr<OthelloGameState> second = copy->clone();
			second->makeMove(moves[i].first,moves[i].second);
			points = search(second,depth-1,color);
			if(points <= minus){
				minus = points;
			}
		}
		return minus;
	}

}

std::pair<int, int> kchan9::Kchan9AI::chooseMove(const OthelloGameState& state){
	std::pair<int,int> bestmove; 
	int points = 0;
	int highest = -9999;

	int color = 0; // white
	if(state.isBlackTurn()){
		color = 1;
	}

	std::unique_ptr<OthelloGameState> copy = state.clone();
	std::vector<std::pair<int,int>> moves = all_moves(copy);
	for(unsigned int i = 0; i < moves.size(); i++ ){
		std::unique_ptr<OthelloGameState> second = state.clone();
		second->makeMove(moves[i].first, moves[i].second);
		points = search(second,2,color);
		if(points >= highest){
			highest = points;
			bestmove = moves[i];
		}
	}
	return bestmove;
}