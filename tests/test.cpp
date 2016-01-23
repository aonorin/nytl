#include <nytl/nytl.hpp>
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>
#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
using namespace nytl;

using vec1d = vec<1, double>;

template<std::size_t D, typename P, std::size_t A = D>
simplexRegion<D, P, A> convexFromPoints(const std::vector<vec<D, P>>& points)
{
	std::vector<line<D, P>> lines;
	for(auto it = points.cbegin(); it != points.cend(); ++it)
	{
		auto& p = *it;
		for(auto it2 = it; it2 != points.cend(); ++it2)
		{
			auto& p2 = *it2;

			if(&p == &p2) continue;
			lines.push_back(line<D, P>{p, p2});
		}
	}

	std::cout << lines.size() << "lines remaining\n";

	auto lines2 = lines;
	for(auto it = lines.cbegin(); it != lines.cend(); ++it)
	{
		auto& l1 = *it;
		for(auto l2 : lines2)
		{
			if(all(l1.a == l2.a) || all(l1.b == l2.b) || all(l1.b == l2.a) || all(l1.a == l2.b)) 
			{
				std::cout << "continue\n";
				continue;
			}

			if(intersects(l1, l2))
			{
				it = lines.erase(it);
				--it;
				break;
			}
		}
	}

	std::cout << lines.size() << "lines remaining\n";

	simplexRegion<D, P, A> ret;
	for(std::size_t i(0); i < lines.size(); ++i)
	{
		auto line = lines[i];
		lines.erase(lines.cbegin() + i);

		simplex<D, P, A> simp;
		simp.points()[0] = line.a;
		simp.points()[1] = line.b;
		
		std::size_t idx = 1;
		for(auto i2(0); i2 < lines.size(); ++i2)
		{
			auto& line2 = lines[i2];

			if(all(line2.a == line.a))
			{
				simp.points()[++idx] = line2.b;
				lines.erase(lines.cbegin() + i2);
				lines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
			else if(all(line2.b == line.a))
			{
				simp.points()[++idx] = line2.a;
				lines.erase(lines.cbegin() + i2);
				lines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
		
			if(idx == A) break; //all found
		}

		assert(idx == A); //error here! could not construct simplex
		ret.addNoCheck(simp);
	}


	//code dupilcation here -- argh -> search for better loop above
	if(lines.size() == 3) //should be always true?
	{
		auto& line = lines[0];

		simplex<D, P, A> simp;
		simp.points()[0] = line.a;
		simp.points()[1] = line.b;

		std::size_t idx = 1;
		for(std::size_t i(1); i < lines.size(); ++i)
		{
			auto& line2 = lines[i];
			if(all(line2.a == line.a))
			{
				simp.points()[++idx] = line2.b;
				lines.erase(lines.cbegin() + i);
				lines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
			else if(all(line2.b == line.a))
			{
				simp.points()[++idx] = line2.a;
				lines.erase(lines.cbegin() + i);
				lines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
		}
	
		assert(idx == A); //error here! could not construct simplex
		ret.addNoCheck(simp);
	}

	return ret;
}

template<std::size_t D, typename P, std::size_t A>
simplexRegion<D, double, A> intersection2(const simplex<D, P, A>& sa, const simplex<D, P, A>& sb)
{
	//general equotation system that is fulfilled by all points in the intersectio area	
	mat<D + 2, (A + 1) * 2 + 1, P> eqs;
	for(std::size_t i(0); i < A + 1; ++i)
	{
		eqs.col(i) = sa.points()[i];
		eqs.col(A + 1 + i) = -sb.points()[i];

		eqs.row(D)[i] = 1;
		eqs.row(D)[A + 1 + i] = 0;

		eqs.row(D + 1)[i] = 0;
		eqs.row(D + 1)[A + 1 + i] = 1;
	}

	eqs.col((A + 1) * 2).fill(0);

	eqs.row(D)[(A + 1) * 2] = 1;
	eqs.row(D + 1)[(A + 1) * 2] = 1;

	std::cout << eqs << std::endl;

	//les
	auto les = linearEquotationSystem<D + 2, (A + 1) * 2, P>(eqs);
	auto solution = les.solve();

	//analzye solutionSet
	if(!solution.solvable()) //no intersection
	{
		return {};
	}
	else if(solution.unambigouoslySolvable()) //baiscally just a point
	{
		//return cartesian(sa, static_cast<vec<D, P>>(solution.solution()));
		return {};
	}
	else //area
	{
		//assert solution.numberVariables() == A?
		
		domainedSolutionSet<(A + 1) * 2> dss;
		dss.solutionSet_ = solution;
		dss.domains_.fill(linearDomain{0, 1});
		dss.bake();
	
		std::vector<dynVecb> sequences(std::pow(2, solution.numberVariables()), 
				dynVecb(solution.numberVariables()));

		//only allows ~32(maybe 64 or more) dimensions cause of this
		for(std::size_t i(0); i < (1 << solution.numberVariables()); ++i)
		{
			for(std::size_t o(0); o < solution.numberVariables(); ++o)
			{
				sequences[i][o] = (i & (1 << o));
			}

			//std::cout << "seq: " << sequences[i] << "\n";
		}

		std::vector<vec<D, double>> cartesianPoints;
		for(auto& comb : sequences)
		{
			auto sol = dss.solution(dynVeci{0, 1}, comb, 0);
			auto cart = cartesian(sa, vec<A + 1, double>(solution.solution(sol)));
			cartesianPoints.push_back(cart);
			//std::cout << "{0, 1}, " << comb << " --> " << sol << " --> " << cart << "\n";

			sol = dss.solution(dynVeci{1, 0}, comb, 0);
			cart = cartesian(sa, vec<A + 1, double>(solution.solution(sol)));
			cartesianPoints.push_back(cart);
			//std::cout << "{1, 0}, " << comb << " --> " << sol << " --> " << cart << "\n";
		}

		//erase doubled points
		std::sort(cartesianPoints.begin(), cartesianPoints.end(), 
				[](const vec<D, double>& a, const vec<D, double>& b){ return all(a < b); });
		auto newEnd = std::unique(cartesianPoints.begin(), cartesianPoints.end(), 
				[](vec<D, double> a, vec<D, double> b) -> bool { return all(a == b); });
		cartesianPoints.erase(newEnd, cartesianPoints.cend());

		return convexFromPoints<D, double, A>(cartesianPoints);
	}
}

int main()
{
	triangle2f tr1{{50.f, 0.f}, {0.f, 100.f}, {100.f, 100.f}};
	triangle2f tr2{{0.f, 50.f}, {100.f, 50.f}, {50.f, 150.f}};

	auto r = intersection2(tr1, tr2);

	for(auto& s : r.areas())
	{
		std::cout << s.points() << "\n";
	}
}
