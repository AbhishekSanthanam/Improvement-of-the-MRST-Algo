// TIDAA_CP_Steiner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <bits/stdc++.h>
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
#include <utility>
#include <chrono>

using namespace std;

typedef pair<int, int> grid_point;
typedef pair < grid_point, grid_point > edge;

struct comp_t
{
    bool operator()(const grid_point &a, const grid_point &b) const
    {
        if (a.second != b.second) return (a.second < b.second);
	return (a.first < b.first);
    }
};


typedef struct Graph {

    set<grid_point, comp_t> P_x_sorted; 
    set< grid_point > P_y_sorted;    // This is required, yes?
    list<edge> E;
    bool grown=false;
    int L=0;
} Graph;

vector<int> TerMapPermut(vector<pair<int,int>> terminals){

	for(int i=0;i<terminals.size();i++){

		swap(terminals[i].first, terminals[i].second);
	}

	sort(terminals.begin(),terminals.end());

	map<pair<int,int>,int> order;    // map pair(x,y) with it's order

	for(int i=0;i<terminals.size();i++){
		swap(terminals[i].first, terminals[i].second);
	}

	for(int i=0;i<terminals.size();i++){
		order[terminals[i]]=i+1;
	}

  //By default the sort function sorts the vector elements on basis of first element of pairs.
	sort(terminals.begin(),terminals.end());

	vector<int> C;

	for(int i=0;i<terminals.size();i++){
		C.push_back(order[terminals[i]]);
	}
	return C;
}

Graph Permut(vector<int> C){
	Graph P;
	for(int i=0;i<C.size();i++){

		P.P_x_sorted.insert(make_pair(C[i],i+1));
		P.P_y_sorted.insert(make_pair(C[i],i+1));
	}
	return P;
}


bool extreme(Graph& G) {

	if (G.P_x_sorted.size() == 2){
		auto p1 = *(G.P_x_sorted.begin()), p2 = *(G.P_x_sorted.rbegin());
		grid_point p3 = {p1.first, p2.second};
		if (p1 != p3)
			G.E.push_back ( {p1 , p3 } );
		if (p2 != p3)
			G.E.push_back ( {p2 , p3 } );

		G.P_x_sorted.erase(G.P_x_sorted.begin());
		G.P_y_sorted.erase(p1);
		G.L += abs(p1.first  - p2.first) + abs(p2.second - p1.second);
		return false;
	}

	else if (G.P_x_sorted.size() == 1){
		return false;

	}
	set<grid_point,comp_t>::reverse_iterator x_rit = G.P_x_sorted.rbegin() , x_rit2 = G.P_x_sorted.rbegin(); ++x_rit2;
	set<grid_point,comp_t>::iterator x_it = G.P_x_sorted.begin() , x_it2 = G.P_x_sorted.begin()  ; ++x_it2;
	set<grid_point,comp_t>::reverse_iterator y_rit = G.P_y_sorted.rbegin() , y_rit2 = G.P_y_sorted.rbegin(); ++y_rit2;
	set<grid_point,comp_t>::iterator y_it = G.P_y_sorted.begin() , y_it2 = G.P_y_sorted.begin()  ; ++y_it2;

	grid_point adj_p, p;
	if (x_it -> second != x_it2 -> second){

		p = *x_it;
		G.L +=x_it2->second - x_it -> second ;
		adj_p = { p.first , x_it2->second };    // add epsilon_i_j+1
		G.P_x_sorted.erase(x_it);   //removed epsilon_i_j
		G.P_y_sorted.erase(p);

	}
	else if ( x_rit -> second != x_rit2 -> second ){
		

		p = *(G.P_x_sorted.rbegin());

		G.L +=x_rit->second - x_rit2 -> second ;


		adj_p = { p.first , x_rit2 -> second };
		
		G.P_x_sorted.erase( --G.P_x_sorted.end());  //removed epsilon_i_j
		G.P_y_sorted.erase( p );


		

	}
	else if ( y_rit -> first != y_rit2 -> first ){


		p = *y_rit;

		G.L +=y_rit->first - y_rit2 -> first ;

		adj_p = { y_rit2 -> first , p.second  };

		G.P_y_sorted.erase( p );   //removed epsilon_i_j
		G.P_x_sorted.erase( p );

		

	}

	else if ( y_it -> first != y_it2 -> first ){
	

		p = *y_it;

	    G.L +=y_it2->first - y_it -> first ;

		adj_p = { y_it2->first , p.second  };
		G.P_y_sorted.erase( p);
		G.P_x_sorted.erase( p );


		
	}
	else return true;

	G.P_x_sorted.insert( adj_p);
	G.P_y_sorted.insert( adj_p);
	G.E.push_back( { p, adj_p } );
	return extreme(G);

}



bool compare_edge_lists(list<edge> l1, list<edge> l2)
{
	if(l1.size()!=l2.size())
		return false;
	list<edge>:: iterator it=l1.begin();
	list<edge>:: iterator it2=l2.begin();
	  
	for(int i=0;i<l1.size();i++)
	{
	    if(*it!=*it2)
	    {
	      return false;
	    }
	    advance(it,1);
	    advance(it2,1);
    }

	return true;
}


void fork(Graph &G, list <Graph> &TreeList){

	Graph G1, G2, G3;
	G1=G; G2=G; G3=G;




	set<grid_point,comp_t>::reverse_iterator x_rit = G.P_x_sorted.rbegin() , x_rit2 = G.P_x_sorted.rbegin(); ++x_rit2;++x_rit2;
	set<grid_point,comp_t>::iterator x_it = G.P_x_sorted.begin() , x_it2 = G.P_x_sorted.begin()  ; ++x_it2;++x_it2;
	set<grid_point>::reverse_iterator y_rit = G.P_y_sorted.rbegin() , y_rit2 = G.P_y_sorted.rbegin(); ++y_rit2;++y_rit2;
	set<grid_point>::iterator y_it = G.P_y_sorted.begin() , y_it2 = G.P_y_sorted.begin()  ; ++y_it2;++y_it2;


	if( x_it -> second != x_it2 -> second)
	{ 
		grid_point epsilon_i_j=*x_it;++x_it; pair<int,int> epsilon_i_k = *x_it;
		grid_point epsilon_i_plus_1_j = {epsilon_i_j.first,x_it2->second };
		pair<int,int> epsilon_i_plus_1_k = {epsilon_i_k.first,x_it2->second };

		G1.P_y_sorted.erase(epsilon_i_j); G1.P_y_sorted.erase(epsilon_i_k);
		G1.P_x_sorted.erase(epsilon_i_j); G1.P_x_sorted.erase(epsilon_i_k);
		G1.P_y_sorted.insert(epsilon_i_plus_1_j); G1.P_y_sorted.insert(epsilon_i_plus_1_k);
		G1.P_x_sorted.insert(epsilon_i_plus_1_j); G1.P_x_sorted.insert(epsilon_i_plus_1_k);

		G1.E.push_back(make_pair(epsilon_i_j,epsilon_i_plus_1_j));

		G1.E.push_back(make_pair(epsilon_i_k,epsilon_i_plus_1_k));


		G1.L += 2 *(epsilon_i_plus_1_j.second - epsilon_i_j.second);
		TreeList.push_back(G1);

		G2.P_y_sorted.erase(epsilon_i_j); G2.P_y_sorted.erase(epsilon_i_k);
		G2.P_x_sorted.erase(epsilon_i_j); G2.P_x_sorted.erase(epsilon_i_k);
		G2.P_y_sorted.insert(epsilon_i_plus_1_j);
		G2.P_x_sorted.insert(epsilon_i_plus_1_j);

		G2.E.push_back(make_pair(epsilon_i_j,epsilon_i_plus_1_j));
		G2.E.push_back(make_pair(epsilon_i_j,epsilon_i_k));

		G2.L+=(epsilon_i_plus_1_j.second - epsilon_i_j.second +abs(epsilon_i_j.first-epsilon_i_k.first));
		TreeList.push_back(G2);

		G3.P_y_sorted.erase(epsilon_i_j); G3.P_y_sorted.erase(epsilon_i_k);
		G3.P_x_sorted.erase(epsilon_i_j); G3.P_x_sorted.erase(epsilon_i_k);
		G3.P_y_sorted.insert(epsilon_i_plus_1_k);
		G3.P_x_sorted.insert(epsilon_i_plus_1_k);

		G3.E.push_back(make_pair(epsilon_i_k,epsilon_i_plus_1_k));
		G3.E.push_back(make_pair(epsilon_i_j,epsilon_i_k));

		G3.L+=(epsilon_i_plus_1_j.second - epsilon_i_j.second +abs(epsilon_i_j.first-epsilon_i_k.first));
		TreeList.push_back(G3);  

		for(auto it=TreeList.begin();it!=TreeList.end();it++)
		{
			if(it->P_x_sorted==G.P_x_sorted && it->P_y_sorted==G.P_y_sorted && compare_edge_lists(it->E,G.E) && it->grown==G.grown && it->L==G.L)
			{	
				TreeList.erase(it);
				break;
			}
		}

		return ;
	}

	if(x_rit -> second != x_rit2 -> second )
		{

		pair<int,int> epsilon_i_j=*x_rit;++x_rit; pair<int,int> epsilon_i_k = *x_rit;
		pair<int,int> epsilon_i_minus_1_j = make_pair(epsilon_i_j.first, x_rit2 -> second);
		pair<int,int> epsilon_i_minus_1_k = make_pair(epsilon_i_k.first, x_rit2 -> second);

		G1.P_y_sorted.erase(epsilon_i_j); G1.P_y_sorted.erase(epsilon_i_k);
		G1.P_x_sorted.erase(epsilon_i_j); G1.P_x_sorted.erase(epsilon_i_k);
		G1.P_y_sorted.insert(epsilon_i_minus_1_j); G1.P_y_sorted.insert(epsilon_i_minus_1_k);
		G1.P_x_sorted.insert(epsilon_i_minus_1_j); G1.P_x_sorted.insert(epsilon_i_minus_1_k);

		G1.E.push_back(make_pair(epsilon_i_j,epsilon_i_minus_1_j));
		G1.E.push_back(make_pair(epsilon_i_k,epsilon_i_minus_1_k));

		G1.L+= 2 *(-epsilon_i_minus_1_j.second + epsilon_i_j.second);;
		TreeList.push_back(G1);

		G2.P_y_sorted.erase(epsilon_i_j); G2.P_y_sorted.erase(epsilon_i_k);
		G2.P_x_sorted.erase(epsilon_i_j); G2.P_x_sorted.erase(epsilon_i_k);
		G2.P_y_sorted.insert(epsilon_i_minus_1_j);
		G2.P_x_sorted.insert(epsilon_i_minus_1_j);

		G2.E.push_back(make_pair(epsilon_i_j,epsilon_i_minus_1_j));
		G2.E.push_back(make_pair(epsilon_i_j,epsilon_i_k));

		G2.L+=(-epsilon_i_minus_1_j.second + epsilon_i_j.second+abs(epsilon_i_j.first-epsilon_i_k.first));
		TreeList.push_back(G2);

		G3.P_y_sorted.erase(epsilon_i_j); G3.P_y_sorted.erase(epsilon_i_k);
		G3.P_x_sorted.erase(epsilon_i_j); G3.P_x_sorted.erase(epsilon_i_k);
		G3.P_y_sorted.insert(epsilon_i_minus_1_k);
		G3.P_x_sorted.insert(epsilon_i_minus_1_k);

		G3.E.push_back(make_pair(epsilon_i_k,epsilon_i_minus_1_k));
		G3.E.push_back(make_pair(epsilon_i_j,epsilon_i_k));

		G3.L+=(-epsilon_i_minus_1_j.second + epsilon_i_j.second+abs(epsilon_i_j.first-epsilon_i_k.first));
		TreeList.push_back(G3);

		//TreeList.remove(G);
		list<Graph>:: iterator it;
		for(it=TreeList.begin();it!=TreeList.end();it++)
		{
			if(it->P_x_sorted==G.P_x_sorted && it->P_y_sorted==G.P_y_sorted && compare_edge_lists(it->E,G.E) && it->grown==G.grown && it->L==G.L)
			{
				TreeList.erase(it);break;
			}
		}
		return ;

	}

	if(y_it -> first != y_it2 -> first )
	{
		pair<int,int> epsilon_i_j = *y_it;++y_it; pair<int,int> epsilon_i_k=*y_it;

		pair<int,int> epsilon_i_j_plus_1= {y_it2->first,epsilon_i_j.second };
		pair<int,int> epsilon_i_k_plus_1= {y_it2->first,epsilon_i_k.second };

		G1.P_y_sorted.erase(epsilon_i_j); G1.P_y_sorted.erase(epsilon_i_k);
		G1.P_x_sorted.erase(epsilon_i_j); G1.P_x_sorted.erase(epsilon_i_k);
		G1.P_y_sorted.insert(epsilon_i_j_plus_1); G1.P_y_sorted.insert(epsilon_i_k_plus_1);
		G1.P_x_sorted.insert(epsilon_i_j_plus_1); G1.P_x_sorted.insert(epsilon_i_k_plus_1);

		G1.E.push_back(make_pair(epsilon_i_j,epsilon_i_j_plus_1));
		G1.E.push_back(make_pair(epsilon_i_k,epsilon_i_k_plus_1));

		G1.L+=2*(epsilon_i_j_plus_1.first - epsilon_i_j.first);
		TreeList.push_back(G1);

		G2.P_y_sorted.erase(epsilon_i_j); G2.P_y_sorted.erase(epsilon_i_k);
		G2.P_x_sorted.erase(epsilon_i_j); G2.P_x_sorted.erase(epsilon_i_k);
		G2.P_y_sorted.insert(epsilon_i_j_plus_1);
		G2.P_x_sorted.insert(epsilon_i_j_plus_1);

		G2.E.push_back(make_pair(epsilon_i_j,epsilon_i_j_plus_1));
		G2.E.push_back(make_pair(epsilon_i_j,epsilon_i_k));

		G2.L+=(epsilon_i_j_plus_1.first - epsilon_i_j.first+abs(epsilon_i_j.second-epsilon_i_k.second));
		TreeList.push_back(G2);

		G3.P_y_sorted.erase(epsilon_i_j); G3.P_y_sorted.erase(epsilon_i_k);
		G3.P_x_sorted.erase(epsilon_i_j); G3.P_x_sorted.erase(epsilon_i_k);
		G3.P_y_sorted.insert(epsilon_i_k_plus_1);
		G3.P_x_sorted.insert(epsilon_i_k_plus_1);

		G3.E.push_back(make_pair(epsilon_i_k,epsilon_i_k_plus_1));
		G3.E.push_back(make_pair(epsilon_i_j,epsilon_i_k));

		G3.L+=(epsilon_i_j_plus_1.first - epsilon_i_j.first+abs(epsilon_i_j.second-epsilon_i_k.second));
		TreeList.push_back(G3);

		//TreeList.remove(G);
		list<Graph>:: iterator it;
		for(it=TreeList.begin();it!=TreeList.end();it++)
		{
			if(it->P_x_sorted==G.P_x_sorted && it->P_y_sorted==G.P_y_sorted && it->grown==G.grown && it->L==G.L  && compare_edge_lists(it->E,G.E))
			{
				TreeList.erase(it);break;
			}
		}
		return ;

	}


	if(y_rit -> first != y_rit2 -> first)
	{
		pair<int,int> epsilon_i_j=*y_rit;++y_rit; pair<int,int> epsilon_i_k=*y_rit;

		pair<int,int> epsilon_i_j_minus_1={y_rit2 -> first,epsilon_i_j.second };
		pair<int,int> epsilon_i_k_minus_1={y_rit2 -> first,epsilon_i_k.second };

		G1.P_y_sorted.erase(epsilon_i_j); G1.P_y_sorted.erase(epsilon_i_k);
		G1.P_x_sorted.erase(epsilon_i_j); G1.P_x_sorted.erase(epsilon_i_k);
		G1.P_y_sorted.insert(epsilon_i_j_minus_1); G1.P_y_sorted.insert(epsilon_i_k_minus_1);
		G1.P_x_sorted.insert(epsilon_i_j_minus_1); G1.P_x_sorted.insert(epsilon_i_k_minus_1);

		G1.E.push_back(make_pair(epsilon_i_j,epsilon_i_j_minus_1));
		G1.E.push_back(make_pair(epsilon_i_k,epsilon_i_k_minus_1));

		G1.L+=2*(epsilon_i_j.first - epsilon_i_k_minus_1.first);
		TreeList.push_back(G1);

		G2.P_y_sorted.erase(epsilon_i_j); G2.P_y_sorted.erase(epsilon_i_k);
		G2.P_x_sorted.erase(epsilon_i_j); G2.P_x_sorted.erase(epsilon_i_k);
		G2.P_y_sorted.insert(epsilon_i_j_minus_1);
		G2.P_x_sorted.insert(epsilon_i_j_minus_1);

		G2.E.push_back(make_pair(epsilon_i_j,epsilon_i_j_minus_1));
		G2.E.push_back(make_pair(epsilon_i_j,epsilon_i_k));

		G2.L+=(epsilon_i_j.first - epsilon_i_k_minus_1.first+abs(epsilon_i_j.second-epsilon_i_k.second));
		TreeList.push_back(G2);

		G3.P_y_sorted.erase(epsilon_i_j); G3.P_y_sorted.erase(epsilon_i_k);
		G3.P_x_sorted.erase(epsilon_i_j); G3.P_x_sorted.erase(epsilon_i_k);
		G3.P_y_sorted.insert(epsilon_i_k_minus_1);
		G3.P_x_sorted.insert(epsilon_i_k_minus_1);

		G3.E.push_back(make_pair(epsilon_i_k,epsilon_i_k_minus_1));
		G3.E.push_back(make_pair(epsilon_i_j,epsilon_i_k));

		G3.L+=(epsilon_i_j.first - epsilon_i_k_minus_1.first+abs(epsilon_i_j.second-epsilon_i_k.second));
		TreeList.push_back(G3);

		//TreeList.remove(G);
		list<Graph>:: iterator it;
		for(it=TreeList.begin();it!=TreeList.end();it++)
		{
			if(it->P_x_sorted==G.P_x_sorted && it->P_y_sorted==G.P_y_sorted && compare_edge_lists(it->E,G.E) && it->grown==G.grown && it->L==G.L)
			{
				TreeList.erase(it);break;
			}
		}
		return;

	}

	cout<<"Control flow shouldn't come here"<<endl;
	exit(1);
	return;

}

Graph Const_optRST(Graph G) {

    list <Graph> TreeList;
    G.grown = false;
    TreeList.emplace_back(G);
    list <Graph> ::iterator it = TreeList.begin();
    while (it != TreeList.end()) {
        if (it->grown == false) {
            if (extreme(*it)) {

                fork(*it, TreeList);
				it = TreeList.begin();
            }
            else {
                it->grown = true;
				++it;
            }
        }
		else ++it;
    }
    auto opt_rst = std::min_element(TreeList.begin(), TreeList.end(), [](const Graph& lhs, const Graph& rhs) { return lhs.L < rhs.L; });
    return *opt_rst;
}
void print_pair (grid_point p, string s){
	cout << s << " " << p.first << " "<< p.second << endl;
}
void optimise( list <edge> & Tree , grid_point p ){

	int cnt = 0;
	grid_point p1,p2;
	auto itrp1 = Tree.end(), itrp2 = Tree.end();

	for (auto it = Tree.begin(); it != Tree.end(); ++it){
		auto e = *it;
		if (e.first == p ){
			if (e.second != p ){
				if (cnt == 0){
					p1 = e.second;
					itrp1 = it;
					cnt++;
				}
				else {
					p2 = e.second;
					itrp2 = it;
					break;
				}
			}
			
		}
		else if (e.second == p){
			if (cnt == 0){
				p1 = e.first;
				itrp1 = it;
				cnt++;
			}
			else {
				p2 = e.first;
				itrp2 = it;
				break;
			}

		}
	}

	auto itr1 = Tree.end() , itr2 = Tree.end();
	int g1 = 0, g2 = 0; 
	for (auto it = Tree.begin(); it != Tree.end(); ++it ){
		auto e = *it;
		if (e.first == p1){
			if (e.second != p){
				if (itr1 != Tree.end()){
					return;
				}
				else{
					if (e.second.second == p1.second ){
						itr1 = it; 
						g1 = e.second.first > p1.first;
					}
					else return;
				}
			}
		}
		else if (e.second == p1){
			if (e.first != p){
				if (itr1 != Tree.end()){
					return;
				}
				else{
					if (e.first.second == p1.second ){
						itr1 = it;
						g1 = e.first.first > p1.first;

					}
					else return;
				}
			}

		}
		if (e.first == p2){
			if (e.second != p){
				if (itr2 != Tree.end()){
					return;
				}
				else{
					if (e.second.second == p2.second ){
						itr2 = it;
						g2 = e.second.first > p1.first;
					}
					else return;
				}
				
			}
		}
		else if (e.second == p2){
			if (e.first != p){
				if (itr2 != Tree.end()){
					return;
				}
				else{
					if (e.first.second == p2.second ){
						itr2 = it;
						g2 = e.first.first > p2.first;
					}
					else return;
				}
			}
		}
	}

	if (g1 != g2) return;

	int y1 = ((itr1->first == p1) ? (itr1->second.first) : (itr1->first.first));
	int y2 = ((itr2->first == p2) ? (itr2->second.first) : (itr2->first.first));

	int y_n = (g1 ? min(y1,y2) :max(y1,y2) ); 

	grid_point new_p = {y_n, p.second };
	if (itrp1 -> first == p){
		itrp1 -> second = new_p;
	}
	else itrp1 -> first = new_p;
	int del = 1;

	if (itr1 -> first == p1){
		if (y1 == y_n)
		itr1 -> first = new_p;
		else {
			itr1 -> first = {y_n, p1.second};
			*itrp2 = { itr1->first, new_p };
			del = 0;
		}
	}
	else{
		if (y1 == y_n)
		itr1 -> second = new_p;
		else {
			itr1 -> second = {y_n, p1.second};
			*itrp2 = { itr1->second, new_p };
			del = 0;
		}
	}

	if (itr2 -> first == p2){
		if (y2 == y_n)
		itr2 -> first = new_p;
		else {
			itr2 -> first = {y_n, p2.second};
			*itrp2 = { itr2->first, new_p };
			del = 0;
		}
	}
	else{
		if (y2 == y_n)
		itr2 -> second = new_p;
		else {
			itr2 -> second = {y_n, p2.second};
			*itrp2 = { itr2->second, new_p };
			del = 0;
		}
	}

	if (y1 == y2){
		Tree.erase(itrp2);
	}


}
bool check(list<edge> &Tree, edge e){
	int cnt1 = 0, cnt2 = 0;
	for ( auto e1: Tree){
		if (e1.first == e.first || e1.second == e.first) cnt1++;
		if (e1.first == e.second || e1.second == e.second) cnt2++;
	}
	return (cnt1< 3 && cnt2 < 3);
}

void combine(list < edge> & Tree, int y_coord){
	set < grid_point > s;
	for (auto e: Tree){
		if (e.first.first == y_coord && e.second.first == y_coord && check(Tree,e)){
			s.insert({min(e.first.second, e.second.second),max(e.first.second, e.second.second)});
		}
	}
	if (s.size() == 0) return;
	auto it = s.begin();
	int st = it->first;
	int en = it->second;
	for (it++; it != s.end(); ++it){
		int nst = it->first;
		int pst = prev(it)->first;
		if (nst == en){
			Tree.remove((edge){(grid_point){y_coord,pst}, (grid_point){y_coord,en}});
			Tree.remove((edge){(grid_point){y_coord,en}, (grid_point){y_coord,pst}});
		}
		else{
			if (pst != st){

				Tree.remove({(grid_point){y_coord,pst}, (grid_point){y_coord,en}});
				Tree.remove({(grid_point){y_coord,en}, (grid_point){y_coord,pst}});
				Tree.push_back({(grid_point){y_coord,st}, (grid_point){y_coord,en}});
			}
			st = nst;
		}
		en = it->second;
	}
	if (prev(it)->first != st){
		int pst = prev(it)->first;
		Tree.remove({(grid_point){y_coord,pst}, (grid_point){y_coord , en}});
		Tree.remove({(grid_point){y_coord,en}, (grid_point){y_coord,pst}});
		Tree.push_back({(grid_point){y_coord,st}, (grid_point){y_coord,en}});
	}

}




int main()
{

    int n ;cin>>n; 
    int ans = 0;

    Graph G;
	for (int i =0 ; i < n; ++i){
		grid_point p;
		cin>>p.second>>p.first;
		G.P_x_sorted.insert(p);
		G.P_y_sorted.insert(p);
	}

    set<grid_point>:: iterator it;
    list < edge > Tree;


    int cur_sz = 0;

    Graph district;

	auto start = chrono::high_resolution_clock::now();

	grid_point intr = {-1,-1};
	list < grid_point > op_p;

    for (it=(G.P_x_sorted).begin();it!=(G.P_x_sorted).end();it++){



		district.P_x_sorted.insert(*it);
		district.P_y_sorted.insert(*it);
		++cur_sz;
		if (cur_sz == 7) {

			Graph d_ret = Const_optRST(district);
			if (intr.first != -1)
			combine(d_ret.E, intr.first);
			combine(d_ret.E, it->first);
			Tree.splice( Tree.end(), d_ret.E );
			if (intr.first != -1){
				optimise(Tree, intr);
			}
			intr = *it;

			district.P_x_sorted.clear();
			district.P_y_sorted.clear();
			district.E.clear();
			district.L = 0;
			district.grown = false;
			district.P_x_sorted.insert(*it);
			district.P_y_sorted.insert(*it);

			cur_sz = 1;
		}


    }

    if (cur_sz > 1 ) {

		Graph d_ret = Const_optRST(district);
	
		Tree.splice( Tree.end(), d_ret.E );
		if (intr.first != -1)
			optimise(Tree,intr );

     }
	 for (auto it=Tree.begin();it!=Tree.end();it++){
		 auto p1 = it->first, p2 = it->second;
		 if (p1.second > p2.second ){
			 it->first = p2;
			 it->second = p1;
		 }
	 }
	auto stop = chrono::high_resolution_clock::now();

	auto duration_sec = chrono::duration_cast<chrono::seconds>(stop - start);
	auto duration_milli_sec = chrono::duration_cast<chrono::milliseconds>(stop - start);
 
	cout << "Time Taken in seconds is "<<duration_sec.count() << endl;
	cout << "Time Taken in Milli seconds is "<<duration_milli_sec.count() << endl;
	

  
   	for(auto it2=Tree.begin();it2!=Tree.end();it2++)
    {
    	//cout<<"("<<((*it2).first).first<<","<<((*it2).first).second<<") <-> ("<<((*it2).second).first<<","<<((*it2).second).second<<") " << endl;
		ans += abs((it2->first).first - it2->second.first) + abs(it2->first.second - it2->second.second);
	}   
    cout<<"Length of the RMST is "<<ans<<endl;
	FILE* fp=fopen("Tree.csv","w+");	
	fprintf(fp,"x1,y1,x2,y2\n");	
	for(auto it2=Tree.begin();it2!=Tree.end();it2++)
    {
		fprintf(fp,"%s,%s,%s,%s\n",(it2->first).first,it2->first.second,it2->second.first,it2->second.second);  
	}
    return 0;
}

